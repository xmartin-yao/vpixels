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
#include "LuaBmp.h"
#include "LuaUtil.h"
#include "Bmp.h"
#include "Exception.h"
#include <cstdint>

/////////////////////////
namespace LuaBmpImpl
{
  // ID of LuaBmp and metatable name
  const char ID[] = "vp.bmp";

  // wrappers of copy ctor and member functions of vp::Bmp
  int Clone( lua_State* L );
  int Import( lua_State* L );
  int Export( lua_State* L );
  int BitsPerPixel( lua_State* L );
  int Width( lua_State* L );
  int Height( lua_State* L );
  int Dimension( lua_State* L );
  int ColorTableSize( lua_State* L );
  int SetColorTable( lua_State* L );
  int GetColorTable( lua_State* L );
  int SetAllPixels( lua_State* L );
  int SetPixel( lua_State* L );
  int GetPixel( lua_State* L );

  // meta methods
  int ToString( lua_State* L );
  int Finalizer( lua_State* L );

  // utils
  int Cast2Lua( lua_State* L, vp::Bmp* pBmp );
  vp::Bmp* NewBmp( lua_State* L );
  vp::Bmp* CheckBmp( lua_State* L, int arg );
  uint8_t CheckBpp( lua_State* L, int arg );

  // methods of LuaBmp
  const luaL_Reg Methods[] = {
    { "clone",          Clone },
    { "import",         Import },
    { "export",         Export },
    { "bitsperpixel",   BitsPerPixel },
    { "bpp",            BitsPerPixel },
    { "width",          Width },
    { "height",         Height },
    { "dimension",      Dimension },
    { "colortablesize", ColorTableSize },
    { "setcolortable",  SetColorTable },
    { "setcolor",       SetColorTable },
    { "getcolortable",  GetColorTable },
    { "getcolor",       GetColorTable },
    { "setallpixels",   SetAllPixels },
    { "setall",         SetAllPixels },
    { "setpixel",       SetPixel },
    { "getpixel",       GetPixel },
    { nullptr, nullptr }
  };
} //LuaBmpImpl

///////////////////////
namespace
{
  // __tostring of LuaBmp metatable
  int ToString( lua_State* L ) 
  {
    luaL_checktype( L, 1, LUA_TTABLE );
    lua_pushfstring( L, "metatable of %s", LuaBmpImpl::ID );

    return 1;
  }
}

//////////////////////
// register metatable
////////////////////////////////////
void LuaBmp::Register( lua_State* L )
{
  luaL_newmetatable( L, LuaBmpImpl::ID ); 

  // meta fields
  lua_pushstring( L, "__index" ); // points to itself
  lua_pushvalue( L, -2 );
  lua_settable( L, -3 );

  lua_pushstring( L, "__tostring" );
  lua_pushcfunction( L, LuaBmpImpl::ToString );
  lua_settable( L, -3 );

  lua_pushstring( L, "__gc");
  lua_pushcfunction( L, LuaBmpImpl::Finalizer );
  lua_settable( L, -3 );

  // methods
  luaL_setfuncs( L, LuaBmpImpl::Methods, 0 );  // 0 means no upvalue

  // add a metatable to the metatable
  lua_createtable( L, 0, 1 );
  lua_pushstring( L, "__tostring" );  // contains __tostring only
  lua_pushcfunction( L, ToString );
  lua_settable( L, -3 );
  lua_setmetatable( L, -2 );

  // pop the metatable
  lua_pop( L, 1 );
}

////////////////////////
// bmp = New( bpp, width, height )
// bmp = New()
/////////////////////////////////////
int LuaBmp::New( lua_State* L )
{
  vp::Bmp* pBmp = nullptr;
  if( lua_gettop( L ) != 0 )
    pBmp = LuaBmpImpl::NewBmp( L );
  else // no argument
    pBmp = new vp::Bmp();

  return LuaBmpImpl::Cast2Lua( L, pBmp );
}

////////////////////
// New() called with arguments
//////////////////////////////////////////
vp::Bmp* LuaBmpImpl::NewBmp( lua_State* L )
{
  LuaUtil::CheckArgs( L, 3 );

  auto bpp = CheckBpp( L, 1 );

  auto width = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueLower( L, 2, width, 1 );  // at least 1 pixel wide

  auto height = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueLower( L, 3, height, 1 ); // at least 1 pixel high

  return new vp::Bmp( bpp, width, height );
}

///////////////
// bmp2 = bmp1:Clone()
////////////////////////////////
int LuaBmpImpl::Clone( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Bmp* pBmpOld = CheckBmp( L, 1 );
  vp::Bmp* pBmpNew = new vp::Bmp( *pBmpOld );

  return Cast2Lua( L, pBmpNew );
}

////////////////
// bmp:Import( "filename.bmp" )
/////////////////////////////////
int LuaBmpImpl::Import( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  const char* FileName = luaL_checkstring( L, 2 );

  bool Error = false;
  try
  {
    // file does not exist
    if( !pBmp->Import( FileName ) )
      return luaL_error( L, "failed to open '%s'", FileName );
  }
  catch( const vp::Exception& e )
  {
    // error occurred during importing
    Error = true;

    // No exception safety
    // vp::Bmp object is not in valid state, results of calling methods
    // of LuaBmp object are undefined. However no memory leak is guaranteed.

    // set error message, call lua_error() outside of catch-block;
    // otherwise, the vp::Exception object won't be deallocated,
    // b/c luaL_error() or lua_error() never returns.
    luaL_where( L, 1 );
    lua_pushfstring( L, "failed to import '%s' (%s)", FileName, e.what() );
    lua_concat( L, 2 );
  }

  if( Error )
    return lua_error( L );  // exception caught
  else
    return 0;  // file successfully imported
}

///////////////////////
// bmp:Export( "filename.bmp" [, true|false] )
///////////////////////////////////////////////
int LuaBmpImpl::Export( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2, 1 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  const char* FileName = luaL_checkstring( L, 2 );
  bool OverWrite = LuaUtil::OptBoolean( L, 3, false );

  if( !pBmp->Export(FileName, OverWrite) )
    luaL_error( L, "file '%s' already exists", FileName );

  return 0;
}

////////////
// bpp = bmp:BitsPerPixel()
///////////////////////////////
int LuaBmpImpl::BitsPerPixel( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  lua_pushunsigned( L, pBmp->BitsPerPixel() );

  return 1;
}

//////////////
// w = bmp:Width()
////////////////////////
int LuaBmpImpl::Width( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  lua_pushunsigned( L, pBmp->Width() );

  return 1;
}

/////////////
// h = bmp:Height()
////////////////////////////////
int LuaBmpImpl::Height( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  lua_pushunsigned( L, pBmp->Height() );

  return 1;
}

/////////////////
// w, h = bmp:Dimension()
//////////////////////////////////
int LuaBmpImpl::Dimension( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  lua_pushunsigned( L, pBmp->Width() );
  lua_pushunsigned( L, pBmp->Height() );

  return 2;
}

///////////////////
// size = bmp:ColorTableSize()
////////////////////////////////////
int LuaBmpImpl::ColorTableSize( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  lua_pushunsigned( L, pBmp->ColorTableSize() );

  return 1;
}

/////////////
// bmp:SetColorTable( index, b, g, r )
///////////////////////////////////////
int LuaBmpImpl::SetColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 5 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  auto Size = pBmp->ColorTableSize();
  if( Size == 0 )
    luaL_error( L, "image has no color table" );

  auto Index = LuaUtil::CheckUint8( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, Size );

  auto Blue  = LuaUtil::CheckUint8( L, 3 );
  auto Green = LuaUtil::CheckUint8( L, 4 );
  auto Red   = LuaUtil::CheckUint8( L, 5 );

  pBmp->SetColorTable( Index, Blue, Green, Red );

  return 0;
}

////////////////////////
// b, g, r = bmp:GetColorTable( index )
///////////////////////////////////////////
int LuaBmpImpl::GetColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );
  auto Size = pBmp->ColorTableSize();
  if( Size == 0 )
    luaL_error( L, "image has no color table" );

  auto Index = LuaUtil::CheckUint8( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, Size );

  uint8_t Blue, Green, Red;
  pBmp->GetColorTable( Index, Blue, Green, Red );

  lua_pushunsigned( L, Blue );
  lua_pushunsigned( L, Green );
  lua_pushunsigned( L, Red );

  return 3;
}

/////////////
// bmp:SetAllPixels( colorIndex )
// bmp:SetAllPixels( b, g, r )
//////////////////////////////////
int LuaBmpImpl::SetAllPixels( lua_State* L )
{
  vp::Bmp* pBmp = CheckBmp( L, 1 );
  auto Size = pBmp->ColorTableSize();
  if( Size != 0 )
  {
    LuaUtil::CheckArgs( L, 2 );

    auto ColorIndex = LuaUtil::CheckUint8( L, 2 );
    LuaUtil::CheckValueUpper( L, 2, ColorIndex, Size );

    pBmp->SetAllPixels( ColorIndex );
  }
  else
  {
    LuaUtil::CheckArgs( L, 4 );

    auto Blue  = LuaUtil::CheckUint8( L, 2 );
    auto Green = LuaUtil::CheckUint8( L, 3 );
    auto Red   = LuaUtil::CheckUint8( L, 4 );

    pBmp->SetAllPixels( Blue, Green, Red );
  }

  return 0;  
}

//////////////
// bmp:SetPixel( x, y, colorIndex )
// bmp:SetPixel( x, y, b, g, r )
/////////////////////////////////////
int LuaBmpImpl::SetPixel( lua_State* L )
{
  vp::Bmp* pBmp = CheckBmp( L, 1 );

  auto X = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, X, pBmp->Width() );

  auto Y = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueUpper( L, 3, Y, pBmp->Height() );

  auto Size = pBmp->ColorTableSize();
  if( Size != 0 )
  {
    LuaUtil::CheckArgs( L, 4 );

    auto ColorIndex = LuaUtil::CheckUint8( L, 4 );
    LuaUtil::CheckValueUpper( L, 4, ColorIndex, Size );

    pBmp->SetPixel( X, Y, ColorIndex );
  }
  else
  {
    LuaUtil::CheckArgs( L, 6 );

    auto Blue  = LuaUtil::CheckUint8( L, 4 );
    auto Green = LuaUtil::CheckUint8( L, 5 );
    auto Red   = LuaUtil::CheckUint8( L, 6 );

    pBmp->SetPixel( X, Y, Blue, Green, Red );
  }

  return 0;  
}

////////////////
// colorIndex = bmp:GetPixel( x, y )
// b, g, r = bmp:GetPixel( x, y )
/////////////////////////////////////
int LuaBmpImpl::GetPixel( lua_State* L )
{
  LuaUtil::CheckArgs( L, 3 );

  vp::Bmp* pBmp = CheckBmp( L, 1 );

  auto X = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, X, pBmp->Width() );

  auto Y = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueUpper( L, 3, Y, pBmp->Height() );

  if( pBmp->ColorTableSize() != 0 )
  {
    lua_pushunsigned( L, pBmp->GetPixel( X, Y ) );

    return 1;
  }
  else
  {
    uint8_t Blue, Green, Red;
    pBmp->GetPixel( X, Y, Blue, Green, Red );

    lua_pushunsigned( L, Blue );
    lua_pushunsigned( L, Green );
    lua_pushunsigned( L, Red );

    return 3;
  }
}

///////////////
// meta method __tostring
///////////////////////////
int LuaBmpImpl::ToString( lua_State* L )
{
  vp::Bmp* pBmp = CheckBmp( L, 1 );
  lua_pushfstring( L, "%s: bpp=%d %dx%d", ID, pBmp->BitsPerPixel(),
                   pBmp->Width(), pBmp->Height() );
  return 1;
}

//////////////
// meta method __gc
////////////////////////////////////
int LuaBmpImpl::Finalizer( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  void* pUD = luaL_checkudata( L, 1, ID );
  vp::Bmp** ppBmp = static_cast<vp::Bmp**>(pUD);
  if( *ppBmp != nullptr )
  {
    delete *ppBmp;
    *ppBmp = nullptr;
  }

  return 0; 
}

//////////////
// cast vp::Bmp object into Lua object
/////////////////////////////////////////////
int LuaBmpImpl::Cast2Lua( lua_State* L, vp::Bmp* pBmp )
{
  // create a userdatum (a pointer to vp::Bmp)
  void* pUD = lua_newuserdata( L, sizeof(vp::Bmp*) );

  // initialize the userdatum
  vp::Bmp** ppBmp = static_cast<vp::Bmp**>(pUD);
  *ppBmp = pBmp;   // store the address of vp::Bmp object

  // set the metatable to the userdatum
  luaL_getmetatable( L, ID );
  lua_setmetatable( L, -2 );

  return 1;
}

////////////////////
// check if the argument(a Lua object) is a LuaBmp object
////////////////////////////////////////////////////////
vp::Bmp* LuaBmpImpl::CheckBmp( lua_State* L, int arg )
{
  void* pUD = luaL_checkudata( L, arg, ID );
  vp::Bmp** ppBmp = static_cast<vp::Bmp**>(pUD);

  // in case __gc() has been called directly, e.g. bmp:__gc()
  if( *ppBmp == nullptr )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object (pBmp is null)", ID );
    luaL_argerror( L, arg, msg );
  }

  return *ppBmp;
}

/////////////////////////////////////////////////
uint8_t LuaBmpImpl::CheckBpp( lua_State* L, int arg )
{
  auto bpp = LuaUtil::CheckUint8( L, arg );
  if( !vp::Bmp::Supported(bpp) )
  {
    const char* msg = lua_pushfstring( L, "supported color depth: 1, 4, 8 and 24, got %d",
                                       bpp );
    luaL_argerror( L, arg, msg );
  }

  return bpp;
}
