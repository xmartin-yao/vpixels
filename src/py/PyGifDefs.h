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

#include <cstdint>

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
//   PyGifImpl::GetImage() and PyGifImageImpl::Cast2Py()). When PyGifObject
//   goes out of scope, every PyGifImageObject is set to a status
//   that is not Valid.
//
// Both are instantiated when PyGifObject is created (see PyGifImpl::Init()),
// need to be deleted when PyGifObject goes out of scope (see PyGifImpl::Dealloc()).
//
// ForwardIter: forward iteration if true; reverse iteration, otherwise.
//
// IterIndex: index for iteration.
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

  // for iteration over images
  bool ForwardIter;
  Py_ssize_t IterIndex;
} PyGifObject;

//////////////////////////////
// Status of a PyGifImageObject
//   Normal:    normal status, set when it is created and kept during its life cycle.           
//   Abandoned: set when its corresponding vp:GifImage object is removed from
//              the vp::Gif object.
//   Orphaned:  set when the PyGifObject it belongs to is out of scope.
//   Invalid:   set when it is out of scope and subject to garbage collection.           
/////////////////////////////////////////////////////////////////
enum class Status : int8_t { Normal, Abandoned, Orphaned, Invalid };

//////////////////////////
// Data for GifImage_Type
//
// status: status of a PyGifImageObject
//
// pGifImage: pointer to vp::GifImage
//   Set to nullptr when PyGifImageObject goes out of scope.
//   Don't need to delete it, as vp::Gif will take care of it.
//
// pGifObject: pointer to PyGifObject
//////////////////////////////////////////////////////////////////////////
typedef struct PyGifImageObject {
  PyObject_HEAD
  Status status;
  vp::GifImage* pGifImage;
  PyGifObject*  pGifObject;
} PyGifImageObject;


//////////////////////////////////////
namespace PyGifImageImpl
{
  // cast to PyGifImageObject
  PyObject* Cast2Py( vp::GifImage*, PyGifObject* );

  // copy another PyGifImageObject
  PyObject* Clone( PyGifImageObject*, PyObject* );
}

#endif //PyGifDefs_h
