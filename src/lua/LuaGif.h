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

#ifndef LuaGif_h
#define LuaGif_h

// forward
namespace vp { class Gif; }
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
//   LuaGif::GetImage() and LuaGifImage::Cast2Lua()). When LuaGif goes
//   out of scope, every LuaGifImage is set to invalid (see 
//   LuaGif::Finalizer()).
//
// Both are instantiated when LuaGif is created (see LuaGif::New() and 
// LuaGif::Cast2Lua()), need to be deleted when LuaGif goes out of scope
// (see LuaGif::Finalizer()).
//////////////////////////////////////////////////////////////////////////
typedef struct LuaGifUD
{
  vp::Gif* pGif;
  SimpleList<LuaGifImageUD>* pListImageUD;
} LuaGifUD;

////////////////////////////////
namespace LuaGif
{
  // register metatable 
  void Register( lua_State* L );

  // create a LuaGif
  int New( lua_State* L );

  // ID of LuaGif
  extern const char ID[];
};

#endif //LuaGif_h
