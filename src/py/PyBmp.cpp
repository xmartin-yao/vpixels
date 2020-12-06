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
#include "config.h"

////////
// docstrings
//////////////////////
PyDoc_STRVAR( Bmp_Type_doc,
"A " PACKAGE_NAME ".bmp class represents a BMP image. To instantiate a " PACKAGE_NAME ".bmp\n\
object, call bmp() of the module.\n\n\
" PACKAGE_NAME ".bmp(bbp, width, height) -> " PACKAGE_NAME ".bmp\n\n\
   bpp:    color resolution, 1, 4, 8, and 24 bits/pixel supported\n\
   width:  image width in pixels\n\
   height: image height in pixels\n\n\
Examples:\n\n\
   " PACKAGE_NAME ".bmp()\n\n\
Create a " PACKAGE_NAME ".bmp object of color resolution 1 bit/pixel, width 1,\n\
and height 1.\n\n\
   " PACKAGE_NAME ".bmp(8, 30, 10)\n\n\
Create a " PACKAGE_NAME ".bmp object of color resolution 8 bits/pixel, width 30,\n\
and height 10.\n\n\
   " PACKAGE_NAME ".bmp(bpp=4, width=60, height=20)\n\n\
Create a " PACKAGE_NAME ".bmp object of color resolution 4 bits/pixel, width 60,\n\
and height 20.");

PyDoc_STRVAR( importf_doc,
"importf(name)\n\n\
   name: name of a BMP file to be imported in\n\n\
Import a BMP file into " PACKAGE_NAME ".bmp object.");

PyDoc_STRVAR( export_doc,
"export(name, overwrite)\n\n\
   name: name of a BMP file to be exported to\n\
   overwrite: if True, overwrite existing file, default == False\n\n\
Export " PACKAGE_NAME ".bmp object to a BMP file." );

PyDoc_STRVAR( clone_doc,
"clone() -> " PACKAGE_NAME ".bmp\n\n\
Create a new " PACKAGE_NAME ".bmp object that is the same as the current one." );

PyDoc_STRVAR( bitsperpixel_doc,
"bitsperpixel() -> int\n\n\
Return color resolution of " PACKAGE_NAME ".bmp object in bits/pixel." );

PyDoc_STRVAR( bpp_doc, "Alias of bitsperpixel(...)." );

PyDoc_STRVAR( width_doc,
"width() -> int\n\n\
Return image width in pixels." );

PyDoc_STRVAR( height_doc,
"height() -> int\n\n\
Return image height in pixels." );

PyDoc_STRVAR( dimension_doc,
"dimension() -> tuple\n\n\
Return image dimension (width and height) in pixels." );

PyDoc_STRVAR( colortablesize_doc,
"colortablesize() -> int\n\n\
Return size of color table. When color resolution is 24 bits/pixel,\n\
" PACKAGE_NAME ".bmp object has no color table and color table size equals to 0." );

PyDoc_STRVAR( setcolortable_doc,
"setcolortable(index, red, green, blue)\n\n\
   index: index of an entry in color table\n\
   red:   intensity of red channel\n\
   green: intensity of green channel\n\
   blue:  intensity of blue channel\n\n\
Set an entry in color table, when color resolution is 1, 4, or 8 bits/pixel.\n\
Index is within range [0, size) and intensities are within range [0, 255]." );

PyDoc_STRVAR( setcolor_doc, "Alias of setcolortable(...)." );

PyDoc_STRVAR( getcolortable_doc,
"getcolortable(index) -> tuple\n\n\
   index: index of an entry in color table\n\n\
Return intensities of red, green, and blue channel of an entry in\n\
color table, when color resolution is 1, 4, or 8 bits/pixel. Index is\n\
within range [0, size)." );

PyDoc_STRVAR( getcolor_doc, "Alias of getcolortable(...)." );

PyDoc_STRVAR( setallpixels_doc,
"setallpixels(index)\n\n\
   index: index of an entry in color table\n\n\
Set all pixels to the same color represented by an index of an entry in\n\
color table, when color resolution is 1, 4, or 8 bits/pixel. Index is within\n\
range [0, size).\n\n\
setallpixels(red, green, blue)\n\n\
   red:   intensity of red channel\n\
   green: intensity of green channel\n\
   blue:  intensity of blue channel\n\n\
Set all pixels to the same color, when color resolution is 24 bits/pixel.\n\
Intensities are within range [0, 255].");

PyDoc_STRVAR( setall_doc, "Alias of setallpixels(...)." );

PyDoc_STRVAR( setpixel_doc,
"setpixel(x, y, index)\n\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\
   index: index of an entry in color table\n\n\
Set color of a pixel, i.e. an index of an entry in color table, when color\n\
resolution is 1, 4, or 8 bits/pixel. Index is within range [0, size).\n\n\
setpixel(x, y, red, green, blue)\n\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\
   red:   intensity of red channel\n\
   green: intensity of green channel\n\
   blue:  intensity of blue channel\n\n\
Set color of a pixel, when color resolution is 24 bits/pixel. Intensities\n\
are within range [0, 255]." );

PyDoc_STRVAR( getpixel_doc,
"getpixel(x, y) -> int\n\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\n\
Return color of a pixel, i.e. an index of an entry in color table,\n\
when color resolution is 1, 4, or 8 bits/pixel.\n\n\
getpixel(x, y) -> tuple\n\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\n\
Return color of a pixel, i.e. intensities of red, green, and blue channel,\n\
when color resolution is 24 bits/pixel." );


/////////////////
// data for Bmp_Type
///////////////////////////////
typedef struct PyBmpObject
{
  PyObject_HEAD
  vp::Bmp* pBmp;
} PyBmpObject;

////////
// methods for Bmp_Type
/////////////////////////
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
  PyObject* Clone( PyBmpObject* self, PyObject* );
  PyObject* BitsPerPixel( PyBmpObject* self, PyObject* );
  PyObject* Width( PyBmpObject* self, PyObject* );
  PyObject* Height( PyBmpObject* self, PyObject* );
  PyObject* Dimension( PyBmpObject* self, PyObject* );
  PyObject* ColorTableSize( PyBmpObject* self, PyObject* );
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
    MDef( importf,        Import,         METH_O,       importf_doc )
    MDef( export,         Export,         METH_VARARGS, export_doc )
    MDef( clone,          Clone,          METH_NOARGS,  clone_doc )
    MDef( bitsperpixel,   BitsPerPixel,   METH_NOARGS,  bitsperpixel_doc )
    MDef( bpp,            BitsPerPixel,   METH_NOARGS,  bpp_doc )
    MDef( width,          Width,          METH_NOARGS,  width_doc )
    MDef( height,         Height,         METH_NOARGS,  height_doc )
    MDef( dimension,      Dimension,      METH_NOARGS,  dimension_doc )
    MDef( colortablesize, ColorTableSize, METH_NOARGS,  colortablesize_doc )
    MDef( setcolortable,  SetColorTable,  METH_VARARGS, setcolortable_doc )
    MDef( setcolor,       SetColorTable,  METH_VARARGS, setcolor_doc )
    MDef( getcolortable,  GetColorTable,  METH_VARARGS, getcolortable_doc )
    MDef( getcolor,       GetColorTable,  METH_VARARGS, getcolor_doc )
    MDef( setallpixels,   SetAllPixels,   METH_VARARGS, setallpixels_doc )
    MDef( setall,         SetAllPixels,   METH_VARARGS, setall_doc )
    MDef( setpixel,       SetPixel,       METH_VARARGS, setpixel_doc )
    MDef( getpixel,       GetPixel,       METH_VARARGS, getpixel_doc )
    { nullptr, nullptr, 0, nullptr } 
  };

  constexpr char ID[] = {PACKAGE_NAME ".bmp"};
} //PyBmpImpl

/////////////
// Bmp_Type
//////////////////////////////
PyTypeObject PyBmp::Bmp_Type = {
  PyVarObject_HEAD_INIT( nullptr, 0 )
  PyBmpImpl::ID,                  // tp_name
  sizeof(PyBmpObject),            // tp_basicsize
  0,                              // tp_itemsize
  (destructor)PyBmpImpl::Dealloc, // tp_dealloc
  0,                              // tp_print (2.x), tp_vectorcall_offset (3.8)
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
  Bmp_Type_doc,                   // tp_doc
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
#if PY_MAJOR_VERSION == 3
  ,0                              // tp_finalize
#if PY_MINOR_VERSION >= 8
  ,0                              // tp_vectorcall
#endif
#if PY_MINOR_VERSION == 8
  ,0                              // tp_print (3.8 only)
#endif
#endif
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

  Py_TYPE(self)->tp_free( self );
}

///////////////////////////////////////
PyObject* PyBmpImpl::Repr( PyBmpObject* self )
{
  return PyString_FromFormat( "<%s object: bpp=%d %dx%d>",
                              Py_TYPE(self)->tp_name, self->pBmp->BitsPerPixel(),
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

  const char* FileName = PyString_AsString( arg );

  try
  {
    if( !self->pBmp->Import(FileName) )
    {
      // file does not exist
      PyErr_Format( PyExc_IOError, "failed to open '%s'", FileName );
      return nullptr;
    }
  }
  catch( const vp::Exception& e )
  {
    // No exception safety
    // vp::Bmp object is not in valid state, results of calling methods of
    // PyBmp object are undefined. However no memory leak is guaranteed.

    PyErr_Format( PyExc_Exception, "failed to import '%s' (%s)", FileName, e.what() );
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
  PyObject* pyBool = Py_False;  // False by default
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
PyObject* PyBmpImpl::Clone( PyBmpObject* self, PyObject* )
{
  PyTypeObject* type = Py_TYPE(self);
  PyObject* other = type->tp_alloc( type, 0 );
  reinterpret_cast<PyBmpObject*>(other)->pBmp = new vp::Bmp( *(self->pBmp) );

  return other;
}

///////////////////
// bpp = bmp.BitsPerPixel()
////////////////////////////////////////////////
PyObject* PyBmpImpl::BitsPerPixel( PyBmpObject* self, PyObject* )
{
  return Py_BuildValue( "B", self->pBmp->BitsPerPixel() );
}

///////////////////
// bpp = bmp.Width()
/////////////////////////////////////////
PyObject* PyBmpImpl::Width( PyBmpObject* self, PyObject* )
{
  return Py_BuildValue( "H", self->pBmp->Width() );
}

///////////////////
// bpp = bmp.Height()
////////////////////////////////////////////
PyObject* PyBmpImpl::Height( PyBmpObject* self, PyObject* )
{
  return Py_BuildValue( "H", self->pBmp->Height() );
}

///////////////////
// w, h = bmp.Dimension()
//////////////////////////////////////////////
PyObject* PyBmpImpl::Dimension( PyBmpObject* self, PyObject* )
{
  return Py_BuildValue( "HH", self->pBmp->Width(), self->pBmp->Height() );
}

///////////////////
// size = bmp.ColorTableSize()
/////////////////////////////////////////////////////////////////
PyObject* PyBmpImpl::ColorTableSize( PyBmpObject* self, PyObject* )
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

  Value_CheckUpperOne( Index, Size )

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

    Value_CheckUpperOne( ColorIndex, Size )
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
