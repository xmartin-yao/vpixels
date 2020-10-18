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

#include <lua.hpp>
#include "LuaGif.h"
#include "LuaGifDefs.h"
#include "LuaUtil.h"
#include "Gif.h"
#include "GifImage.h"
#include "Exception.h"
#include "SimpleList.h"

//////////////////////////////////
namespace LuaGifImpl
{
  // ID of LuaGif and its metatable
  const char ID[] = "vp.gif";

  // wrappers of copy ctor and member functions of vp::Gif
  int Clone( lua_State* L );
  int Import( lua_State* L );
  int Export( lua_State* L );
  int Version( lua_State* L );
  int BitsPerPixel( lua_State* L );
  int Width( lua_State* L );
  int Height( lua_State* L );
  int Dimension( lua_State* L );
  int ColorTable( lua_State* L );
  int ColorTableSorted( lua_State* L );
  int ColorTableSize( lua_State* L );
  int SetColorTable( lua_State* L );
  int GetColorTable( lua_State* L );
  int BackgroundColor( lua_State* L );
  int AspectRatio( lua_State* L );
  int Images( lua_State* L );
  int GetImage( lua_State* L );
  int RemoveImage( lua_State* L );
  int Size( lua_State* L );

  // meta methods
  int Indexing( lua_State* L );
  int ToString( lua_State* L );
  int Finalizer( lua_State* L );

  // utils
  int Cast2Lua( lua_State* L, vp::Gif* pGif );
  int GetField( lua_State* L );
  vp::Gif* NewGif( lua_State* L );
  vp::Gif* CheckGif( lua_State* L, int arg );
  void     CheckGif( lua_State* L, vp::Gif* pGif, int arg );
  LuaGifUD* CheckGifUD( lua_State* L, int arg );
  void Invalidate( SimpleList<LuaGifImageUD>* pListImageUD );

  // methods of LuaGif
  const luaL_Reg Methods[] = {
    { "clone",          Clone },
    { "import",         Import },
    { "export",         Export },
    { "version",        Version },
    { "bitsperpixel",   BitsPerPixel },
    { "bpp",            BitsPerPixel },
    { "width",          Width },
    { "height",         Height },
    { "dimension",      Dimension },
    { "colortable",     ColorTable },
    { "colorsorted",    ColorTableSorted },
    { "colortablesize", ColorTableSize },
    { "setcolortable",  SetColorTable },
    { "setcolor",       SetColorTable },
    { "getcolortable",  GetColorTable },
    { "getcolor",       GetColorTable },
    { "backgroundcolor",BackgroundColor },
    { "background",     BackgroundColor },
    { "aspectratio",    AspectRatio },
    { "images",         Images },
    { "getimage",       GetImage },
    { "removeimage",    RemoveImage },
    { "remove",         RemoveImage },
    { "size",           Size },
    { nullptr, nullptr }
  };
} //LuaGifImpl

////////////////////////
namespace
{
  // __tostring of LuaGif metatable
  int ToString( lua_State* L ) 
  {
    luaL_checktype( L, 1, LUA_TTABLE );
    lua_pushfstring( L, "metatable of %s", LuaGifImpl::ID );

    return 1;
  }
}

//////////////////////
// register metatable
////////////////////////////////////
void LuaGif::Register( lua_State* L )
{
  luaL_newmetatable( L, LuaGifImpl::ID ); 

  // meta fields
  lua_pushstring( L, "__index" );
  lua_pushcfunction( L, LuaGifImpl::Indexing );
  lua_settable( L, -3 );

  lua_pushstring( L, "__len" );
  lua_pushcfunction( L, LuaGifImpl::Images );
  lua_settable( L, -3 );

  lua_pushstring( L, "__tostring" );
  lua_pushcfunction( L, LuaGifImpl::ToString );
  lua_settable( L, -3 );

  lua_pushstring( L, "__gc");
  lua_pushcfunction( L, LuaGifImpl::Finalizer );
  lua_settable( L, -3 );

  // methods
  luaL_setfuncs( L, LuaGifImpl::Methods, 0 ); // 0 means no upvalue

  // add a metatable to the metatable
  lua_createtable( L, 0, 1 );
  lua_pushstring( L, "__tostring" );  // contains __tostring only
  lua_pushcfunction( L, ToString );
  lua_settable( L, -3 );
  lua_setmetatable( L, -2 );

  // pop the metatable
  lua_pop( L, 1 );
}

////////////////////
// gif = New()
// gif = New( bpp, width, height [, images, globalcolor] )
////////////////////////////////////////////////////////
int LuaGif::New( lua_State* L )
{
  vp::Gif* pGif = nullptr;
  if( lua_gettop( L ) != 0 )
    pGif = LuaGifImpl::NewGif( L );
  else // no argument
    pGif = new vp::Gif();

  return LuaGifImpl::Cast2Lua( L, pGif );
}

/////////////////
// New() called with arguments
///////////////////////////////////////
vp::Gif* LuaGifImpl::NewGif( lua_State* L )
{
  LuaUtil::CheckArgs( L, 3, 2 );

  auto bpp = LuaUtil::CheckUint8( L, 1 );
  LuaUtil::CheckValueRange( L, 1, bpp, 2, 8 );

  auto width = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueLower( L, 2, width, 1 );   // at least 1 pixel wide

  auto height = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueLower( L, 3, height, 1 );  // at least 1 pixel high

  auto images = LuaUtil::OptUint16( L, 4, 1 );
  LuaUtil::CheckValueLower( L, 4, images, 1 );  // at least 1 image

  bool colortable = LuaUtil::OptBoolean( L, 5, true );

  return new vp::Gif( bpp, width, height, images, colortable );
}

//////////
// gif2 = gif1:Clone()
//////////////////////////////////
int LuaGifImpl::Clone( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGifOld = CheckGif( L, 1 );
  vp::Gif* pGifNew = new vp::Gif( *pGifOld );

  return Cast2Lua( L, pGifNew );
}

/////////////////
// gif:Import( "filename.gif" )
//////////////////////////////////
int LuaGifImpl::Import( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2 );

  LuaGifUD* pGifUD = CheckGifUD( L, 1 );
  CheckGif( L, pGifUD->pGif,  1 );

  const char* FileName = luaL_checkstring( L, 2 );

  bool Error = false;
  try
  {
    // file does not exist
    if( !pGifUD->pGif->Import( FileName ) )
      return luaL_error( L, "failed to open '%s'", FileName );
  }
  catch( const vp::Exception& e )
  {
    // error occurred during importing
    Error = true;

    // No exception safety
    // vp::Gif object is not in valid state, results of calling methods of
    // LuaGif object are undefined. However no memory leak is guaranteed.

    // set error message
    luaL_where( L, 1 );
    lua_pushfstring( L, "failed to import '%s' (%s)", FileName, e.what() );
    lua_concat( L, 2 );
  }

  // reset the list, no matter whether importing failed or not
  Invalidate( pGifUD->pListImageUD );
  pGifUD->pListImageUD->Clear();

  if( Error )
    return lua_error( L );  // exception caught
  else
    return 0;  // file successfully imported
}

///////////////////
// gif:Export( "filename.gif" [, true|false] )
//////////////////////////////////
int LuaGifImpl::Export( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  const char* FileName = luaL_checkstring( L, 2 );
  bool OverWrite = LuaUtil::OptBoolean( L, 3, false );

  if( !pGif->Export(FileName, OverWrite) )
    luaL_error( L, "file '%s' already exists", FileName );

  return 0;
}

////////////
// v = gif:Version()
//////////////////////////////////
int LuaGifImpl::Version( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushstring( L, pGif->Version().c_str() );

  return 1;
}

//////////
// bpp = gif:BitsPerPixel()
// gif:BitsPerPixel( bpp )
//////////////////////////////////
int LuaGifImpl::BitsPerPixel( lua_State* L ) 
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );
  vp::Gif* pGif = CheckGif( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pGif->BitsPerPixel() );

    return 1;
  }
  else
  {
    auto bpp = LuaUtil::CheckUint8( L, 2 );
    LuaUtil::CheckValueRange( L, 2, bpp, 2, 8 );
    pGif->BitsPerPixel( bpp );

    return 0;
  }
}

/////////////////
// w = gif:Width()
//////////////////////////////////
int LuaGifImpl::Width( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushunsigned( L, pGif->Width() );

  return 1;
}

///////////////
// h = gif:Height()
//////////////////////////////////
int LuaGifImpl::Height( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushunsigned( L, pGif->Height() );

  return 1;
}

////////////////
// w, h = gif:Dimension()
//////////////////////////////////
int LuaGifImpl::Dimension( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushunsigned( L, pGif->Width() );
  lua_pushunsigned( L, pGif->Height() );

  return 2;
}

///////////////
// ret_bool = gif:ColorTable()
//////////////////////////////////////////
int LuaGifImpl::ColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushboolean( L, pGif->ColorTable() );

  return 1;
}

/////////////////
// ret_bool = gif:ColorTableSorted()
/////////////////////////////////////////
int LuaGifImpl::ColorTableSorted( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushboolean( L, pGif->ColorTableSorted() );

  return 1;
}

////////////////////////
// size = gif:ColorTableSize()
// gif:ColorTableSize( size )
//////////////////////////////////
int LuaGifImpl::ColorTableSize( lua_State* L ) 
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pGif->ColorTableSize() );

    return 1;
  }
  else
  {
    auto Size = LuaUtil::CheckUint16( L, 2 );
    LuaUtil::CheckValueRange( L, 2, Size, 0, 256 );

    pGif->ColorTableSize( Size );

    return 0;
  }
}

//////////////////
// gif:SetColorTable( index, r, g, b )
////////////////////////////////////////
int LuaGifImpl::SetColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 5 );

  vp::Gif* pGif = CheckGif( L, 1 );
  if( !pGif->ColorTable() )
    luaL_error( L, "no global color table" );

  auto Index = LuaUtil::CheckUint8( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, pGif->ColorTableSize() );

  auto Red   = LuaUtil::CheckUint8( L, 3 );
  auto Green = LuaUtil::CheckUint8( L, 4 );
  auto Blue  = LuaUtil::CheckUint8( L, 5 );

  pGif->SetColorTable( Index, Red, Green, Blue );

  return 0;
}

/////////////////////
// r, g, b = gif:GetColorTable( index )
////////////////////////////////////////
int LuaGifImpl::GetColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2 );

  vp::Gif* pGif = CheckGif( L, 1 );
  if( !pGif->ColorTable() )
    luaL_error( L, "no global color table" );

  auto Index = LuaUtil::CheckUint8( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, pGif->ColorTableSize() );
  uint8_t Red, Green, Blue;
  pGif->GetColorTable( Index, Red, Green, Blue );

  lua_pushunsigned( L, Red );
  lua_pushunsigned( L, Green );
  lua_pushunsigned( L, Blue );

  return 3;
}

/////////////////
// color_index = gif:BackgroundColor()
// gif:BackgroundColor(color_index)
///////////////////////////////////////////
int LuaGifImpl::BackgroundColor( lua_State* L ) 
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );

  auto pGif = CheckGif( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pGif->BackgroundColor() );

    return 1;
  }
  else
  {
    auto ColorIndex = LuaUtil::CheckUint8( L, 2 );
    if( !pGif->ColorTable() )
      luaL_error( L, "no global color table, cannot set background color" );

    LuaUtil::CheckValueUpper( L, 2, ColorIndex, pGif->ColorTableSize() );
    pGif->BackgroundColor( ColorIndex );

    return 0;
  }
}

/////////////////
// ratio = gif:AspectRatio()
//////////////////////////////////
int LuaGifImpl::AspectRatio( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushunsigned( L, pGif->AspectRatio() );

  return 1;
}

////////////
// n = gif:Images()
// n = #gif
//////////////////////////////////
int LuaGifImpl::Images( lua_State* L ) 
{
  // When being called by operator #, it actually gets two arguments
  // (both arguments are of LuaGif object). Lua Manual states that
  // only one argument will be passed in.
  LuaUtil::CheckArgs( L, 1, 1 ); // 1 optional arg, for operator #

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushunsigned( L, pGif->Images() );

  return 1;
}

//////////////
// image0 = gif:GetImage( 0 )
//////////////////////////////////
int LuaGifImpl::GetImage( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2 );

  LuaGifUD* pGifUD = CheckGifUD( L, 1 );
  vp::Gif* pGif = pGifUD->pGif;
  CheckGif( L, pGif, 1 );

  auto Images = static_cast<uint16_t>(pGif->Images());
  if( Images == 0 )
    luaL_error( L, "'%s' object has no image", ID );

  auto Index = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, Images );
  vp::GifImage& Image = (*pGif)[Index];

  return LuaGifImageImpl::Cast2Lua( L, &Image, pGifUD );
}

/////////////
// ret_bool = gif:RemoveImage( 0 )
//////////////////////////////////
int LuaGifImpl::RemoveImage( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );

  vp::Gif* pGif = CheckGif( L, 1 );

  auto Images = static_cast<uint16_t>(pGif->Images());
  if( Images == 0 )
    luaL_error( L, "'%s' object has no image", ID );

  if( Images == 1 )
    luaL_error( L, "'%s' object has only one image", ID );

  auto Index = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, Images );

  lua_pushboolean( L, pGif->Remove(Index) );

  return 1;
}

////////////
// size = gif:Size()
//////////////////////////////////
int LuaGifImpl::Size( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushunsigned( L, pGif->Size() );

  return 1;
}

///////////
// meta method __index
///////////////////////////////////
int LuaGifImpl::Indexing( lua_State* L )
{
  // if called with a number as argument
  if( lua_type( L, 2 ) == LUA_TNUMBER )
    return GetImage( L );  // return an image
  else
    return GetField( L );  // return a field in metatable
}

///////////////
// meta method __tostring
////////////////////////////////////
int LuaGifImpl::ToString( lua_State* L )
{
  vp::Gif* pGif = CheckGif( L, 1 );
  lua_pushfstring( L, "%s: %s bpp=%d %dx%d images=%d colors=%d", 
                   ID, pGif->Version().c_str(), pGif->BitsPerPixel(),
                   pGif->Width(), pGif->Height(),
                   pGif->Images(), pGif->ColorTableSize() );
  return 1;
}

///////////
// meta method __gc
////////////////////////////////////
int LuaGifImpl::Finalizer( lua_State* L )
{
  LuaGifUD* pGifUD = CheckGifUD( L, 1 );

  // delete the list
  if( pGifUD->pListImageUD != nullptr )
  {
    Invalidate( pGifUD->pListImageUD );

    delete pGifUD->pListImageUD;
    pGifUD->pListImageUD = nullptr;
  }

  // delete vp:Gif object
  if( pGifUD->pGif != nullptr )
  {
    delete pGifUD->pGif;
    pGifUD->pGif = nullptr;
  }

  return 0; 
}

///////////////
// cast vp::Gif object into a Lua object
/////////////////////////////////////////////
int LuaGifImpl::Cast2Lua( lua_State* L, vp::Gif* pGif )
{
  // create a userdatum (a Lua object)
  void* pUD = lua_newuserdata( L, sizeof(LuaGifUD) );

  // initialize the userdatum
  LuaGifUD* pGifUD = static_cast<LuaGifUD*>(pUD);
  pGifUD->pGif = pGif;
  pGifUD->pListImageUD = new SimpleList<LuaGifImageUD>();

  // set the metatable to userdatum
  luaL_getmetatable( L, ID );
  lua_setmetatable( L, -2 );

  return 1;
}

////////////////
// check if the argument (a Lua object) is LuaGifUD
////////////////////////////////////////////////////////
LuaGifUD* LuaGifImpl::CheckGifUD( lua_State* L, int arg )
{
  void* pUD = luaL_checkudata( L, arg, ID );

  // this should not happen, just in case
  LuaGifUD* pGifUD = static_cast<LuaGifUD*>(pUD);
  if( pGifUD == nullptr )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object", ID );
    luaL_argerror( L, arg, msg );
  }

  return pGifUD;
}

/////////////////////
// check if the argument(a Lua object) is LuaGifUD, return vp::Gif
///////////////////////////////////////////////////////////////////
vp::Gif* LuaGifImpl::CheckGif( lua_State* L, int arg )
{
  LuaGifUD* pGifUD = CheckGifUD( L, arg );

  // in case __gc() has been called directly, e.g. gif:__gc()
  if( pGifUD->pGif == nullptr )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object (pGif is null)", ID );
    luaL_argerror( L, arg, msg );
  }

  return pGifUD->pGif;
}

//////////////////////////////////
void LuaGifImpl::CheckGif( lua_State* L, vp::Gif* pGif, int arg )
{
  if( pGif == nullptr )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object (pGif is null)", ID );
    luaL_argerror( L, arg, msg );
  }
}

///////////////////////////////////
int LuaGifImpl::GetField( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );

  CheckGifUD( L, 1 );
  const char* Key = luaL_checkstring( L, 2 );
  if( luaL_getmetafield( L, 1, Key ) )
    return 1;
  else
    return luaL_error( L, "'%s' object has no field '%s'", ID, Key );
}

////////////////////////
// set LuaGifImage objects in the list to invalid state
//////////////////////////////////////////////////////////////
void LuaGifImpl::Invalidate( SimpleList<LuaGifImageUD>* pListImageUD )
{
  pListImageUD->Rewind();
  LuaGifImageUD* pGifImageUD = pListImageUD->Next();
  while( pGifImageUD != nullptr )
  {
    pGifImageUD->IsValid = false;
    pGifImageUD = pListImageUD->Next();
  }
}
