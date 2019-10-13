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
#include "PyGif.h"
#include "PyGifImage.h"
#include "PyUtil.h"
#include "Gif.h"
#include "Exception.h"
#include "SimpleList.h"

//////////////////////
namespace PyGifImpl
{
  // Python special methods
  PyObject* New( PyTypeObject* type, PyObject* args, PyObject* kw );
  int  Init( PyGifObject* self, PyObject* args, PyObject* kw );
  void Dealloc( PyGifObject* self );
  Py_ssize_t Length( PyGifObject* self );
  PyObject* Repr( PyGifObject* self );

  // methods for Gif_Type (exposed to Python)
  PyObject* Import( PyGifObject* self, PyObject* arg );
  PyObject* Export( PyGifObject* self, PyObject* args );
  PyObject* Clone( PyGifObject* self );
  PyObject* Version( PyGifObject* self );
  PyObject* BitsPerPixel( PyGifObject* self, PyObject* args );
  PyObject* Width( PyGifObject* self );
  PyObject* Height( PyGifObject* self );
  PyObject* Dimension( PyGifObject* self );
  PyObject* ColorTable( PyGifObject* self );
  PyObject* ColorTableSorted( PyGifObject* self );
  PyObject* ColorTableSize( PyGifObject* self, PyObject* args );
  PyObject* SetColorTable( PyGifObject* self, PyObject* args );
  PyObject* GetColorTable( PyGifObject* self, PyObject* args );
  PyObject* BackgroundColor( PyGifObject* self, PyObject* args );
  PyObject* AspectRatio( PyGifObject* self );
  PyObject* GetImage( PyGifObject* self, PyObject* arg );
  PyObject* Images( PyGifObject* self );
  PyObject* Size( PyGifObject* self );

  // utils
  vp::Gif* NewGif( PyObject* args, PyObject* kw );
  void Invalidate( SimpleList<PyGifImageObject>* pGifImageObjectList );

  // Gif_Type methods
  PyMethodDef Methods[] = {
    // cannot use 'import' as name, which causes SyntaxError
    MDef( importf,          Import,           METH_O,       "import from GIF file." )
    MDef( export,           Export,           METH_VARARGS, "export to GIF file." )
    MDef( clone,            Clone,            METH_NOARGS,  "clone itself and create a new object." )
    MDef( version,          Version,          METH_NOARGS,  "get GIF version." )
    MDef( bitsperpixel,     BitsPerPixel,     METH_VARARGS, "get/set color resolution." )
    MDef( bpp,              BitsPerPixel,     METH_VARARGS, "get/set color resolution." )
    MDef( width,            Width,            METH_NOARGS,  "get width." )
    MDef( height,           Height,           METH_NOARGS,  "get height." )
    MDef( dimension,        Dimension,        METH_NOARGS,  "get width and height." )
    MDef( colortable,       ColorTable,       METH_NOARGS,  "return True if there is global color table." )
    MDef( colortablesize,   ColorTableSize,   METH_VARARGS, "get/set global color table size." )
    MDef( colortablesorted, ColorTableSorted, METH_NOARGS,  "return True if global color table is sorted" )
    MDef( setcolortable,    SetColorTable,    METH_VARARGS, "set a color table entry." )
    MDef( setcolor,         SetColorTable,    METH_VARARGS, "set a color table entry." )
    MDef( getcolortable,    GetColorTable,    METH_VARARGS, "get a color table entry." )
    MDef( getcolor,         GetColorTable,    METH_VARARGS, "get a color table entry." )
    MDef( backgroundcolor,  BackgroundColor,  METH_VARARGS, "get/set background color." )
    MDef( background,       BackgroundColor,  METH_VARARGS, "get/set background color." )
    MDef( aspectratio,      AspectRatio,      METH_NOARGS,  "get aspect ratio." )
    MDef( getimage,         GetImage,         METH_O,       "get an image object." )
    MDef( images,           Images,           METH_NOARGS,  "get the number of images." )
    MDef( size,             Size,             METH_NOARGS,  "get the size of resulting GIF file." )
    { nullptr, nullptr, 0, nullptr } 
  };

  // mapping methods
  PyMappingMethods Mapping = {
    (lenfunc)Length,      // mp_length
    (binaryfunc)GetImage, // mp_subscript
    0,                    // mp_ass_subscript
  };
} //PyGifImpl

//////////////////////////////
PyTypeObject PyGif::Gif_Type = {
  PyVarObject_HEAD_INIT( nullptr, 0 )
  "vp.gif",                       // tp_name
  sizeof(PyGifObject),            // tp_basicsize
  0,                              // tp_itemsize
  (destructor)PyGifImpl::Dealloc, // tp_dealloc
  0,                              // tp_print
  0,                              // tp_getattr
  0,                              // tp_setattr
  0,                              // tp_compare
  (reprfunc)PyGifImpl::Repr,      // tp_repr
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  &PyGifImpl::Mapping,            // tp_as_mapping
  0,                              // tp_hash
  0,                              // tp_call
  0,                              // tp_str
  0,                              // tp_getattro
  0,                              // tp_setattro
  0,                              // tp_as_buffer
  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, // tp_flags, allow subclass
  "vp.gif object",                // tp_doc
  0,                              // tp_traverse
  0,                              // tp_clear
  0,                              // tp_richcompare
  0,                              // tp_weaklistoffset
  0,                              // tp_iter
  0,                              // tp_iternext
  PyGifImpl::Methods,             // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  0,                              // tp_base
  0,                              // tp_dict
  0,                              // tp_descr_get
  0,                              // tp_descr_set
  0,                              // tp_dictoffset
  (initproc)PyGifImpl::Init,      // tp_init
  0,                              // tp_alloc
  PyGifImpl::New,                 // tp_new
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
PyObject* PyGifImpl::New( PyTypeObject* type, PyObject*, PyObject* )
{
  PyObject* self = type->tp_alloc( type, 0 );

  PyGifObject* pGifObject = reinterpret_cast<PyGifObject*>(self);
  pGifObject->pGif = nullptr;
  pGifObject->pGifImageObjectList = nullptr;

  return self;
}

////////////////////////////////////////////////
int PyGifImpl::Init( PyGifObject* self, PyObject* args, PyObject* kw )
{
  if( self->pGif != nullptr )
  {
    PyErr_SetString( PyExc_Exception, "__init__ already called" );
    return -1;
  }

  if( PyTuple_Size(args) == 0 && kw == nullptr )
    self->pGif = new vp::Gif();
  else
    self->pGif = NewGif( args, kw );

  if( self->pGif == nullptr )
    return -1;

  self->pGifImageObjectList = new SimpleList<PyGifImageObject>();

  return 0;
}

///////////////////
// NewGif( bpp, width, height [, images, globalcolor] )
///////////////////////////////////////////////////////////
vp::Gif* PyGifImpl::NewGif( PyObject* args, PyObject* kw )
{
  uint8_t bpp = 0;
  int16_t width = 0;
  int16_t height = 0;
  int32_t images = 1;
  PyObject* pyBool = Py_True;
  static const char* kwords[] = { "bpp", "width", "height", "images", "colortable", nullptr };

  if( !PyArg_ParseTupleAndKeywords( args, kw, "bhh|iO!", const_cast<char**>(kwords), &bpp,
                                    &width, &height, &images, &PyBool_Type, &pyBool ) )
    return nullptr;

  Value_CheckRange( 1, bpp, 2, 8 )
  Value_CheckLower( 2, width, 1 )
  Value_CheckLower( 3, height, 1 )
  Value_CheckLower( 4, images, 1 )
  bool colortable = PyObject_IsTrue( pyBool );

  return new vp::Gif( bpp, width, height, images, colortable );
}

///////////////////////////////////////
void PyGifImpl::Dealloc( PyGifObject* self )
{
  // delete list
  if( self->pGifImageObjectList != nullptr )
  {
    Invalidate( self->pGifImageObjectList );

    delete self->pGifImageObjectList;
    self->pGifImageObjectList = nullptr;
  }

  // delete vp::Gif object
  if( self->pGif != nullptr )
  {
    delete self->pGif;
    self->pGif = nullptr;
  }

  self->ob_type->tp_free( self );
}

///////////////////
// set each PyGifImageObject in the list to invalid state
////////////////////////////////////////////////////////////////
void PyGifImpl::Invalidate( SimpleList<PyGifImageObject>* pGifImageObjectList )
{
  pGifImageObjectList->Rewind();
  PyGifImageObject* pGifImageObject = pGifImageObjectList->Next();
  while( pGifImageObject != nullptr )
  {
    pGifImageObject->IsValid = false;
    pGifImageObject = pGifImageObjectList->Next();
  }
}

///////////////////////////////////////
PyObject* PyGifImpl::Repr( PyGifObject* self )
{
  return PyString_FromFormat( "<%s object: %s bpp=%d %dx%d images=%d colors=%d>",
                              self->ob_type->tp_name,
                              self->pGif->Version().c_str(),
                              self->pGif->BitsPerPixel(),
                              self->pGif->Width(), self->pGif->Height(),
                              self->pGif->Images(), self->pGif->ColorTableSize() );                            
}

///////////////////
// gif.Import( "filename.gif" )
//////////////////////////////////////////////////////
PyObject* PyGifImpl::Import( PyGifObject* self, PyObject* arg )
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
    if( !self->pGif->Import(FileName) )
    {
      // file does not exist
      PyErr_Format( PyExc_IOError, "failed to open '%s'", FileName );
      return nullptr;
    }
  }
  catch( const vp::Exception& e )
  {
    // No exception safety
    // vp::Gif object is not in valid state, results of calling methods of
    // PyGif object are undefined. However no memory leak is guaranteed.

    PyErr_Format( PyExc_Exception, "failed to import '%s' (%s)", FileName, e.what() );
  }

  // reset the list, no matter whether importing failed or not
  Invalidate( self->pGifImageObjectList );
  self->pGifImageObjectList->Clear();

  if( PyErr_Occurred() == nullptr )
    Py_RETURN_NONE;  // file successfully imported
  else
    return nullptr;  // exception caught
}

///////////////////
// gif.Export( "filename.gif" [, True|False] )
//////////////////////////////////////////////////////
PyObject* PyGifImpl::Export( PyGifObject* self, PyObject* args )
{
  const char* FileName = nullptr;
  PyObject* pyBool = Py_False;  // False by default
  if( !PyArg_ParseTuple(args, "s|O!", &FileName, &PyBool_Type, &pyBool) )
    return nullptr;

  bool OverWrite = PyObject_IsTrue( pyBool );
  if( !self->pGif->Export(FileName, OverWrite) )
  {
    PyErr_Format( PyExc_IOError, "file '%s' already exists", FileName );
    return nullptr;
  }

  Py_RETURN_NONE;
}

///////////////////
// gif2 = gif1.Clone()
//////////////////////////////////////////
PyObject* PyGifImpl::Clone( PyGifObject* self )
{
  // create a new PyGifObject
  PyTypeObject* type = self->ob_type;
  PyObject* other = type->tp_alloc( type, 0 );

  // initialize the PyGifObject
  PyGifObject* pGifObject = reinterpret_cast<PyGifObject*>(other);
  pGifObject->pGif = new vp::Gif( *(self->pGif) );
  pGifObject->pGifImageObjectList = new SimpleList<PyGifImageObject>();

  return other;
}

///////////////////
// v = gif.Version()
////////////////////////////////////////////////
PyObject* PyGifImpl::Version( PyGifObject* self )
{
  return Py_BuildValue( "s", self->pGif->Version().c_str() );
}

///////////////////////
// bpp = gif.BitsPerPixel()
// gif.BitsPerPixel( bpp )
////////////////////////////////////////////////
PyObject* PyGifImpl::BitsPerPixel( PyGifObject* self, PyObject* args )
{
  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "B", self->pGif->BitsPerPixel() );
  }
  else
  {
    uint8_t bpp;
    if( !PyArg_ParseTuple( args, "b", &bpp ) )
      return nullptr;

    Value_CheckRange( 1, bpp, 2, 8 )

    self->pGif->BitsPerPixel( bpp );

    Py_RETURN_NONE;
  }
}

///////////////////
// w = gif.Width()
/////////////////////////////////////////
PyObject* PyGifImpl::Width( PyGifObject* self )
{
  return Py_BuildValue( "H", self->pGif->Width() );
}

///////////////////
// h = gif.Height()
///////////////////////////////////////////
PyObject* PyGifImpl::Height( PyGifObject* self )
{
  return Py_BuildValue( "H", self->pGif->Height() );
}

///////////////////
// w, h = gif.Dimension()
//////////////////////////////////////////////
PyObject* PyGifImpl::Dimension( PyGifObject* self )
{
  return Py_BuildValue( "HH", self->pGif->Width(), self->pGif->Height() );
}

///////////////////
// ret_bool = gif.ColorTable()
////////////////////////////////////////////////
PyObject* PyGifImpl::ColorTable( PyGifObject* self )
{
  if( self->pGif->ColorTable() )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// ret_bool = gif.ColorTableSorted()
////////////////////////////////////////////////
PyObject* PyGifImpl::ColorTableSorted( PyGifObject* self )
{
  if( self->pGif->ColorTableSorted() )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// size = gif.ColorTableSize()
// gif.ColorTableSize( size )
/////////////////////////////////////////////////////////////////
PyObject* PyGifImpl::ColorTableSize( PyGifObject* self, PyObject* args )
{
  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "H", self->pGif->ColorTableSize() );
  }
  else
  {
    int16_t Size;
    if( !PyArg_ParseTuple( args, "h", &Size ) )
      return nullptr;

    Value_CheckRange( 1, Size, 0, 256 )

    self->pGif->ColorTableSize( Size );

    Py_RETURN_NONE;
  }
}

///////////////////
// gif.SetColorTable( index, r, g, b )
/////////////////////////////////////////////////////////////
PyObject* PyGifImpl::SetColorTable( PyGifObject* self, PyObject* args )
{
  if( !self->pGif->ColorTable() )
  {
    PyErr_SetString( PyExc_Exception, "no global color table");
    return nullptr;
  }

  uint8_t Index, Red, Green, Blue;
  if( !PyArg_ParseTuple( args, "bbbb", &Index, &Red, &Green, &Blue ) )
    return nullptr;

  Value_CheckUpper( 1, Index, self->pGif->ColorTableSize() )

  self->pGif->SetColorTable(Index, Red, Green, Blue);

  Py_RETURN_NONE;
}

///////////////////
// r, g, b = gif.GetColorTable( index )
///////////////////////////////////////////////////////////////
PyObject* PyGifImpl::GetColorTable( PyGifObject* self, PyObject* args )
{
  if( !self->pGif->ColorTable() )
  {
    PyErr_SetString( PyExc_Exception, "no global color table");
    return nullptr;
  }

  uint8_t Index;
  if( !PyArg_ParseTuple( args, "b", &Index ) )
    return nullptr;

  Value_CheckUpper( 1, Index, self->pGif->ColorTableSize() )

  uint8_t Red, Green, Blue;
  self->pGif->GetColorTable(Index, Red, Green, Blue);

  return Py_BuildValue( "BBB", Red, Green, Blue );
}

///////////////////
// color_index = gif.BackgroundColor()
// gif.BackgroundColor(color_index)
/////////////////////////////////////////
PyObject* PyGifImpl::BackgroundColor( PyGifObject* self, PyObject* args )
{
  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "B", self->pGif->BackgroundColor() );
  }
  else
  {
    uint8_t Index;
    if( !PyArg_ParseTuple( args, "b", &Index ) )
      return nullptr;

    if( !self->pGif->ColorTable() )
    {
      PyErr_SetString( PyExc_Exception, "no global color table, cannot set background color");
      return nullptr;
    }

    Value_CheckUpper( 1, Index, self->pGif->ColorTableSize() )
    self->pGif->BackgroundColor( Index );

    Py_RETURN_NONE;
  }
}

///////////////////
// ratio = gif.AspectRatio()
/////////////////////////////////////////
PyObject* PyGifImpl::AspectRatio( PyGifObject* self )
{
  return Py_BuildValue( "B", self->pGif->AspectRatio() );
}

///////////////////
// n = gif.Images()
//////////////////////////////////////////
PyObject* PyGifImpl::Images( PyGifObject* self )
{
  return Py_BuildValue( "I", self->pGif->Images() );
}

///////////////////
// n = len(gif)
///////////////////////////////////
Py_ssize_t PyGifImpl::Length( PyGifObject* self )
{
  return static_cast<Py_ssize_t>(self->pGif->Images());
}

///////////////////
// image0 = gif.GetImage( 0 )
////////////////////////////////////////////////////////
PyObject* PyGifImpl::GetImage( PyGifObject* self, PyObject* arg )
{
  if( self->pGif->Images() == 0 )
  {
    PyErr_Format( PyExc_Exception, "'%s' object contains no image",
                  Py_TYPE(self)->tp_name );
    return nullptr;
  }

  if( !PyInt_CheckExact(arg) )
  {
    PyErr_Format( PyExc_TypeError, "requires an integer, got %s",
                  Py_TYPE(arg)->tp_name );
    return nullptr;  
  }

  Py_ssize_t Index = PyInt_AsSsize_t(arg);
  Value_CheckRangeEx( 1, Index, 0, static_cast<Py_ssize_t>(self->pGif->Images()) )

  vp::GifImage& Image = (*self->pGif)[Index];

  return PyGifImage::Cast2Py( &Image, self );
}

///////////////////
// size = gif.Size()
//////////////////////////////////////////
PyObject* PyGifImpl::Size( PyGifObject* self )
{
  return Py_BuildValue( "I", self->pGif->Size() );
}
