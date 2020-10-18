////////////////////////////////////////////////////////////////////////
// Copyright (C) 2020 Xueyi Yao
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

#ifndef PyGifDefs_h
#define PyGifDefs_h

////////////////////////
// Declarations shared by PyGif.cpp and PyGIfImage.cpp
////////////////////////////////////////////////////////

// forward
namespace vp { class Gif; class GifImage; }
template<typename T> class SimpleList;
struct PyGifImageObject;

////////////////////////////
// Data for Gif_Type
//
// pGif: pointer to vp::Gif object
//
// pGifImageObjectList: list of PyGifImageObjects
//   This list is to track every PyGifImageObject created by
//   PyGifObject and notifies them when PyGifObject goes out of scope.
//   It stores pointers of every PyGifImageObject created (see 
//   PyGif::GetImage() and PyGifImage::Cast2Py()). When PyGifObject
//   goes out of scope, every PyGifImageObject is set to invalid
//   (see PyGif::Dealloc()).
//
// Both are instantiated when PyGifObject is created (see PyGif::Init()),
// need to be deleted when PyGifObject goes out of scope (see PyGif::Dealloc()).
//
//////////////////////////////////////////////////////////////////////////
// Instead of using a list to track every PyGifImageObject, an alternative
// is to use reference count. When a PyGifImageObject is created, increment
// the reference count for PyGifObject; when a PyGifImageObject goes out of
// scope, decrement the reference count for PyGifObject. In this way, even
// if PyGifObject is out of scope, it is still not garbage-collected, though
// not accessible, and every PyGifImageObject is still manipulable, though
// meaningless. This approach is simple and Pythonic, but the user will not
// get an immediate feedback of the fact that PyGifObject is already out of
// scope.
///////////////////////////////////////////////////////////////////////////
typedef struct PyGifObject {
  PyObject_HEAD
  vp::Gif* pGif;
  SimpleList<PyGifImageObject>* pGifImageObjectList;
} PyGifObject;


//////////////////////////
// Data for GifImage_Type
//
// IsValid: status of LuaGIfImage
//   It is initialized to true when a PyGifImageObject is created
//   (see PyGifImage::Cast2Py()); set to false when either LuaGif
//   or LuaGIfImage goes out of scope (see PyGifImage::Dealloc()
//   and PyGif::Dealloc()).
//
// pGifImage: pointer to vp::GifImage
//   Set to nullptr when LuaGIfImage goes out of scope.
//   Don't need to delete it, as vp::Gif will take care of it.
//
// pGifObject: pointer to PyGifObject
//   PyGifObject contains a list of pointers to PyGifImageObjects.
//   A PyGifImageObject adds itself to the list when it is created (see
//   PyGifImage::Cast2Py()), removes itself from the list when it goes
//   out of scope (PyGifImage::Dealloc())
//////////////////////////////////////////////////////////////////////////
typedef struct PyGifImageObject {
  PyObject_HEAD
  bool IsValid;
  vp::GifImage* pGifImage;
  PyGifObject*  pGifObject;
} PyGifImageObject;


//////////////////////////////////////
namespace PyGifImageImpl
{
  // cast to PyGifImageObject
  PyObject* Cast2Py( vp::GifImage*, PyGifObject* );
}

#endif //PyGifDefs_h
