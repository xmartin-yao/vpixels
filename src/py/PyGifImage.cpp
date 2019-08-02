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
#include "PyGif.h"
#include "PyUtil.h"
#include "Gif.h"
#include "GifImage.h"
#include "Exception.h"
#include "SimpleList.h"

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

////////////////////////////
namespace PyGifImageImpl
{
  // Python special methods
  PyObject* New( PyTypeObject* type, PyObject* args, PyObject* kw );
  int  Init( PyGifImageObject* self, PyObject* args, PyObject* kw );
  void Dealloc( PyGifImageObject* self );
  PyObject* Repr( PyGifImageObject* self );

  // methods GifImage_Type (exposed to Python)
  PyObject* Clone( PyGifImageObject* self, PyObject* arg );
  PyObject* BitsPerPixel( PyGifImageObject* self );
  PyObject* Left( PyGifImageObject* self );
  PyObject* Top( PyGifImageObject* self );
  PyObject* Width( PyGifImageObject* self );
  PyObject* Height( PyGifImageObject* self );
  PyObject* Dimension( PyGifImageObject* self );
  PyObject* Crop( PyGifImageObject* self, PyObject* args );
  PyObject* SetAllPixels( PyGifImageObject* self, PyObject* args );
  PyObject* SetPixel( PyGifImageObject* self, PyObject* args );
  PyObject* GetPixel( PyGifImageObject* self, PyObject* args );
  PyObject* Interlaced( PyGifImageObject* self );
  PyObject* Delay( PyGifImageObject* self, PyObject* args );
  PyObject* ColorTable( PyGifImageObject* self );
  PyObject* ColorTableSorted( PyGifImageObject* self );
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
    MDef( clone,            Clone,            METH_O,       "clone other image." )
    MDef( bitsperpixel,     BitsPerPixel,     METH_NOARGS,  "get color resolution." )
    MDef( bpp,              BitsPerPixel,     METH_NOARGS,  "get color resolution." )
    MDef( left,             Left,             METH_NOARGS,  "get left position of the image." )
    MDef( top,              Top,              METH_NOARGS,  "get top position of the image." )
    MDef( width,            Width,            METH_NOARGS,  "get image width." )
    MDef( height,           Height,           METH_NOARGS,  "get image height." )
    MDef( dimension,        Dimension,        METH_NOARGS,  "get width and height of the image." )
    MDef( crop,             Crop,             METH_VARARGS, "crop the image." )
    MDef( setallpixels,     SetAllPixels,     METH_VARARGS, "set all pixels to same color." )
    MDef( setall,           SetAllPixels,     METH_VARARGS, "set all pixels to same color." )
    MDef( setpixel,         SetPixel,         METH_VARARGS, "set the color of a pixel." )
    MDef( getpixel,         GetPixel,         METH_VARARGS, "get the color of a pixel." )
    MDef( interlaced,       Interlaced,       METH_NOARGS,  "return true if pixels are interlaced." )
    MDef( delay,            Delay,            METH_VARARGS, "set/get delay time for the image." )
    MDef( colortable,       ColorTable,       METH_NOARGS,  "return True if there is local color table." )
    MDef( colortablesorted, ColorTableSorted, METH_NOARGS,  "return True if color table sorted." )
    MDef( colortablesize,   ColorTableSize,   METH_VARARGS, "set/get color table size." )
    MDef( setcolortable,    SetColorTable,    METH_VARARGS, "set a color table entry." )
    MDef( setcolor,         SetColorTable,    METH_VARARGS, "set a color table entry." )
    MDef( getcolortable,    GetColorTable,    METH_VARARGS, "get a color table entry." )
    MDef( getcolor,         GetColorTable,    METH_VARARGS, "get a color table entry." )
    MDef( disposalmethod,   DisposalMethod,   METH_VARARGS, "set/get disposal method." )
    MDef( disposal,         DisposalMethod,   METH_VARARGS, "set/get disposal method." )
    MDef( hastransparentcolor, HasTransColor, METH_VARARGS, "query if image has transparent color, turn on/off transparent color." )
    MDef( hastranscolor,    HasTransColor,    METH_VARARGS, "query if image has transparent color, turn on/off transparent color." )
    MDef( transparentcolor, TransColor,       METH_VARARGS, "set/get transparent color." )
    MDef( transcolor,       TransColor,       METH_VARARGS, "set/get transparent color." )
    { nullptr, nullptr, 0, nullptr } 
  };
} //PyGifImageImpl

///////////////////
// GifImage_Type
/////////////////////////////////
PyTypeObject PyGifImage::GifImage_Type = {
  PyVarObject_HEAD_INIT( nullptr, 0 )
  "vp.gifimage",                  // tp_name
  sizeof(PyGifImageObject),       // tp_basicsize
  0,                              // tp_itemsize
  (destructor)PyGifImageImpl::Dealloc, // tp_dealloc
  0,                              // tp_print
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
  "vp.gifimage object",           // tp_doc
  0,                              // tp_traverse
  0,                              // tp_clear
  0,                              // tp_richcompare
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
};

///////////////////////////////////////////////////////////////
PyObject* PyGifImage::Cast2Py( vp::GifImage* pGifImage, PyGifObject* pGifObject )
{
  // create new PyGifImageObject
  PyTypeObject* type = &GifImage_Type;
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

  self->ob_type->tp_free(self);
}

///////////////////////////////////////
PyObject* PyGifImageImpl::Repr( PyGifImageObject* self )
{
  GifImage_Check( self )

  return PyString_FromFormat( "<%s object: bpp=%d (%d,%d) %dx%d colors=%d>",
                              self->ob_type->tp_name, self->pGifImage->BitsPerPixel(),
                              self->pGifImage->Left(), self->pGifImage->Top(),
                              self->pGifImage->Width(), self->pGifImage->Height(),
                              self->pGifImage->ColorTableSize() );
}

///////////////////
// img2.Clone( img1 )
////////////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Clone( PyGifImageObject* self, PyObject* arg )
{
  GifImage_Check( self )

  if( Py_TYPE(arg) != &PyGifImage::GifImage_Type )
  {
    PyErr_Format( PyExc_TypeError, "argument #1: %s object expected, got %s",
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
                  "argument #1: %s object belongs to an incompatible %s object",
                  PyGifImage::GifImage_Type.tp_name, PyGif::Gif_Type.tp_name );
    return nullptr;
  }

  Py_RETURN_NONE;
}

///////////////////
// bpp = img.BitsPerPixel()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::BitsPerPixel( PyGifImageObject* self )
{
  GifImage_Check( self )
  return Py_BuildValue( "B", self->pGifImage->BitsPerPixel() );
}

///////////////////
// l = img.Left()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Left( PyGifImageObject* self )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Left() );
}

///////////////////
// t = img.Top()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Top( PyGifImageObject* self )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Top() );
}

///////////////////
// w = img.Width()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Width( PyGifImageObject* self )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Width() );
}

///////////////////
// h = img.Height()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Height( PyGifImageObject* self )
{
  GifImage_Check( self )
  return Py_BuildValue( "H", self->pGifImage->Height() );
}

///////////////////
// w, h = img.Dimension()
///////////////////////////////////////////////////////
PyObject* PyGifImageImpl::Dimension( PyGifImageObject* self )
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

  int16_t LeftLower = self->pGifImage->Left();
  int16_t LeftUpper = LeftLower + self->pGifImage->Width();
  int16_t TopLower = self->pGifImage->Top();
  int16_t TopUpper = TopLower + self->pGifImage->Height();

  Value_CheckRangeEx( 1, Left, LeftLower, LeftUpper )
  Value_CheckRangeEx( 2, Top, TopLower, TopUpper )
  Value_CheckRange( 3, Width,  1, LeftUpper - Left )
  Value_CheckRange( 4, Height, 1, TopUpper - Top )

  self->pGifImage->Crop( Left, Top, Width, Height );

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

  Value_CheckUpper( 1, ColorIndex, Size )

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

  self->pGifImage->SetPixel( X, Y, ColorIndex );

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

  return Py_BuildValue( "B", self->pGifImage->GetPixel(X, Y) );
}

///////////////////
// ret_bool = img.Interlaced( x, y )
///////////////////////////////////////////////////
PyObject* PyGifImageImpl::Interlaced( PyGifImageObject* self )
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
  if( self->pGifImage->SingleImage() )
  {
    PyErr_SetString( PyExc_Exception, "cannot set/get delay time for single image" );
    return nullptr;
  }

  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "H", self->pGifImage->Delay() );
  }
  else
  {
    int16_t Centisecond;
    if( !PyArg_ParseTuple( args, "h", &Centisecond ) )
      return nullptr;

    if( Centisecond < 0 )
    {
      PyErr_SetString( PyExc_ValueError, "requires unsigned integer" );
      return nullptr;
    }

    self->pGifImage->Delay( Centisecond );

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
  if( self->pGifImage->SingleImage() )
  {
    PyErr_SetString( PyExc_Exception, "cannot set/get disposal method for single image" );
    return nullptr;
  }

  if( PyTuple_Size( args ) == 0 )
  {
    return Py_BuildValue( "B", self->pGifImage->DisposalMethod() );
  }
  else
  {
    uint8_t MethodID;
    if( !PyArg_ParseTuple( args, "b", &MethodID ) )
      return nullptr;

    Value_CheckRange( 1, MethodID, 0, 3 )

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
  if( self->pGifImage->SingleImage() )
  {
    PyErr_SetString( PyExc_Exception, "single image has no transparent color" );
    return nullptr;
  }

  if( PyTuple_Size( args ) == 0 )
  {
    if( self->pGifImage->HasTransColor() )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }
  else
  {
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
    PyErr_SetString( PyExc_Exception, "cannot set/get transparent color for single image" );
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
PyObject* PyGifImageImpl::ColorTable( PyGifImageObject* self )
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
PyObject* PyGifImageImpl::ColorTableSorted( PyGifImageObject* self )
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

    Value_CheckRange( 1, Size, 0, (1 << self->pGifImage->BitsPerPixel()) )

    self->pGifImage->ColorTableSize( Size );

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

  Value_CheckUpper( 1, Index, self->pGifImage->ColorTableSize() )

  uint8_t Red, Green, Blue;
  self->pGifImage->GetColorTable(Index, Red, Green, Blue);

  return Py_BuildValue( "BBB", Red, Green, Blue );
}
