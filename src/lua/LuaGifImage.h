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

#ifndef LuaGifImage_h
#define LuaGifImage_h

// forward
namespace vp { class GifImage; }
struct LuaGifUD;

////////////////////////////
// Userdatum for LuaGIfImage
//
// IsValid: status of LuaGIfImage
//   It is initialized to true when a LuaGIfImage userdatum is created
//   (see LuaGifImage::Cast2Lua()); set to false when either LuaGif or
//   LuaGIfImage goes out of scope (see LuaGif::Finalizer() and 
//   LuaGifImage::Finalizer()).
//
// pGifImage: pointer to vp::GifImage
//   Set to nullptr when LuaGIfImage goes out of scope.
//   Don't need to delete it, as vp::Gif will take care of it.
//
// pGifUD: pointer to LuaGif userdatum
//   LuaGif userdatum contains a list of pointers to LuaGIfImage objects.
//   A LuaGIfImage object adds itself to the list when it is created (see
//   LuaGifImage::Cast2Lua()), removes itself from the list when it goes
//   out of scope (LuaGifImage::Finalizer())
////////////////////////////////////////////////////////////////////////////
typedef struct LuaGifImageUD
{
  bool IsValid;
  vp::GifImage* pGifImage;
  LuaGifUD* pGifUD;
} LuaGifImageUD;

///////////////////////////////////
namespace LuaGifImage 
{
  // register metatable
  void Register( lua_State* L );

  // create LuaGifImage
  int Cast2Lua( lua_State* L, vp::GifImage* pGifImage, LuaGifUD* pGifUD );
}

#endif  //LuaGifImage_h
