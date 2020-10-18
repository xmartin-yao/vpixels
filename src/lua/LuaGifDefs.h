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

#ifndef LuaGifDefs_h
#define LuaGifDefs_h

////////////////////////
// Declarations shared by LuaGif.cpp and LuaGIfImage.cpp
//////////////////////////////////////////////////////////

// forward
namespace vp { class Gif; class GifImage; }
template<typename T> class SimpleList;
struct LuaGifImageUD;

/////////////////////////////
// Userdatum for LuaGif
//
// pGif: pointer to vp::Gif object
//
// pListImageUD: list of LuaGifImage userdatum
//   This list is to track every LuaGifImage userdatum created by
//   LuaGif and notifies them when LuaGif goes out of scope.
//   It stores pointers of every created LuaGifImage userdatum (see 
//   LuaGifImpl::GetImage() and LuaGifImageImpl::Cast2Lua()). When LuaGif goes
//   out of scope, every LuaGifImage is set to invalid (see 
//   LuaGif::Finalizer()).
//
// Both are instantiated when LuaGif is created (see LuaGifImpl::New() and 
// LuaGifImpl::Cast2Lua()), need to be deleted when LuaGif goes out of scope
// (see LuaGifImpl::Finalizer()).
//////////////////////////////////////////////////////////////////////////
typedef struct LuaGifUD
{
  vp::Gif* pGif;
  SimpleList<LuaGifImageUD>* pListImageUD;
} LuaGifUD;


////////////////////////////
// Userdatum for LuaGIfImage
//
// IsValid: status of LuaGIfImage
//   It is initialized to true when a LuaGIfImage userdatum is created
//   (see LuaGifImageImpl::Cast2Lua()); set to false when either LuaGif or
//   LuaGIfImage goes out of scope (see LuaGifImpl::Finalizer() and 
//   LuaGifImageImpl::Finalizer()).
//
// pGifImage: pointer to vp::GifImage
//   Set to nullptr when LuaGIfImage goes out of scope.
//   Don't need to delete it, as vp::Gif will take care of it.
//
// pGifUD: pointer to LuaGif userdatum
//   LuaGif userdatum contains a list of pointers to LuaGIfImage objects.
//   A LuaGIfImage object adds itself to the list when it is created (see
//   LuaGifImageImpl::Cast2Lua()), removes itself from the list when it goes
//   out of scope (LuaGifImageImpl::Finalizer())
////////////////////////////////////////////////////////////////////////////
typedef struct LuaGifImageUD
{
  bool IsValid;
  vp::GifImage* pGifImage;
  LuaGifUD* pGifUD;
} LuaGifImageUD;


////////////////////////////////
namespace LuaGifImpl
{
  // ID of LuaGif
  extern const char ID[];
}


////////////////////////////////
namespace LuaGifImageImpl
{
  // cast GifImage into LuaGifImage
  int Cast2Lua( lua_State* L, vp::GifImage* pGifImage, LuaGifUD* pGifUD );
}

#endif //LuaGifDefs_h
