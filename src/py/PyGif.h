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

#ifndef PyGif_h
#define PyGif_h

// forward
namespace vp { class Gif; }
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


////////////////////////////////
namespace PyGif
{
  // type object of PyGif
  extern PyTypeObject Gif_Type;
};

#endif //PyGif_h
