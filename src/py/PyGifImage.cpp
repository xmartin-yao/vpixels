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
#include "PyGifImage.h"
#include "PyGifDefs.h"
#include "PyGif.h"
#include "PyUtil.h"
#include "Gif.h"
#include "GifImage.h"
#include "Exception.h"
#include "SimpleList.h"
#include "config.h"

/////////////////////
// check if image object is still good for use
///////////////////////////////////////////////
#define GifImage_Check( self )  \
  if( (self)->IsValid != true ) \
  { \
    PyErr_Format( PyExc_Exception, "invalid '%s' object, '%s' object already out of scope", \
                  PyGifImage::GifImage_Type.tp_name, PyGif::Gif_Type.tp_name ); \
    return nullptr; \
  }

////////
// docstrings
//////////////////////
PyDoc_STRVAR( GifImage_Type_doc,
"A " PACKAGE_NAME ".gifimage class represents an image (a.k.a frame) in a GIF image.\n\n\
" PACKAGE_NAME ".gifimage class cannot be directly instantiated. To access " PACKAGE_NAME ".gifImage\n\
objects contained in a " PACKAGE_NAME ".gif object, call getimage() of " PACKAGE_NAME ".gif object\n\
or use subscript operator of " PACKAGE_NAME ".gif object.\n\n\
Examples:\n\n\
   gif  = " PACKAGE_NAME ".gif(2, 3, 4, 5)  # create a " PACKAGE_NAME ".gif object\n\
   img0 = gif.getimage(0)          # get image 0\n\
   img1 = gif[1]                   # get image 1" );

PyDoc_STRVAR( clone_doc,
"clone(other)\n\n\
   other: a " PACKAGE_NAME ".gifimage object to be cloned\n\n\
Clone another " PACKAGE_NAME ".gifimage object, so this " PACKAGE_NAME ".gifimage object will be\n\
the same as the other one.");

PyDoc_STRVAR( bitsperpixel_doc,
"bitsperpixel(bpp)\n\n\
   bpp: color resolution in bits/pixel\n\n\
Change color resolution of " PACKAGE_NAME ".gifimage object. Support any value within\n\
range [2,8]. Size of local color table, if there is one, will be changed\n\
accordingly.\n\n\
bitsperpixel() -> int\n\n\
Return color resolution of " PACKAGE_NAME ".gifimage object." );

PyDoc_STRVAR( bpp_doc, "Alias of bitsperpixel(...)." );

PyDoc_STRVAR( left_doc,
"left() -> int\n\n\
Return x-coordinate of image's origin with respect to canvas.");

PyDoc_STRVAR( top_doc,
"top() -> int\n\n\
Return y-coordinate of image's origin with respect to canvas.");

PyDoc_STRVAR( width_doc,
"width() -> int\n\n\
Return image width in pixels." );

PyDoc_STRVAR( height_doc,
"height() -> int\n\n\
Return image height in pixels." );

PyDoc_STRVAR( dimension_doc,
"dimension() -> tuple\n\n\
Return dimension (width and height) of image in pixels." );

PyDoc_STRVAR( crop_doc,
"crop(left, top, width, height)\n\n\
   left:   x-coordinate of image's origin with respect to canvas\n\
   top:    y-coordinate of image's origin with respect to canvas\n\
   width:  width of image\n\
   height: height of image\n\n\
Crop " PACKAGE_NAME ".gifimage object with the specified parameters." );

PyDoc_STRVAR( setallpixels_doc,
"setallpixels(index)\n\n\
   index: index of an entry in color table entry\n\n\
Set all pixels to the same color represented by an index of an entry in local\n\
color table if there is one; or an entry in global color table, otherwise.\n\
Index is within range [0, size)." );

PyDoc_STRVAR( setall_doc, "Alias of setallpixels(...)." );

PyDoc_STRVAR( setpixel_doc,
"setpixel(x, y, index)\n\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\
   index: index of an entry in color table\n\n\
Set color of a pixel. i.e. an index of an entry in local color table\n\
if there is one; or an entry in global color table, otherwise. Index is\n\
within range [0, size)." );

PyDoc_STRVAR( getpixel_doc,
"getpixel(x, y) -> int\n\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\n\
Return color of a pixel, i.e. an index of an entry in local color table\n\
if there is one; or an entry in global color table, otherwise." );

PyDoc_STRVAR( transparent_doc,
"transparent(x, y) -> bool\n\
   x: x-coordinate of a pixel\n\
   y: y-coordinate of a pixel\n\n\
Return True if pixel is transparent, else False." );

PyDoc_STRVAR( trans_doc, "Alias of transparent(...)" );

PyDoc_STRVAR( interlaced_doc,
"interlaced() -> bool\n\n\
Return True if pixels are interlaced, else False.");

PyDoc_STRVAR( delay_doc,
"delay(time)\n\n\
   time: delay time in centi-second.\n\n\
Change delay time of " PACKAGE_NAME ".gifimage object.\n\n\
delay() -> int\n\n\
Return delay time of " PACKAGE_NAME ".gifimage object.");

PyDoc_STRVAR( colortable_doc,
"colortable() -> bool\n\n\
Return True if " PACKAGE_NAME ".gifimage object has a local color table, else False.");

PyDoc_STRVAR( colortablesorted_doc,
"colortablesorted() -> bool\n\n\
Return True if entries in local color table are sorted, else False.");

PyDoc_STRVAR( colortablesize_doc,
"colortablesize(size)\n\n\
   size: size of local color table to be changed to,\n\n\
Change size of local color table. Color resolution will be changed accordingly.\n\
Size can be any value in {0, 4, 8, 16, 32, 64, 128, 256}\n\n\
colortablesize() -> int\n\n\
Return size of local color table. 0 means no local color table." );

PyDoc_STRVAR( setcolortable_doc,
"setcolortable(index, red, green, blue)\n\n\
   index: index of an entry in local color table\n\
   red:   intensity of red channel\n\
   green: intensity of green channel\n\
   blue:  intensity of blue channel\n\n\
Set an entry in local color table if there is one. Index is within\n\
range [0, size) and intensities are within range [0, 255]" );

PyDoc_STRVAR( setcolor_doc, "Alias of setcolortable(...)." );

PyDoc_STRVAR( getcolortable_doc,
"getcolortable(index) -> tuple\n\n\
   index: index of an entry in local color table\n\n\
Return intensities of red, green, and blue channel of an entry in\n\
local color table if there is one. Index is within range [0, size)" );

PyDoc_STRVAR( getcolor_doc, "Alias of getcolortable(...)." );

PyDoc_STRVAR( disposalmethod_doc,
"disposalmethod(method)\n\n\
   method = 0: disposal method not specified\n\
            1: draw image on top of previous one\n\
            2: draw image after canvas is restored to background color\n\
            3: draw image after canvas is restored to previous state\n\
               before previous one was drawn\n\n\
Set disposal method.\n\n\
disposalmethod() -> int\n\n\
Return disposal method.");

PyDoc_STRVAR( disposal_doc, "Alias of disposalmethod(...)");

PyDoc_STRVAR( hastransparentcolor_doc,
"hastransparentcolor(switch)\n\n\
Turn on transparent color if switch == True, else turn off.\n\n\
hastransparentcolor() -> bool\n\n\
Return True if image has transparent color, else False.");

PyDoc_STRVAR( hastranscolor_doc, "Alias of hastransparentcolor(...)" );

PyDoc_STRVAR( transparentcolor_doc,
"transparentcolor(index)\n\n\
   index: index of an entry in color table\n\n\
Set transparent color represented by an index of an entry in local color table\n\
if there is one; or an entry in global color table, otherwise.\n\n\
transparentcolor() -> int\n\n\
Return transparent color represented by an index of an entry in local color table\n\
if there is one; or an entry in global color table, otherwise.");

PyDoc_STRVAR( transcolor_doc, "Alias of transparentcolor(...)" );


////////
// methods for GifImage_Type
////////////////////////////
namespace PyGifImageImpl
{
  // Python special methods
  PyObject* New( PyTypeObject* type, PyObject* args, PyObject* kw );
  int  Init( PyGifImageObject* self, PyObject* args, PyObject* kw );
  void Dealloc( PyGifImageObject* self );
  PyObject* Repr( PyGifImageObject* self );
  PyObject* RichCompare( PyObject* obj1, PyObject* obj2, int op );

  // methods GifImage_Type (exposed to Python)
  PyObject* Clone( PyGifImageObject* self, PyObject* arg );
  PyObject* BitsPerPixel( PyGifImageObject* self, PyObject* args );
  PyObject* Left( PyGifImageObject* self, PyObject* );
  PyObject* Top( PyGifImageObject* self, PyObject* );
  PyObject* Width( PyGifImageObject* self, PyObject* );
  PyObject* Height( PyGifImageObject* self, PyObject* );
  PyObject* Dimension( PyGifImageObject* self, PyObject* );
  PyObject* Crop( PyGifImageObject* self, PyObject* args );
  PyObject* SetAllPixels( PyGifImageObject* self, PyObject* args );
  PyObject* SetPixel( PyGifImageObject* self, PyObject* args );
  PyObject* GetPixel( PyGifImageObject* self, PyObject* args );
  PyObject* Transparent( PyGifImageObject* self, PyObject* args );
  PyObject* Interlaced( PyGifImageObject* self, PyObject* );
  PyObject* Delay( PyGifImageObject* self, PyObject* args );
  PyObject* ColorTable( PyGifImageObject* self, PyObject* );
  PyObject* ColorTableSorted( PyGifImageObject* self, PyObject* );
  PyObject* ColorTableSize( PyGifImageObject* self, PyObject* args );
  PyObject* SetColorTable( PyGifImageObject* self, PyObject* args );
  PyObject* GetColorTable( PyGifImageObject* self, PyObject* args );
  PyObject* DisposalMethod( PyGifImageObject* self, PyObject* args );
  PyObject* HasTransColor( PyGifImageObject* self, PyObject* args );
  PyObject* TransColor( PyGifImageObject* self, PyObject* args );

  // utils
  uint16_t CheckColorTable( PyGifImageObject* self );

  // type methods
  PyMethodDef Methods[] = {
    MDef( clone,            Clone,            METH_O,       clone_doc )
    MDef( bitsperpixel,     BitsPerPixel,     METH_VARARGS, bitsperpixel_doc )
    MDef( bpp,              BitsPerPixel,     METH_VARARGS, bpp_doc )
    MDef( left,             Left,             METH_NOARGS,  left_doc )
    MDef( top,              Top,              METH_NOARGS,  top_doc )
    MDef( width,            Width,            METH_NOARGS,  width_doc )
    MDef( height,           Height,           METH_NOARGS,  height_doc )
    MDef( dimension,        Dimension,        METH_NOARGS,  dimension_doc )
    MDef( crop,             Crop,             METH_VARARGS, crop_doc )
    MDef( setallpixels,     SetAllPixels,     METH_VARARGS, setallpixels_doc )
    MDef( setall,           SetAllPixels,     METH_VARARGS, setall_doc )
    MDef( setpixel,         SetPixel,         METH_VARARGS, setpixel_doc )
    MDef( getpixel,         GetPixel,         METH_VARARGS, getpixel_doc )
    MDef( transparent,      Transparent,      METH_VARARGS, transparent_doc )
    MDef( trans,            Transparent,      METH_VARARGS, trans_doc )
    MDef( interlaced,       Interlaced,       METH_NOARGS,  interlaced_doc )
    MDef( delay,            Delay,            METH_VARARGS, delay_doc )
    MDef( colortable,       ColorTable,       METH_NOARGS,  colortable_doc )
    MDef( colortablesorted, ColorTableSorted, METH_NOARGS,  colortablesorted_doc )
    MDef( colortablesize,   ColorTableSize,   METH_VARARGS, colortablesize_doc )
    MDef( setcolortable,    SetColorTable,    METH_VARARGS, setcolortable_doc )
    MDef( setcolor,         SetColorTable,    METH_VARARGS, setcolor_doc )
    MDef( getcolortable,    GetColorTable,    METH_VARARGS, getcolortable_doc )
    MDef( getcolor,         GetColorTable,    METH_VARARGS, getcolor_doc )
    MDef( disposalmethod,   DisposalMethod,   METH_VARARGS, disposalmethod_doc )
    MDef( disposal,         DisposalMethod,   METH_VARARGS, disposal_doc )
    MDef( hastransparentcolor, HasTransColor, METH_VARARGS, hastransparentcolor_doc )
    MDef( hastranscolor,    HasTransColor,    METH_VARARGS, hastranscolor_doc )
    MDef( transparentcolor, TransColor,       METH_VARARGS, transparentcolor_doc )
    MDef( transcolor,       TransColor,       METH_VARARGS, transcolor_doc )
    { nullptr, nullptr, 0, nullptr } 
  };

  constexpr char ID[] = {PACKAGE_NAME ".gifimage"};
} //PyGifImageImpl

///////////////////
// GifImage_Type
/////////////////////////////////
PyTypeObject PyGifImage::GifImage_Type = {
  PyVarObject_HEAD_INIT( nullptr, 0 )
  PyGifImageImpl::ID,             // tp_name
  sizeof(PyGifImageObject),       // tp_basicsize
  0,                              // tp_itemsize
  (destructor)PyGifImageImpl::Dealloc, // tp_dealloc
  0,                              // tp_print (2.x), tp_vectorcall_offset (3.8)
  0,                              // tp_getattr
  0,                              // tp_setattr
  0,                              // tp_compare
  (reprfunc)PyGifImageImpl::Repr, // tp_repr
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping
  0,                              // tp_hash
  0,                              // tp_call
  0,                              // tp_str
  0,                              // tp_getattro
  0,                              // tp_setattro
  0,                              // tp_as_buffer
  Py_TPFLAGS_DEFAULT,             // tp_flags
  GifImage_Type_doc,              // tp_doc
  0,                              // tp_traverse
  0,                              // tp_clear
  PyGifImageImpl::RichCompare,    // tp_richcompare
  0,                              // tp_weaklistoffset
  0,                              // tp_iter
  0,                              // tp_iternext
  PyGifImageImpl::Methods,        // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  0,                              // tp_base
  0,                              // tp_dict
  0,                              // tp_descr_get
  0,                              // tp_descr_set
  0,                              // tp_dictoffset
  (initproc)PyGifImageImpl::Init, // tp_init
  0,                              // tp_alloc
  PyGifImageImpl::New,            // tp_new
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

///////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Cast2Py( vp::GifImage* pGifImage, PyGifObject* pGifObject )
{
  // create new PyGifImageObject
  PyTypeObject* type = &PyGifImage::GifImage_Type;
  PyObject* self = type->tp_alloc( type, 0 );

  // initialize PyGifImageObject
  PyGifImageObject* pGifImageObject = reinterpret_cast<PyGifImageObject*>(self);
  pGifImageObject->IsValid = true;
  pGifImageObject->pGifImage = pGifImage;
  pGifImageObject->pGifObject = pGifObject;

  // add PyGifImageObject to list
  pGifObject->pGifImageObjectList->Add( pGifImageObject );

  return self;
}


//////////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::New(PyTypeObject*, PyObject*, PyObject* )
{
  PyErr_Format( PyExc_Exception, "cannot directly instantiate '%s'",
                PyGifImage::GifImage_Type.tp_name );
  return nullptr;
}

/////////////////////////////////////////////////////////
int PyGifImageImpl::Init( PyGifImageObject*, PyObject*, PyObject* )
{
  PyErr_Format( PyExc_Exception, "cannot directly instantiate '%s'",
                PyGifImage::GifImage_Type.tp_name );
  return -1;
}

///////////////////////////////////////////
void PyGifImageImpl::Dealloc( PyGifImageObject* self )
{
  // set itself to invalid and remove itself from the list
  if( self->IsValid )
  {
    self->IsValid = false;
    self->pGifObject->pGifImageObjectList->Remove( self );
  }

  // do not need to delete it, as it instantiated somewhere else
  if( self->pGifImage != nullptr )
    self->pGifImage = nullptr;

  Py_TYPE(self)->tp_free(self);
}

///////////////////////////////////////
PyObject* PyGifImageImpl::Repr( PyGifImageObject* self )
{
  GifImage_Check( self )

  return PyString_FromFormat( "<%s object: bpp=%d (%d,%d) %dx%d colors=%d>",
                              Py_TYPE(self)->tp_name, self->pGifImage->BitsPerPixel(),
                              self->pGifImage->Left(), self->pGifImage->Top(),
                              self->pGifImage->Width(), self->pGifImage->Height(),
                              self->pGifImage->ColorTableSize() );
}

/////////////////////////////
// operator: '<', '<=', '==', '!=', '>' or '>='
/////////////////////////////////////////////////////
PyObject* PyGifImageImpl::RichCompare( PyObject* obj1, PyObject* obj2, int op )
{
  // supported operator: '==' or '!='
  if( op != Py_EQ && op != Py_NE )
  {
    PyErr_Format( PyExc_TypeError, "unsupported comparison between '%s' and '%s'",
                  Py_TYPE(obj1)->tp_name, Py_TYPE(obj2)->tp_name );

    return nullptr;
  }

  // one object is not of GifImage_Type
  if( Py_TYPE(obj1) != &PyGifImage::GifImage_Type ||
      Py_TYPE(obj2) != &PyGifImage::GifImage_Type )
  {
    if( op == Py_EQ )
      Py_RETURN_FALSE;
    else
      Py_RETURN_TRUE;
  }

  // compare two objects of GifImage_Type
  PyGifImageObject* Img1 = reinterpret_cast<PyGifImageObject*>( obj1 );
  PyGifImageObject* Img2 = reinterpret_cast<PyGifImageObject*>( obj2 );
  GifImage_Check( Img1 )
  GifImage_Check( Img2 )
  if( *(Img1->pGifImage) == *(Img2->pGifImage) )
  {
    if( op == Py_EQ )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }
  else
  {
    if( op == Py_NE )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }
}

///////////////////
// img2.Clone( img1 )
////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Clone( PyGifImageObject* self, PyObject* arg )
{
  GifImage_Check( self )

  if( Py_TYPE(arg) != &PyGifImage::GifImage_Type )
  {
    PyErr_Format( PyExc_TypeError, "argument expected a %s object (got %s)",
                  PyGifImage::GifImage_Type.tp_name, Py_TYPE(arg)->tp_name );
    return nullptr;
  }

  PyGifImageObject* other = reinterpret_cast<PyGifImageObject*>( arg );
  GifImage_Check( other )

  try {
    *(self->pGifImage) = *(other->pGifImage);
  }
  catch( const vp::Exception& )
  {
    PyErr_Format( PyExc_Exception,
                  "argument %s object belongs to an incompatible %s object",
                  PyGifImage::GifImage_Type.tp_name, PyGif::Gif_Type.tp_name );
    return nullptr;
  }

  Py_RETURN_NONE;
}

///////////////////
// bpp = img.BitsPerPixel()
// img.BitsPerPixel( bpp )
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::BitsPerPixel( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )
  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "B", self->pGifImage->BitsPerPixel() );
  }
  else
  {
    uint8_t bpp;
    if( !PyArg_ParseTuple( args, "b", &bpp ) )
      return nullptr;

    if( self->pGifImage->ColorTable() )
    {
      Value_CheckRangeOne( bpp, 2, 8 )
    }
    else
    {
      Value_CheckRangeOne( bpp, 2, self->pGifObject->pGif->BitsPerPixel() )
    }

    self->pGifImage->BitsPerPixel( bpp );

    Py_RETURN_NONE;
  }
}

///////////////////
// l = img.Left()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Left( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Left() );
}

///////////////////
// t = img.Top()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Top( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Top() );
}

///////////////////
// w = img.Width()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Width( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Width() );
}

///////////////////
// h = img.Height()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Height( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Height() );
}

///////////////////
// w, h = img.Dimension()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Dimension( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  return Py_BuildValue( "HH", self->pGifImage->Width(), self->pGifImage->Height() );
}

///////////////////
// img.Crop( left, top, width, height )
////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Crop( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  int16_t Left, Top, Width, Height;
  if( !PyArg_ParseTuple( args, "hhhh", &Left, &Top, &Width, &Height ) )
    return nullptr;

  uint16_t LeftLower = self->pGifImage->Left();
  uint16_t LeftUpper = LeftLower + self->pGifImage->Width();
  uint16_t TopLower = self->pGifImage->Top();
  uint16_t TopUpper = TopLower + self->pGifImage->Height();

  Value_CheckRangeEx( 1, Left, LeftLower, LeftUpper )
  Value_CheckRangeEx( 2, Top, TopLower, TopUpper )
  Value_CheckRange( 3, Width,  1, LeftUpper - Left )
  Value_CheckRange( 4, Height, 1, TopUpper - Top )

  self->pGifImage->Crop( static_cast<uint16_t>(Left), static_cast<uint16_t>(Top),
                         static_cast<uint16_t>(Width), static_cast<uint16_t>(Height) );

  Py_RETURN_NONE;
}

////////////////////////////////////////////////////////
uint16_t PyGifImageImpl::CheckColorTable( PyGifImageObject* self )
{
  auto Size = self->pGifImage->CheckColorTable(); 
  if( Size == 0 )
    PyErr_SetString( PyExc_Exception, 
                     "there is neither global nor local color table");

  return Size;
}

///////////////////
// img.SetAllPixels( colorIndex )
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::SetAllPixels( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  auto Size = CheckColorTable( self );
  if( PyErr_Occurred() != nullptr )
    return nullptr;

  uint8_t ColorIndex;
  if( !PyArg_ParseTuple( args, "b", &ColorIndex ) )
    return nullptr;

  Value_CheckUpperOne( ColorIndex, Size )

  self->pGifImage->SetAllPixels( ColorIndex );

  Py_RETURN_NONE;
}

///////////////////
// img.SetPixel( x, y, colorIndex )
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::SetPixel( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  auto Size = CheckColorTable( self );
  if( PyErr_Occurred() != nullptr )
    return nullptr;

  int16_t X, Y;
  uint8_t ColorIndex;
  if( !PyArg_ParseTuple( args, "hhb", &X, &Y, &ColorIndex ) )
    return nullptr;

  Value_CheckRangeEx( 1, X, 0, self->pGifImage->Width() )
  Value_CheckRangeEx( 2, Y, 0, self->pGifImage->Height() )
  Value_CheckUpper( 3, ColorIndex, Size )

  self->pGifImage->SetPixel( static_cast<uint16_t>(X), static_cast<uint16_t>(Y), ColorIndex );

  Py_RETURN_NONE;
}

///////////////////
// colorIndex = img.GetPixel( x, y )
////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::GetPixel( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  int16_t X, Y;
  if( !PyArg_ParseTuple( args, "hh", &X, &Y ) )
    return nullptr;

  Value_CheckRangeEx( 1, X, 0, self->pGifImage->Width() )
  Value_CheckRangeEx( 2, Y, 0, self->pGifImage->Height() )

  return Py_BuildValue( "B", self->pGifImage->GetPixel(static_cast<uint16_t>(X), static_cast<uint16_t>(Y)) );
}

///////////////////
// ret_bool = img.Transparent( x, y )
////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Transparent( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  int16_t X, Y;
  if( !PyArg_ParseTuple( args, "hh", &X, &Y ) )
    return nullptr;

  Value_CheckRangeEx( 1, X, 0, self->pGifImage->Width() )
  Value_CheckRangeEx( 2, Y, 0, self->pGifImage->Height() )

  if( self->pGifImage->Transparent(static_cast<uint16_t>(X), static_cast<uint16_t>(Y)) )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// ret_bool = img.Interlaced()
///////////////////////////////////////////////////
PyObject* PyGifImageImpl::Interlaced( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  if( self->pGifImage->Interlaced() )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// centisecond = img.Delay()
// img.Delay( centisecond )
//////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Delay( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "H", self->pGifImage->Delay() );
  }
  else
  {
    if( self->pGifImage->SingleImage() )
    {
      PyErr_SetString( PyExc_Exception, "cannot set delay time for single image" );
      return nullptr;
    }

    int16_t Centisecond;
    if( !PyArg_ParseTuple( args, "h", &Centisecond ) )
      return nullptr;

    if( Centisecond < 0 )
    {
      PyErr_SetString( PyExc_ValueError, "requires unsigned integer" );
      return nullptr;
    }

    self->pGifImage->Delay( static_cast<uint16_t>(Centisecond) );

    Py_RETURN_NONE;
  }
}

///////////////////
// method_id = img.DisposalMethod()
// img.DisposalMethod( method_id )
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::DisposalMethod( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "B", self->pGifImage->DisposalMethod() );
  }
  else
  {
    if( self->pGifImage->SingleImage() )
    {
      PyErr_SetString( PyExc_Exception, "cannot set disposal method for single image" );
      return nullptr;
    }

    uint8_t MethodID;
    if( !PyArg_ParseTuple( args, "b", &MethodID ) )
      return nullptr;

    Value_CheckRangeOne( MethodID, 0, 3 )

    self->pGifImage->DisposalMethod( MethodID );

    Py_RETURN_NONE;
  }
}

///////////////////
// ret_bool = img.HasTransColor()
// img.HasTransColor( True|False )
/////////////////////////////////////////////////
PyObject* PyGifImageImpl::HasTransColor( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  if( PyTuple_Size( args ) == 0 )
  {
    if( self->pGifImage->HasTransColor() )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }
  else
  {
    if( self->pGifImage->SingleImage() )
    {
      PyErr_SetString( PyExc_Exception, "cannot turn on transparent color for single image" );
      return nullptr;
    }

    PyObject* pyBool = Py_False;
    if( !PyArg_ParseTuple(args, "O!", &PyBool_Type, &pyBool) )
      return nullptr;

    bool TrunOn = PyObject_IsTrue( pyBool );
    self->pGifImage->HasTransColor( TrunOn );

    Py_RETURN_NONE;
  }
}

///////////////////
// color_index = img.TransColor()
// img.TransColor( color_index )
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::TransColor( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )
  if( self->pGifImage->SingleImage() )
  {
    PyErr_SetString( PyExc_Exception, "single image has no transparent color" );
    return nullptr;
  }

  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "B", self->pGifImage->TransColor() );
  }
  else
  {
    uint8_t ColorIndex;
    if( !PyArg_ParseTuple( args, "b", &ColorIndex ) )
      return nullptr;

    auto Size = CheckColorTable( self );
    if( PyErr_Occurred() != nullptr )
      return nullptr;

    Value_CheckRangeEx( 1, ColorIndex, 0, Size )
    self->pGifImage->TransColor( ColorIndex );

    Py_RETURN_NONE;
  }
}

///////////////////
// ret_bool = img.ColorTable()
////////////////////////////////////////////
PyObject* PyGifImageImpl::ColorTable( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  if( self->pGifImage->ColorTable() )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// ret_bool = img.ColorTableSorted()
/////////////////////////////////////////////////
PyObject* PyGifImageImpl::ColorTableSorted( PyGifImageObject* self, PyObject* )
{
  GifImage_Check( self )
  if( self->pGifImage->ColorTableSorted() )
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

///////////////////
// size = img.ColorTableSize()
// img.ColorTableSize( size )
///////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::ColorTableSize( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "H", self->pGifImage->ColorTableSize() );
  }
  else
  {
    int16_t Size;
    if( !PyArg_ParseTuple( args, "h", &Size ) )
      return nullptr;

    Value_CheckRangeOne( Size, 0, 256 )

    self->pGifImage->ColorTableSize( static_cast<uint16_t>(Size) );

    Py_RETURN_NONE;
  }
}

///////////////////
// img.SetColorTable( index, r, g, b )
//////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::SetColorTable( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  if( !self->pGifImage->ColorTable() )
  {
    PyErr_SetString( PyExc_Exception, "no local color table");
    return nullptr;
  }

  uint8_t Index, Red, Green, Blue;
  if( !PyArg_ParseTuple( args, "bbbb", &Index, &Red, &Green, &Blue ) )
    return nullptr;

  Value_CheckUpper( 1, Index, self->pGifImage->ColorTableSize() )

  self->pGifImage->SetColorTable(Index, Red, Green, Blue);

  Py_RETURN_NONE;
}

///////////////////
// r, g, b = img.GetColorTable( index )
//////////////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::GetColorTable( PyGifImageObject* self, PyObject* args )
{
  GifImage_Check( self )

  if( !self->pGifImage->ColorTable() )
  {
    PyErr_SetString( PyExc_Exception, "no local color table");
    return nullptr;
  }

  uint8_t Index;
  if( !PyArg_ParseTuple( args, "b", &Index ) )
    return nullptr;

  Value_CheckUpperOne( Index, self->pGifImage->ColorTableSize() )

  uint8_t Red, Green, Blue;
  self->pGifImage->GetColorTable(Index, Red, Green, Blue);

  return Py_BuildValue( "BBB", Red, Green, Blue );
}
