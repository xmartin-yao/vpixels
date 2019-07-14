////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 Xueyi Yao
//
// This file is part of VPixels.
//
// VPixels is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// VPixels is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#include <Python.h>
#include "PyBmp.h"
#include "PyUtil.h"
#include "Bmp.h"
#include "Exception.h"

/////////////////
// data for Bmp_Type
///////////////////////////////
typedef struct PyBmpObject
{
  PyObject_HEAD
  vp::Bmp* pBmp;
} PyBmpObject;

/////////////////////
namespace PyBmpImpl
{
  // Python special methods
  PyObject* New( PyTypeObject* type, PyObject* args, PyObject* kw );
  int  Init( PyBmpObject* self, PyObject* args, PyObject* kw );
  void Dealloc( PyBmpObject* self );
  PyObject* Repr( PyBmpObject* self );

  // methods of Bmp_Type (exposed to Python)
  PyObject* Import( PyBmpObject* self, PyObject* arg );
  PyObject* Export( PyBmpObject* self, PyObject* args );
  PyObject* Clone( PyBmpObject* self );
  PyObject* BitsPerPixel( PyBmpObject* self );
  PyObject* Width( PyBmpObject* self );
  PyObject* Height( PyBmpObject* self );
  PyObject* Dimension( PyBmpObject* self );
  PyObject* ColorTableSize( PyBmpObject* self );
  PyObject* SetColorTable( PyBmpObject* self, PyObject* args );
  PyObject* GetColorTable( PyBmpObject* self, PyObject* args );
  PyObject* SetAllPixels( PyBmpObject* self, PyObject* args );
  PyObject* SetPixel( PyBmpObject* self, PyObject* args );
  PyObject* GetPixel( PyBmpObject* self, PyObject* args );

  // utils
  vp::Bmp* NewBmp( PyObject* args, PyObject* kw );

  // type methods
  PyMethodDef Methods[]= {
    // cannot use 'import' as name, which causes SyntaxError
    MDef( importf,        Import,         METH_O,       "import from BMP file." )
    MDef( export,         Export,         METH_VARARGS, "export to BMP file." )
    MDef( clone,          Clone,          METH_NOARGS,  "clone itself and create a new object." )
    MDef( bitsperpixel,   BitsPerPixel,   METH_NOARGS,  "get color resolution." )
    MDef( bpp,            BitsPerPixel,   METH_NOARGS,  "get color resolution." )
    MDef( width,          Width,          METH_NOARGS,  "get width." )
    MDef( height,         Height,         METH_NOARGS,  "get height." )
    MDef( dimension,      Dimension,      METH_NOARGS,  "get width and height." )
    MDef( colortablesize, ColorTableSize, METH_NOARGS,  "get color table size." )
    MDef( setcolortable,  SetColorTable,  METH_VARARGS, "set a color table entry." )
    MDef( setcolor,       SetColorTable,  METH_VARARGS, "set a color table entry." )
    MDef( getcolortable,  GetColorTable,  METH_VARARGS, "get a color table entry." )
    MDef( getcolor,       GetColorTable,  METH_VARARGS, "get a color table entry." )
    MDef( setallpixels,   SetAllPixels,   METH_VARARGS, "set all pixels to same color." )
    MDef( setall,         SetAllPixels,   METH_VARARGS, "set all pixels to same color." )
    MDef( setpixel,       SetPixel,       METH_VARARGS, "set the color of a pixel." )
    MDef( getpixel,       GetPixel,       METH_VARARGS, "get the color of a pixel." )
    { nullptr, nullptr, 0, nullptr } 
  };
} //PyBmpImpl

/////////////
// Bmp_Type
//////////////////////////////
PyTypeObject PyBmp::Bmp_Type = {
  PyVarObject_HEAD_INIT( nullptr, 0 )
  "vp.bmp",                       // tp_name
  sizeof(PyBmpObject),            // tp_basicsize
  0,                              // tp_itemsize
  (destructor)PyBmpImpl::Dealloc, // tp_dealloc
  0,                              // tp_print
  0,                              // tp_getattr
  0,                              // tp_setattr
  0,                              // tp_compare
  (reprfunc)PyBmpImpl::Repr,      // tp_repr
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping
  0,                              // tp_hash
  0,                              // tp_call
  0,                              // tp_str
  0,                              // tp_getattro
  0,                              // tp_setattro
  0,                              // tp_as_buffer
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, // tp_flags, allow subclass
  "vp.bmp object",                // tp_doc
  0,                              // tp_traverse
  0,                              // tp_clear
  0,                              // tp_richcompare
  0,                              // tp_weaklistoffset
  0,                              // tp_iter
  0,                              // tp_iternext
  PyBmpImpl::Methods,             // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  0,                              // tp_base
  0,                              // tp_dict
  0,                              // tp_descr_get
  0,                              // tp_descr_set
  0,                              // tp_dictoffset
  (initproc)PyBmpImpl::Init,      // tp_init
  0,                              // tp_alloc
  PyBmpImpl::New,                 // tp_new
  0,                              // tp_free
  0,                              // tp_is_gc
  0,                              // tp_bases
  0,                              // tp_mro
  0,                              // tp_cache
  0,                              // tp_subclasses
  0,                              // tp_weaklist
  0,                              // tp_del
  0                               // tp_version_tag
};

////////////////////////////////////////////////
PyObject* PyBmpImpl::New( PyTypeObject* type, PyObject*, PyObject* )
{
  PyObject* self = type->tp_alloc( type, 0 );
  reinterpret_cast<PyBmpObject*>(self)->pBmp = nullptr;

  return self;
}

////////////////////////////////////////////////
int PyBmpImpl::Init( PyBmpObject* self, PyObject* args, PyObject* kw )
{
  if( self->pBmp != nullptr )
  {
    PyErr_SetString( PyExc_Exception, "__init__ already called" );
    return -1;
  }

  if( PyTuple_Size(args) == 0 && kw == nullptr )
    self->pBmp = new vp::Bmp();
  else
    self->pBmp = NewBmp( args, kw );

  if( self->pBmp == nullptr )
    return -1;

  return 0;
}

///////////////////////////////////////
void PyBmpImpl::Dealloc( PyBmpObject* self )
{
  if( self->pBmp != nullptr )
  {
    delete self->pBmp;
    self->pBmp = nullptr;
  }

  self->ob_type->tp_free( self );
}

///////////////////////////////////////
PyObject* PyBmpImpl::Repr( PyBmpObject* self )
{
  return PyString_FromFormat( "<%s object: bpp=%d %dx%d>",
                              self->ob_type->tp_name, self->pBmp->BitsPerPixel(),
                              self->pBmp->Width(), self->pBmp->Height() );                            
}

/////////////////////////
// keywords: "bpp", "width" and "height"
/////////////////////////////////////////////////////
vp::Bmp* PyBmpImpl::NewBmp( PyObject* args, PyObject* kw )
{
  uint8_t bpp = 0;
  int16_t width = 0;
  int16_t height = 0;
  static const char* kwords[] = { "bpp", "width", "height", nullptr };

  if( !PyArg_ParseTupleAndKeywords( args, kw, "bhh", const_cast<char**>(kwords),
                                    &bpp, &width, &height ) )
    return nullptr;

  if( !vp::Bmp::Supported(bpp) )
  {
    PyErr_Format( PyExc_ValueError, 
                  "supported color depth: 1, 4, 8 and 24 (%d given)", bpp );
    return nullptr;
  }

  Value_CheckLower( 2, width, 1 )
  Value_CheckLower( 3, height, 1 )

  return new vp::Bmp(bpp, width, height);
}

//////////////////////
// bmp.Import( "filename.bmp" )
//////////////////////////////////////////////////////
PyObject* PyBmpImpl::Import( PyBmpObject* self, PyObject* arg )
{
  if( !PyString_CheckExact(arg) )
  {
    PyErr_Format( PyExc_TypeError, "requires string argument, not %s",
                  Py_TYPE(arg)->tp_name );
    return nullptr;    
  }

  char* FileName = PyString_AsString( arg );

  try
  {
    if( !self->pBmp->Import(FileName) )
    {
      PyErr_Format( PyExc_IOError, "failed to open '%s'", FileName );
      return nullptr;
    }
  }
  catch( const vp::Exception& e )
  {
    PyErr_Format( PyExc_IOError, "failed to read '%s' (%s)", FileName, e.what() );
    return nullptr;
  }

  Py_RETURN_NONE;
}

////////////////////////
// bmp.Export( "filename.bmp" [, True|False] )
//////////////////////////////////////////////////////
PyObject* PyBmpImpl::Export( PyBmpObject* self, PyObject* args )
{
  const char* FileName = nullptr;
  PyObject* pyBool = Py_False;
  if( !PyArg_ParseTuple(args, "s|O!", &FileName, &PyBool_Type, &pyBool) )
    return nullptr;

  bool OverWrite = PyObject_IsTrue( pyBool );
  if( !self->pBmp->Export(FileName, OverWrite) )
  {
    PyErr_Format( PyExc_IOError, "file '%s' already exists", FileName );
    return nullptr;
  }

  Py_RETURN_NONE;
}

/////////////////////
// bmp2 = bmp1.Clone()
//////////////////////////////////////////
PyObject* PyBmpImpl::Clone( PyBmpObject* self )
{
  PyTypeObject* type = self->ob_type;
  PyObject* other = type->tp_alloc( type, 0 );
  reinterpret_cast<PyBmpObject*>(other)->pBmp = new vp::Bmp( *(self->pBmp) );

  return other;
}

///////////////////
// bpp = bmp.BitsPerPixel()
////////////////////////////////////////////////
PyObject* PyBmpImpl::BitsPerPixel( PyBmpObject* self )
{
  return Py_BuildValue( "B", self->pBmp->BitsPerPixel() );
}

///////////////////
// bpp = bmp.Width()
/////////////////////////////////////////
PyObject* PyBmpImpl::Width( PyBmpObject* self )
{
  return Py_BuildValue( "H", self->pBmp->Width() );
}

///////////////////
// bpp = bmp.Height()
////////////////////////////////////////////
PyObject* PyBmpImpl::Height( PyBmpObject* self )
{
  return Py_BuildValue( "H", self->pBmp->Height() );
}

///////////////////
// w, h = bmp.Dimension()
//////////////////////////////////////////////
PyObject* PyBmpImpl::Dimension( PyBmpObject* self )
{
  return Py_BuildValue( "HH", self->pBmp->Width(), self->pBmp->Height() );
}

///////////////////
// size = bmp.ColorTableSize()
/////////////////////////////////////////////////////////////////
PyObject* PyBmpImpl::ColorTableSize( PyBmpObject* self )
{
  return Py_BuildValue( "H", self->pBmp->ColorTableSize() );
}

///////////////////
// bmp.SetColorTable( index, b, g, r )
/////////////////////////////////////////////////////////////
PyObject* PyBmpImpl::SetColorTable( PyBmpObject* self, PyObject* args )
{
  uint16_t Size = self->pBmp->ColorTableSize();
  if( Size == 0 )
  {
    PyErr_SetString( PyExc_Exception, "image has no color table");
    return nullptr;
  }

  uint8_t Index, Blue, Green, Red;
  if( !PyArg_ParseTuple( args, "bbbb", &Index, &Blue, &Green, &Red ) )
    return nullptr;

  Value_CheckUpper( 1, Index, Size )

  self->pBmp->SetColorTable(Index, Blue, Green, Red);

  Py_RETURN_NONE;
}

///////////////////
// b, g, r = bmp.GetColorTable( index )
///////////////////////////////////////////////////////////////
PyObject* PyBmpImpl::GetColorTable( PyBmpObject* self, PyObject* args )
{
  uint16_t Size = self->pBmp->ColorTableSize();
  if( Size == 0 )
  {
    PyErr_SetString( PyExc_Exception, "image has no color table");
    return nullptr;
  }

  uint8_t Index;
  if( !PyArg_ParseTuple( args, "b", &Index ) )
    return nullptr;

  Value_CheckUpper( 1, Index, Size )

  uint8_t Blue, Green, Red;
  self->pBmp->GetColorTable(Index, Blue, Green, Red);

  return Py_BuildValue( "BBB", Blue, Green, Red );
}

///////////////////
// bmp.SetAllPixels( colorIndex )
// bmp.SetAllPixels( b, g, r )
///////////////////////////////////////////////////////
PyObject* PyBmpImpl::SetAllPixels( PyBmpObject* self, PyObject* args )
{
  uint16_t Size = self->pBmp->ColorTableSize();
  if( Size != 0 )
  {
    uint8_t ColorIndex;
    if( !PyArg_ParseTuple( args, "b", &ColorIndex ) )
      return nullptr;

    Value_CheckUpper( 1, ColorIndex, Size )
    self->pBmp->SetAllPixels( ColorIndex );
  }
  else
  {
    uint8_t Blue, Green, Red;
    if( !PyArg_ParseTuple( args, "bbb", &Blue, &Green, &Red ) )
      return nullptr;

    self->pBmp->SetAllPixels( Blue, Green, Red );
  }

  Py_RETURN_NONE;
}

///////////////////
// bmp.SetPixel( x, y, colorIndex )
// bmp.SetPixel( x, y, b, g, r )
///////////////////////////////////////////////////////
PyObject* PyBmpImpl::SetPixel( PyBmpObject* self, PyObject* args )
{
  int16_t X, Y;
  uint16_t Size = self->pBmp->ColorTableSize();
  if( Size != 0 )
  {
    uint8_t ColorIndex;
    if( !PyArg_ParseTuple( args, "hhb", &X, &Y, &ColorIndex ) )
      return nullptr;

    Value_CheckRangeEx( 1, X, 0, self->pBmp->Width() )
    Value_CheckRangeEx( 2, Y, 0, self->pBmp->Height() )
    Value_CheckUpper( 3, ColorIndex, Size )

    self->pBmp->SetPixel( X, Y, ColorIndex );
  }
  else
  {
    uint8_t Blue, Green, Red;
    if( !PyArg_ParseTuple( args, "hhbbb", &X, &Y, &Blue, &Green, &Red ) )
      return nullptr;

    Value_CheckRangeEx( 1, X, 0, self->pBmp->Width() )
    Value_CheckRangeEx( 2, Y, 0, self->pBmp->Height() )

    self->pBmp->SetPixel( X, Y, Blue, Green, Red );
  }

  Py_RETURN_NONE;
}

///////////////////
// color_index = bmp.GetPixel( x, y )
// b, g, r = bmp.GetPixel( x, y )
/////////////////////////////////////////////////
PyObject* PyBmpImpl::GetPixel( PyBmpObject* self, PyObject* args )
{
  int16_t X, Y;
  if( !PyArg_ParseTuple( args, "hh", &X, &Y ) )
    return nullptr;

  Value_CheckRangeEx( 1, X, 0, self->pBmp->Width() )
  Value_CheckRangeEx( 2, Y, 0, self->pBmp->Height() )

  if( self->pBmp->ColorTableSize() != 0 )
  {
    return Py_BuildValue( "B", self->pBmp->GetPixel(X, Y) );
  }
  else
  {
    uint8_t Blue, Green, Red;
    self->pBmp->GetPixel( X, Y, Blue, Green, Red );

    return Py_BuildValue( "BBB", Blue, Green, Red );
  }
}
