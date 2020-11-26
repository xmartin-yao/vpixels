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
#include "PyGifDefs.h"
#include "PyUtil.h"
#include "Gif.h"
#include "Exception.h"
#include "SimpleList.h"

////////
// docstrings
//////////////////////
PyDoc_STRVAR( Gif_Type_doc,
"A vpixels.gif class represents a GIF image. A vpixels.gif object contains\n\
vpixels.gifimage objects that represent images (a.k.a. frames) in a GIF image.\n\
To instantiate a vpixels.gif object, call gif() of the module.\n\n\
vpixels.gif(bbp, width, height, images, colortable) --> vpixels.gif\n\n\
   bpp:    color resolution in bits/pixel, supported value within range [2,8]\n\
   width:  canvas width in pixels\n\
   height: canvas height in pixels\n\
   images: number of images, default == 1\n\
   colortable: if True, vpixels.gif object has a global color table,\n\
               default == True\n\n\
Examples:\n\n\
   vpixels.gif()\n\n\
Create a vpixels.gif object of color resolution 1 bit/pixel, width 1,\n\
and height 1, that contains one vpixels.gifimage object and has\n\
a global color table.\n\n\
   vpixels.gif(bpp=3, width=60, height=20)\n\n\
Create a vpixels.gif object of color resolution 3 bits/pixel, width 60,\n\
and height 20, that contains 1 vpixels.gifimage object and has\n\
a global color table.\n\n\
   vpixels.gif(4, 30, 20, 5, False)\n\n\
Create a vpixels.gif object of color resolution 4 bits/pixel, width 30,\n\
and height 20, that contains 5 vpixels.gifimage objects and does not have\n\
a global color table, while each of the 5 vpixels.gifimage objects has\n\
its own local color table." );

PyDoc_STRVAR( importf_doc,
"importf(name)\n\n\
   name: name of a GIF file to be imported in\n\n\
Import a GIF file into vpixels.gif object.");

PyDoc_STRVAR( export_doc,
"export(name, overwrite)\n\n\
   name: name of a GIF file to be exported to\n\
   overwrite: if True, overwrite existing file, default == False\n\n\
Export vpixels.gif object to a GIF file." );

PyDoc_STRVAR( clone_doc,
"clone() -> vpixels.gif\n\n\
Create a new vpixels.gif object that is the same as the current one." );

PyDoc_STRVAR( version_doc,
"version() -> str\n\n\
Return version of GIF specification the vpixels.gif object follows.");

PyDoc_STRVAR( bitsperpixel_doc,
"bitsperpixel(bpp)\n\n\
   bpp: color resolution in bits/pixel\n\n\
Change color resolution of vpixels.gif object. Support any value within\n\
range [2,8]. Size of global color table, if there is one, will be changed\n\
accordingly.\n\n\
bitsperpixel() -> int\n\n\
Return color resolution of vpixels.gif object." );

PyDoc_STRVAR( bpp_doc, "Alias of bitsperpixel(...)." );

PyDoc_STRVAR( width_doc,
"width() -> int\n\n\
Return canvas width in pixels." );

PyDoc_STRVAR( height_doc,
"height() -> int\n\n\
Return canvas height in pixels." );

PyDoc_STRVAR( dimension_doc,
"dimension() -> tuple\n\n\
Return dimension (width and height) of canvas in pixels." );

PyDoc_STRVAR( colortable_doc,
"colortable() -> bool\n\n\
Return True if vpixels.gif object has a global color table, else False." );

PyDoc_STRVAR( colortablesize_doc,
"colortablesize(size)\n\n\
   size: size of global color table to be changed to,\n\n\
Change size of global color table. Color resolution will be changed\n\
accordingly. Size can be any value in {0, 4, 8, 16, 32, 64, 128, 256}\n\n\
colortablesize() -> int\n\n\
Return size of global color table. 0 means no global color table." );

PyDoc_STRVAR( colortablesorted_doc,
"colortablesorted() -> bool\n\n\
return True if entries in global color table are sorted, else False." );

PyDoc_STRVAR( setcolortable_doc,
"setcolortable(index, red, green, blue)\n\n\
   index: index of an entry in global color table\n\
   red:   intensity of red channel\n\
   green: intensity of green channel\n\
   blue:  intensity of blue channel\n\n\
Set an entry in global color table. Index is within range [0, size) and\n\
intensities are within range [0, 255]" );

PyDoc_STRVAR( setcolor_doc, "Alias of setcolortable(...)" );

PyDoc_STRVAR( getcolortable_doc,
"getcolortable(index) -> tuple\n\n\
   index: index of an entry in global color table\n\n\
Return intensities of red, green, and blue channel of an entry in\n\
global color table. Index is within range [0, size)." );

PyDoc_STRVAR( getcolor_doc, "Alias of getcolortable(...)." );

PyDoc_STRVAR( backgroundcolor_doc,
"backgroundcolor(index)\n\n\
   index: index of an entry in global or local color table\n\n\
Set background color.\n\n\
backgroundcolor() -> int\n\n\
Return background color, an index of an entry in global or local color table.");

PyDoc_STRVAR( background_doc, "Alias of backgroundcolor(...)." );

PyDoc_STRVAR( aspectratio_doc,
"aspectratio() -> int\n\n\
Return aspect ratio." );

PyDoc_STRVAR( getimage_doc,
"getimage(index) -> vpixels.gifimage\n\n\
   index: index of a vpixels.gifimage object\n\n\
Return a vpixels.gifimage object contained in vpixels.gif project.\n\
Another way to access a vpixels.gifimage object is using subscript operator.\n\
Index is within range [0, images())." );

PyDoc_STRVAR( removeimage_doc,
"removeimage(index) -> bool\n\n\
   index: index of a vpixels.gifimage object\n\n\
Remove a vpixels.gifimage object contained in vpixels.gif object. Return False,\n\
if removing operation failed. Index is within range [0, images())" );

PyDoc_STRVAR( remove_doc, "Alias of removeimage(...)." );

PyDoc_STRVAR( images_doc,
"images() -> int\n\n\
Return the number of vpixels.gifimage objects contained in vpixels.gif object.\n\
Another way is calling the built-in function len() on vpixels.gif object." );

PyDoc_STRVAR( size_doc,
"size() -> int\n\n\
Return size of the resulting GIF file in bytes, if vpixels.gif object\n\
is exported to a GIF file." );


////////
// methods for Gif_Type
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
  PyObject* RemoveImage( PyGifObject* self, PyObject* arg );
  PyObject* Images( PyGifObject* self );
  PyObject* Size( PyGifObject* self );

  // utils
  vp::Gif* NewGif( PyObject* args, PyObject* kw );
  void Invalidate( SimpleList<PyGifImageObject>* pGifImageObjectList );

  // Gif_Type methods
  PyMethodDef Methods[] = {
    // cannot use 'import' as name, which causes SyntaxError
    MDef( importf,          Import,           METH_O,       importf_doc )
    MDef( export,           Export,           METH_VARARGS, export_doc )
    MDef( clone,            Clone,            METH_NOARGS,  clone_doc )
    MDef( version,          Version,          METH_NOARGS,  version_doc )
    MDef( bitsperpixel,     BitsPerPixel,     METH_VARARGS, bitsperpixel_doc )
    MDef( bpp,              BitsPerPixel,     METH_VARARGS, bpp_doc )
    MDef( width,            Width,            METH_NOARGS,  width_doc )
    MDef( height,           Height,           METH_NOARGS,  height_doc )
    MDef( dimension,        Dimension,        METH_NOARGS,  dimension_doc )
    MDef( colortable,       ColorTable,       METH_NOARGS,  colortable_doc )
    MDef( colortablesize,   ColorTableSize,   METH_VARARGS, colortablesize_doc )
    MDef( colortablesorted, ColorTableSorted, METH_NOARGS,  colortablesorted_doc )
    MDef( setcolortable,    SetColorTable,    METH_VARARGS, setcolortable_doc )
    MDef( setcolor,         SetColorTable,    METH_VARARGS, setcolor_doc )
    MDef( getcolortable,    GetColorTable,    METH_VARARGS, getcolortable_doc )
    MDef( getcolor,         GetColorTable,    METH_VARARGS, getcolor_doc )
    MDef( backgroundcolor,  BackgroundColor,  METH_VARARGS, backgroundcolor_doc )
    MDef( background,       BackgroundColor,  METH_VARARGS, background_doc )
    MDef( aspectratio,      AspectRatio,      METH_NOARGS,  aspectratio_doc )
    MDef( getimage,         GetImage,         METH_O,       getimage_doc )
    MDef( removeimage,      RemoveImage,      METH_O,       removeimage_doc )
    MDef( remove,           RemoveImage,      METH_O,       remove_doc )
    MDef( images,           Images,           METH_NOARGS,  images_doc )
    MDef( size,             Size,             METH_NOARGS,  size_doc )
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
  "vpixels.gif",                  // tp_name
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
  Gif_Type_doc,                   // tp_doc
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
#if PY_MAJOR_VERSION == 3
  ,0                              // tp_finalize
#endif
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

  return new vp::Gif( bpp, static_cast<uint16_t>(width), static_cast<uint16_t>(height), static_cast<size_t>(images), colortable );
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

  Py_TYPE(self)->tp_free( self );
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
                              Py_TYPE(self)->tp_name,
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

  const char* FileName = PyString_AsString( arg );

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
  PyTypeObject* type = Py_TYPE(self);
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

    Value_CheckRangeOne( bpp, 2, 8 )

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

    Value_CheckRangeOne( Size, 0, 256 )

    self->pGif->ColorTableSize( static_cast<uint16_t>(Size) );

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

  Value_CheckUpperOne( Index, self->pGif->ColorTableSize() )

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

    Value_CheckUpperOne( Index, self->pGif->ColorTableSize() )
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
  Value_CheckRangeExOne( Index, 0, static_cast<Py_ssize_t>(self->pGif->Images()) )

  vp::GifImage& Image = (*self->pGif)[static_cast<size_t>(Index)];

  return PyGifImageImpl::Cast2Py( &Image, self );
}

///////////////////
// ret_bool = gif.RemoveImage( 0 )
////////////////////////////////////////////////////////
PyObject* PyGifImpl::RemoveImage( PyGifObject* self, PyObject* arg )
{
  if( self->pGif->Images() == 0 )
  {
    PyErr_Format( PyExc_Exception, "'%s' object contains no image",
                  Py_TYPE(self)->tp_name );
    return nullptr;
  }

  if( self->pGif->Images() == 1 )
  {
    PyErr_Format( PyExc_Exception, "'%s' object contains only one image",
                  Py_TYPE(self)->tp_name );
    return nullptr;
  }

  if( !PyInt_CheckExact(arg) )
  {
    PyErr_Format( PyExc_TypeError, "requires an integer, got %s", Py_TYPE(arg)->tp_name );
    return nullptr;
  }

  Py_ssize_t Index = PyInt_AsSsize_t(arg);
  Value_CheckRangeExOne( Index, 0, static_cast<Py_ssize_t>(self->pGif->Images()) )

  if( self->pGif->Remove(static_cast<size_t>(Index)) )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// size = gif.Size()
//////////////////////////////////////////
PyObject* PyGifImpl::Size( PyGifObject* self )
{
  return Py_BuildValue( "I", self->pGif->Size() );
}
