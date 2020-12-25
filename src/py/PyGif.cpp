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
#include "config.h"

////////
// docstrings
//////////////////////
PyDoc_STRVAR( Gif_Type_doc,
"A " PACKAGE_NAME ".gif class represents a GIF image. A " PACKAGE_NAME ".gif object contains\n\
" PACKAGE_NAME ".gifimage objects that represent images (a.k.a. frames) in a GIF image.\n\
To instantiate a " PACKAGE_NAME ".gif object, call gif() of the module.\n\n\
" PACKAGE_NAME ".gif(bbp, width, height, images, colortable) --> " PACKAGE_NAME ".gif\n\n\
   bpp:    color resolution in bits/pixel, supported value within range [2,8]\n\
   width:  canvas width in pixels\n\
   height: canvas height in pixels\n\
   images: number of images, default == 1\n\
   colortable: if True, " PACKAGE_NAME ".gif object has a global color table,\n\
               default == True\n\n\
Examples:\n\n\
   " PACKAGE_NAME ".gif()\n\n\
Create a " PACKAGE_NAME ".gif object of color resolution 1 bit/pixel, width 1,\n\
and height 1, that contains one " PACKAGE_NAME ".gifimage object and has\n\
a global color table.\n\n\
   " PACKAGE_NAME ".gif(bpp=3, width=60, height=20)\n\n\
Create a " PACKAGE_NAME ".gif object of color resolution 3 bits/pixel, width 60,\n\
and height 20, that contains 1 " PACKAGE_NAME ".gifimage object and has\n\
a global color table.\n\n\
   " PACKAGE_NAME ".gif(4, 30, 20, 5, False)\n\n\
Create a " PACKAGE_NAME ".gif object of color resolution 4 bits/pixel, width 30,\n\
and height 20, that contains 5 " PACKAGE_NAME ".gifimage objects and does not have\n\
a global color table, while each of the 5 " PACKAGE_NAME ".gifimage objects has\n\
its own local color table." );

PyDoc_STRVAR( importf_doc,
"importf(name)\n\n\
   name: name of a GIF file to be imported in\n\n\
Import a GIF file into " PACKAGE_NAME ".gif object.");

PyDoc_STRVAR( export_doc,
"export(name, overwrite)\n\n\
   name: name of a GIF file to be exported to\n\
   overwrite: if True, overwrite existing file, default == False\n\n\
Export " PACKAGE_NAME ".gif object to a GIF file." );

PyDoc_STRVAR( clone_doc,
"clone() -> " PACKAGE_NAME ".gif\n\n\
Create a new " PACKAGE_NAME ".gif object that is the same as the current one." );

PyDoc_STRVAR( version_doc,
"version() -> str\n\n\
Return version of GIF specification the " PACKAGE_NAME ".gif object follows.");

PyDoc_STRVAR( bitsperpixel_doc,
"bitsperpixel(bpp)\n\n\
   bpp: color resolution in bits/pixel\n\n\
Change color resolution of " PACKAGE_NAME ".gif object. Support any value within\n\
range [2,8]. Size of global color table, if there is one, will be changed\n\
accordingly.\n\n\
bitsperpixel() -> int\n\n\
Return color resolution of " PACKAGE_NAME ".gif object." );

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
Return True if " PACKAGE_NAME ".gif object has a global color table, else False." );

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
"getimage(index) -> " PACKAGE_NAME ".gifimage\n\n\
   index: index of a " PACKAGE_NAME ".gifimage object\n\n\
Return a " PACKAGE_NAME ".gifimage object contained in " PACKAGE_NAME ".gif project.\n\
Another way to access a " PACKAGE_NAME ".gifimage object is using subscript operator.\n\
Index is within range [0, images())." );

PyDoc_STRVAR( removeimage_doc,
"removeimage(index) -> bool\n\n\
   index: index of a " PACKAGE_NAME ".gifimage object\n\n\
Remove a " PACKAGE_NAME ".gifimage object contained in " PACKAGE_NAME ".gif object. Return False,\n\
if removing operation failed. Index is within range [0, images())" );

PyDoc_STRVAR( remove_doc, "Alias of removeimage(...)." );

PyDoc_STRVAR( images_doc,
"images() -> int\n\n\
Return the number of " PACKAGE_NAME ".gifimage objects contained in " PACKAGE_NAME ".gif object.\n\
Another way is calling the built-in function len() on " PACKAGE_NAME ".gif object." );

PyDoc_STRVAR( size_doc,
"size() -> int\n\n\
Return size of the resulting GIF file in bytes, if " PACKAGE_NAME ".gif object\n\
is exported to a GIF file." );

PyDoc_STRVAR( reversed_doc,
"__reversed__() -> " PACKAGE_NAME ".gif\n\n\
Return a reverse iterator which is " PACKAGE_NAME ".gif object itself." );


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
  int DelCopyImage( PyGifObject* self, PyObject* arg, PyObject* other );
  PyObject* Repr( PyGifObject* self );
  PyObject* Iter( PyGifObject* self );
  PyObject* IterNext( PyGifObject* self );
  PyObject* Reversed( PyGifObject* self, PyObject* );

  // methods for Gif_Type (exposed to Python)
  PyObject* Import( PyGifObject* self, PyObject* arg );
  PyObject* Export( PyGifObject* self, PyObject* args );
  PyObject* Clone( PyGifObject* self, PyObject* );
  PyObject* Version( PyGifObject* self, PyObject* );
  PyObject* BitsPerPixel( PyGifObject* self, PyObject* args );
  PyObject* Width( PyGifObject* self, PyObject* );
  PyObject* Height( PyGifObject* self, PyObject* );
  PyObject* Dimension( PyGifObject* self, PyObject* );
  PyObject* ColorTable( PyGifObject* self, PyObject* );
  PyObject* ColorTableSorted( PyGifObject* self, PyObject* );
  PyObject* ColorTableSize( PyGifObject* self, PyObject* args );
  PyObject* SetColorTable( PyGifObject* self, PyObject* args );
  PyObject* GetColorTable( PyGifObject* self, PyObject* args );
  PyObject* BackgroundColor( PyGifObject* self, PyObject* args );
  PyObject* AspectRatio( PyGifObject* self, PyObject* );
  PyObject* GetImage( PyGifObject* self, PyObject* arg );
  PyObject* RemoveImage( PyGifObject* self, PyObject* arg );
  PyObject* Images( PyGifObject* self, PyObject* );
  PyObject* Size( PyGifObject* self, PyObject* );

  // utils
  vp::Gif* NewGif( PyObject* args, PyObject* kw );
  void Invalidate( SimpleList<PyGifImageObject>* pGifImageObjectList );
  void RemoveFromList( SimpleList<PyGifImageObject>*, vp::GifImage* );
  PyObject* IterForward( PyGifObject* self );
  PyObject* IterBackward( PyGifObject* self );
  PyObject* FetchImage( PyGifObject* self, PyObject* arg );
  vp::GifImage* FetchImage( PyGifObject* self, PyObject* arg, Py_ssize_t& Index );
  int CopyImage( PyGifObject* self, PyObject* arg, PyObject* other );
  int DelImage( PyGifObject* self, PyObject* arg );

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
    MDef( __reversed__,     Reversed,         METH_NOARGS,  reversed_doc )
    { nullptr, nullptr, 0, nullptr } 
  };

  // mapping methods
  PyMappingMethods Mapping = {
    (lenfunc)Length,      // mp_length
    (binaryfunc)GetImage, // mp_subscript
    (objobjargproc)DelCopyImage, // mp_ass_subscript
  };

  constexpr char ID[] = {PACKAGE_NAME ".gif"};
} //PyGifImpl

//////////////////////////////
PyTypeObject PyGif::Gif_Type = {
  PyVarObject_HEAD_INIT( nullptr, 0 )
  PyGifImpl::ID,                  // tp_name
  sizeof(PyGifObject),            // tp_basicsize
  0,                              // tp_itemsize
  (destructor)PyGifImpl::Dealloc, // tp_dealloc
  0,                              // tp_print (2.x), tp_vectorcall_offset (3.8)
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
  (getiterfunc)PyGifImpl::Iter,   // tp_iter
  (iternextfunc)PyGifImpl::IterNext, // tp_iternext
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
  0,                              // tp_version_tag
#if PY_MAJOR_VERSION == 3
  0,                              // tp_finalize
#if PY_MINOR_VERSION >= 8
  0,                              // tp_vectorcall
#endif
#if PY_MINOR_VERSION == 8
  0,                              // tp_print (3.8 only)
#endif
#endif
};

////////////////////////////////////////////////
PyObject* PyGifImpl::New( PyTypeObject* type, PyObject*, PyObject* )
{
  PyObject* self = type->tp_alloc( type, 0 );

  if( self != nullptr )
  {
    PyGifObject* pGifObject = reinterpret_cast<PyGifObject*>(self);
    pGifObject->pGif = nullptr;
    pGifObject->pGifImageObjectList = nullptr;
  }

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

  // vp::Gif object
  if( PyTuple_Size(args) == 0 && kw == nullptr )
    self->pGif = new vp::Gif();
  else
    self->pGif = NewGif( args, kw );

  if( self->pGif == nullptr )
    return -1;

  // list of PyGifImageObjects
  self->pGifImageObjectList = new SimpleList<PyGifImageObject>();
  if( self->pGifImageObjectList == nullptr )
    return -1;

  // default values for iterator
  self->ForwardIter = true;
  self->IterIndex = 0;

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
    pGifImageObject->status = Status::Orphaned;
    pGifImageObject = pGifImageObjectList->Next();
  }
}

///////////////////////////////////////
PyObject* PyGifImpl::Repr( PyGifObject* self )
{
  return PyString_FromFormat( "<%s: %s bpp=%d %dx%d images=%d colors=%d>",
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
PyObject* PyGifImpl::Clone( PyGifObject* self, PyObject* )
{
  // create a new PyGifObject
  PyTypeObject* type = Py_TYPE(self);
  PyObject* other = type->tp_alloc( type, 0 );

  // initialize the PyGifObject
  if( other != nullptr )
  {
    PyGifObject* pGifObject = reinterpret_cast<PyGifObject*>(other);
    pGifObject->pGif = new vp::Gif( *(self->pGif) );
    pGifObject->pGifImageObjectList = new SimpleList<PyGifImageObject>();
  }

  return other;
}

///////////////////
// v = gif.Version()
////////////////////////////////////////////////
PyObject* PyGifImpl::Version( PyGifObject* self, PyObject* )
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
PyObject* PyGifImpl::Width( PyGifObject* self, PyObject* )
{
  return Py_BuildValue( "H", self->pGif->Width() );
}

///////////////////
// h = gif.Height()
///////////////////////////////////////////
PyObject* PyGifImpl::Height( PyGifObject* self, PyObject* )
{
  return Py_BuildValue( "H", self->pGif->Height() );
}

///////////////////
// w, h = gif.Dimension()
//////////////////////////////////////////////
PyObject* PyGifImpl::Dimension( PyGifObject* self, PyObject* )
{
  return Py_BuildValue( "HH", self->pGif->Width(), self->pGif->Height() );
}

///////////////////
// ret_bool = gif.ColorTable()
////////////////////////////////////////////////
PyObject* PyGifImpl::ColorTable( PyGifObject* self, PyObject* )
{
  if( self->pGif->ColorTable() )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// ret_bool = gif.ColorTableSorted()
////////////////////////////////////////////////
PyObject* PyGifImpl::ColorTableSorted( PyGifObject* self, PyObject* )
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
PyObject* PyGifImpl::AspectRatio( PyGifObject* self, PyObject* )
{
  return Py_BuildValue( "B", self->pGif->AspectRatio() );
}

///////////////////
// n = gif.Images()
//////////////////////////////////////////
PyObject* PyGifImpl::Images( PyGifObject* self, PyObject* )
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
// image = gif.getimage(index)
// __getitem__(), i.e. image = gif[index]
////////////////////////////////////////////////////////
PyObject* PyGifImpl::GetImage( PyGifObject* self, PyObject* arg )
{
  PyObject* pObject = FetchImage( self, arg );
  if( pObject != nullptr )
    self->pGifImageObjectList->Add( reinterpret_cast<PyGifImageObject*>(pObject) );

  return pObject;
}

///////////
// return a vp::GifImage object and its index in vp::Gif object
////////////////////////////////////////////////////////////////////
vp::GifImage* PyGifImpl::FetchImage( PyGifObject* self, PyObject* arg, Py_ssize_t& Index )
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

  Index = PyInt_AsSsize_t(arg);
  Value_CheckRangeExOne( Index, 0, static_cast<Py_ssize_t>(self->pGif->Images()) )

  return &((*self->pGif)[static_cast<size_t>(Index)]);
}

///////////
// return a PyGifImageObject
///////////////////////////////////////////////
PyObject* PyGifImpl::FetchImage( PyGifObject* self, PyObject* arg )
{
  Py_ssize_t Index = 0;
  vp::GifImage* pImage = FetchImage( self, arg, Index );
  if( pImage != nullptr )
    return PyGifImageImpl::Cast2Py( pImage, self );
  else
    return nullptr;
}

///////////////////
// ret_bool = gif.RemoveImage( 0 )
////////////////////////////////////////////////////////
PyObject* PyGifImpl::RemoveImage( PyGifObject* self, PyObject* arg )
{
  if( self->pGif->Images() == 1 )
  {
    PyErr_Format( PyExc_Exception, "'%s' object contains only one image",
                  Py_TYPE(self)->tp_name );
    return nullptr;
  }

  Py_ssize_t Index = 0;
  vp::GifImage* pImage = FetchImage( self, arg, Index );
  if( pImage == nullptr )
    return nullptr;

  // remove it from GifImageObjectList
  RemoveFromList( self->pGifImageObjectList, pImage );

  // remove it from vp::Gif object
  if( self->pGif->Remove(static_cast<size_t>(Index)) )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

////////////////////////////////////////////////////////////////
void PyGifImpl::RemoveFromList( SimpleList<PyGifImageObject>* pGifImageObjectList,
                                vp::GifImage* pGifImage )
{
  pGifImageObjectList->Rewind();
  PyGifImageObject* pGifImageObject = pGifImageObjectList->Next();
  while( pGifImageObject != nullptr )
  {
    // change its status and remove it from the list
    if( pGifImageObject->pGifImage == pGifImage )
    {
      pGifImageObject->status = Status::Abandoned;
      pGifImageObjectList->Remove( pGifImageObject );
      break;
    }

    pGifImageObject = pGifImageObjectList->Next();
  }
}

/////////////////
// Implement mp_ass_subscript, i.e. __delitem__() and __setitem__()
//   arg: index of the image, so a positive integer expected.
//        slice is not supported.
////////////////////////////////////////////////////////////////
int PyGifImpl::DelCopyImage( PyGifObject* self, PyObject* arg, PyObject* other )
{
  if( other == nullptr || other == Py_None )
    return DelImage( self, arg );
  else
    return CopyImage( self, arg, other );
}

/////////////////
// __delitem__(), i.e. del gif[index]
/////////////////////////////////////////////
int PyGifImpl::DelImage( PyGifObject* self, PyObject* index )
{
  return ( RemoveImage(self, index) == Py_True ) ? 0 : -1;
}

/////////////////
// __setitem__()
// i.e. gif[index] = img or gif[index1] = gif[index2]
// this is equivalent to clone() of PyGifImage object
/////////////////////////////////////////////////////////
int PyGifImpl::CopyImage( PyGifObject* self, PyObject* index, PyObject* other )
{
  PyObject* pObject = FetchImage( self, index );
  if( pObject == nullptr )
    return -1;

  // copy the other
  PyGifImageObject* pImageObject = reinterpret_cast<PyGifImageObject*>(pObject);
  PyObject* pRet = PyGifImageImpl::Clone( pImageObject, other );

  // here ends its duty
  Py_DECREF(pObject);

  return (pRet != nullptr) ? 0 : -1;
}

///////////////////
// size = gif.Size()
//////////////////////////////////////////
PyObject* PyGifImpl::Size( PyGifObject* self, PyObject* )
{
  return Py_BuildValue( "I", self->pGif->Size() );
}

///////////////////
// __iter__()
//
// Instead of implementing a iterator and a reverse iterator,
// add __iter__(), __next__(), and __reversed__() to PyGifObject,
// so PyGifObject itself is able to play both roles.
///////////////////////////////////////////////////////////////
PyObject* PyGifImpl::Iter( PyGifObject* self )
{
  // initializing iterator or reverse iterator
  if( self->ForwardIter )
    self->IterIndex = 0;
  else
    self->IterIndex = static_cast<Py_ssize_t>(self->pGif->Images()) - 1;

  Py_INCREF(self);
  return reinterpret_cast<PyObject*>(self);
}

///////////////////
// next() in Python 2, __next__() in Python 3
///////////////////////////////////////////////
PyObject* PyGifImpl::IterNext( PyGifObject* self )
{
  if( self->ForwardIter )
    return IterForward( self );
  else
    return IterBackward( self );
}

///////////////////
//  __reversed__()
//////////////////////////////////////////////////
PyObject* PyGifImpl::Reversed( PyGifObject* self, PyObject* )
{
  // set ForwardIter to false, so it becomes a reverse iterator
  self->ForwardIter = false;
  self->IterIndex = static_cast<Py_ssize_t>(self->pGif->Images()) - 1;

  Py_INCREF(self);
  return reinterpret_cast<PyObject*>(self);
}

////////////////
// iterating forward
//////////////////////////////////////////
PyObject* PyGifImpl::IterForward( PyGifObject* self )
{
  // reach the last image
  if( self->IterIndex >= static_cast<Py_ssize_t>(self->pGif->Images()) )
    return nullptr;

  vp::GifImage& Image = (*self->pGif)[static_cast<size_t>(self->IterIndex)];
  ++self->IterIndex;

  PyObject* pObject = PyGifImageImpl::Cast2Py( &Image, self );
  if( pObject != nullptr )
    self->pGifImageObjectList->Add( reinterpret_cast<PyGifImageObject*>(pObject) );

  return pObject;
}

////////////////
// iterating backward
//////////////////////////////////////////
PyObject* PyGifImpl::IterBackward( PyGifObject* self )
{
  // reach the 1st image
  if( self->IterIndex < 0 )
  {
    // set to true, so it's a forward iterator by default
    self->ForwardIter = true;
    return nullptr;
  }

  vp::GifImage& Image = (*self->pGif)[static_cast<size_t>(self->IterIndex)];
  --self->IterIndex;

  PyObject* pObject = PyGifImageImpl::Cast2Py( &Image, self );
  if( pObject != nullptr )
    self->pGifImageObjectList->Add( reinterpret_cast<PyGifImageObject*>(pObject) );

  return pObject;
}
