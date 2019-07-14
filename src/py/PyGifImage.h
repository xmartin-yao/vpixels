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

#ifndef PyGifImage_h
#define PyGifImage_h

// forward
namespace vp { class GifImage; }
struct PyGifObject;

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
namespace PyGifImage
{
  // type object of PyGifImage
  extern PyTypeObject GifImage_Type;

  // cast to PyGifImageObject
  PyObject* Cast2Py( vp::GifImage*, PyGifObject* );
};

#endif //PyGifImage_h
