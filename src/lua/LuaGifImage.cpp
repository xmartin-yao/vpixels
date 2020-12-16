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
#include "LuaGifImage.h"
#include "LuaGifDefs.h"
#include "LuaUtil.h"
#include "GifImage.h"
#include "Gif.h"
#include "Exception.h"
#include "SimpleList.h"
#include "config.h"

//////////////////////////////
namespace LuaGifImageImpl
{
  // ID of LuaGifImage and its metatable
  const char ID[] = {PACKAGE_NAME ".gifimage"};

  // wrappers of member functions of vp::GifImage
  int Clone( lua_State* L );
  int BitsPerPixel( lua_State* L );
  int Left( lua_State* L );
  int Top( lua_State* L );
  int Width( lua_State* L );
  int Height( lua_State* L );
  int Dimension( lua_State* L );
  int Crop( lua_State* L );
  int SetAllPixels( lua_State* L );
  int SetPixel( lua_State* L );
  int GetPixel( lua_State* L );
  int Transparent( lua_State* L );
  int Interlaced( lua_State* L );
  int Delay( lua_State* L );
  int ColorTable( lua_State* L );
  int ColorTableSorted( lua_State* L );
  int ColorTableSize( lua_State* L );
  int SetColorTable( lua_State* L );
  int GetColorTable( lua_State* L );
  int DisposalMethod( lua_State* L );
  int HasTransColor( lua_State* L );
  int TransColor( lua_State* L );

  // meta methods
  int Equal( lua_State* L );
  int LessThan( lua_State* L );
  int ToString( lua_State* L );
  int Finalizer( lua_State* L );

  // utils
  vp::GifImage* CheckGifImage( lua_State* L, int arg );
  LuaGifImageUD* CheckGifImageUD( lua_State* L, int arg );
  uint16_t CheckColorTable( lua_State* L, vp::GifImage* pGifImage );
  const char* TypeName( lua_State* L, int arg );

  // methods of LuaGifImage
  const luaL_Reg Methods[] = {
    { "clone",            Clone },
    { "bitsperpixel",     BitsPerPixel },
    { "bpp",              BitsPerPixel },
    { "left",             Left },
    { "top",              Top },
    { "width",            Width },
    { "height",           Height },
    { "dimension",        Dimension },
    { "crop",             Crop },
    { "setallpixels",     SetAllPixels },
    { "setall",           SetAllPixels },
    { "setpixel",         SetPixel },
    { "getpixel",         GetPixel },
    { "transparent",      Transparent },
    { "trans",            Transparent },
    { "interlaced",       Interlaced },
    { "delay",            Delay },
    { "colortable",       ColorTable },
    { "colortablesorted", ColorTableSorted },
    { "colortablesize",   ColorTableSize },
    { "setcolortable",    SetColorTable },
    { "setcolor",         SetColorTable },
    { "getcolortable",    GetColorTable },
    { "getcolor",         GetColorTable },
    { "disposalmethod",   DisposalMethod },
    { "disposal",         DisposalMethod },
    { "hastransparentcolor", HasTransColor },
    { "hastranscolor",    HasTransColor },
    { "transparentcolor", TransColor },
    { "transcolor",       TransColor },
    { nullptr, nullptr }
  };
} //LuaGifImageImpl

////////////////////////
namespace
{
  // __tostring of LuaGifImage metatable
  int ToString( lua_State* L ) 
  {
    luaL_checktype( L, 1, LUA_TTABLE );
    lua_pushfstring( L, "metatable of %s", LuaGifImageImpl::ID );

    return 1;
  }
}

////////////////
// register metatable
////////////////////////////////////////
void LuaGifImage::Register( lua_State* L )
{
  luaL_newmetatable( L, LuaGifImageImpl::ID ); 

  // meta fields
  lua_pushstring( L, "__index" );  // point to itself
  lua_pushvalue( L, -2 );
  lua_settable( L, -3 );

  lua_pushstring( L, "__eq" );
  lua_pushcfunction( L, LuaGifImageImpl::Equal );
  lua_settable( L, -3 );

  lua_pushstring( L, "__lt" );
  lua_pushcfunction( L, LuaGifImageImpl::LessThan );
  lua_settable( L, -3 );

  lua_pushstring( L, "__tostring" );
  lua_pushcfunction( L, LuaGifImageImpl::ToString );
  lua_settable( L, -3 );

  lua_pushstring( L, "__gc");
  lua_pushcfunction( L, LuaGifImageImpl::Finalizer );
  lua_settable( L, -3 );

  // methods
  luaL_setfuncs( L, LuaGifImageImpl::Methods, 0 ); // 0 means no upvalue

  // add a metatable to the metatable
  lua_createtable( L, 0, 1 );
  lua_pushstring( L, "__tostring" );  // contains __tostring only
  lua_pushcfunction( L, ToString );
  lua_settable( L, -3 );
  lua_setmetatable( L, -2 );

  // pop the metatable
  lua_pop( L, 1 );
}

////////////////////////////////
// cast GifImage object into Lua object
////////////////////////////////////////////////////////
int LuaGifImageImpl::Cast2Lua( lua_State* L, vp::GifImage* pGifImage,
                               LuaGifUD* pGifUD )
{
  // create a new userdatum (a Lua object)
  void* pUD = lua_newuserdata( L, sizeof(LuaGifImageUD) );

  // initialize the userdatum
  LuaGifImageUD* pImageUD = static_cast<LuaGifImageUD*>(pUD);
  pImageUD->pGifImage = pGifImage;
  pImageUD->pGifUD = pGifUD; 
  pImageUD->status = Status::Normal;

  // add the userdatum to list
  pGifUD->pListImageUD->Add( pImageUD );

  // set metatable to the userdatum
  luaL_getmetatable( L, ID );
  lua_setmetatable ( L, -2 );

  return 1;
}

//////////////
// image2:Clone( image1 )
/////////////////////////////////////
int LuaGifImageImpl::Clone( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );

  auto pThisGifImage = CheckGifImage( L, 1 );
  auto pOtherGifImage = CheckGifImage( L, 2 );

  bool Error = false;
  try
  {
    *pThisGifImage = *pOtherGifImage;
  }
  catch( const vp::Exception& )
  {
    // luaL_argerror() should be called outside of catch-block,
    // b/c it never returns; otherwise, the exception object
    // won't be deallocated.
    Error = true;
  }

  if( Error )
  {
    const char* msg = lua_pushfstring( L, "argument '%s' object belongs to an incompatible '%s' object",
                                       ID, LuaGifImpl::ID );
    return luaL_argerror( L, 2, msg );
  }
  else
    return 0;
}

/////////////
// bpp = image:BitsPerPixel()
// image:BitsPerPixel( bpp )
///////////////////////////////////////////
int LuaGifImageImpl::BitsPerPixel( lua_State* L )
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );
  LuaGifImageUD* pImageUD = CheckGifImageUD( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pImageUD->pGifImage->BitsPerPixel() );

    return 1;
  }
  else
  {
    auto bpp = LuaUtil::CheckUint8( L, 2 );
    if( pImageUD->pGifImage->ColorTable() )
      LuaUtil::CheckValueRange( L, 2, bpp, 2, 8 );
    else
      LuaUtil::CheckValueRange( L, 2, bpp, 2, pImageUD->pGifUD->pGif->BitsPerPixel() );

    pImageUD->pGifImage->BitsPerPixel( bpp );

    return 0;
  }
}

////////////
// left = image:Left()
//////////////////////////////////
int LuaGifImageImpl::Left( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushunsigned( L, pGifImage->Left() );

  return 1;  
}

/////////////
// top = image:Top()
//////////////////////////////////
int LuaGifImageImpl::Top( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushunsigned( L, pGifImage->Top() );

  return 1;  
}

/////////////
// width = image:Width()
//////////////////////////////////
int LuaGifImageImpl::Width( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushunsigned( L, pGifImage->Width() );

  return 1;  
}

////////////
// height = image:Height()
//////////////////////////////////
int LuaGifImageImpl::Height( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushunsigned( L, pGifImage->Height() );

  return 1;  
}

/////////////
// w, h = image:Dimension()
//////////////////////////////////
int LuaGifImageImpl::Dimension( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 1 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushunsigned( L, pGifImage->Width() );
  lua_pushunsigned( L, pGifImage->Height() );

  return 2;
}

///////////////////
// image:Crop( left, top, width, height )
//////////////////////////////////////////
int LuaGifImageImpl::Crop( lua_State* L )
{
  LuaUtil::CheckArgs( L, 5 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );

  auto LeftLower = pGifImage->Left();
  auto LeftUpper = LeftLower + pGifImage->Width();
  auto TopLower = pGifImage->Top();
  auto TopUpper = TopLower + pGifImage->Height();

  auto Left = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueRangeEx( L, 2, Left, LeftLower, LeftUpper );

  auto Top = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueRangeEx( L, 3, Top, TopLower, TopUpper );

  auto Width = LuaUtil::CheckUint16( L, 4 );
  LuaUtil::CheckValueRange( L, 4, Width, 1, LeftUpper - Left );

  auto Height = LuaUtil::CheckUint16( L, 5 );
  LuaUtil::CheckValueRange( L, 5, Height, 1, TopUpper - Top );

  pGifImage->Crop( Left, Top, Width, Height );

  return 0;
}

/////////////////
// image:SetAllPixels( color_index )
//////////////////////////////////////////
int LuaGifImageImpl::SetAllPixels( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );

  auto pGifImage = CheckGifImage( L, 1 );

  auto ColorIndex = LuaUtil::CheckUint8( L, 2 );
  auto Size = CheckColorTable( L, pGifImage );
  LuaUtil::CheckValueUpper( L, 2, ColorIndex, Size );

  pGifImage->SetAllPixels( ColorIndex );

  return 0;  
}

//////////////////
// image:SetPixel( x, y, color_index )
//////////////////////////////////////////
int LuaGifImageImpl::SetPixel( lua_State* L )
{
  LuaUtil::CheckArgs( L, 4 );

  auto pGifImage = CheckGifImage( L, 1 );

  auto X = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, X, pGifImage->Width() );

  auto Y = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueUpper( L, 3, Y, pGifImage->Height() );

  auto ColorIndex = LuaUtil::CheckUint8( L, 4 );
  auto Size = CheckColorTable( L, pGifImage );
  LuaUtil::CheckValueUpper( L, 4, ColorIndex, Size );

  pGifImage->SetPixel( X, Y, ColorIndex );

  return 0;
}

//////////////////////
// color_index = image:GetPixel( x, y )
//////////////////////////////////////////
int LuaGifImageImpl::GetPixel( lua_State* L )
{
  LuaUtil::CheckArgs( L, 3 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );

  auto X = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, X, pGifImage->Width() );

  auto Y = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueUpper( L, 3, Y, pGifImage->Height() );

  lua_pushunsigned( L, pGifImage->GetPixel( X, Y ) );

  return 1;
}

/////////////////
// ret_bool = image:Transparent( x, y )
////////////////////////////////////////
int LuaGifImageImpl::Transparent( lua_State* L )
{
  LuaUtil::CheckArgs( L, 3 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );

  auto X = LuaUtil::CheckUint16( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, X, pGifImage->Width() );

  auto Y = LuaUtil::CheckUint16( L, 3 );
  LuaUtil::CheckValueUpper( L, 3, Y, pGifImage->Height() );

  lua_pushboolean( L, pGifImage->Transparent( X, Y ) );

  return 1;
}

/////////////////
// ret_bool = image:Interlaced()
//////////////////////////////////
int LuaGifImageImpl::Interlaced( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushboolean( L, pGifImage->Interlaced() );

  return 1;  
}

///////////////////////
// centisecond = image:Delay()
// image:Delay( centisecond )
///////////////////////////////////////
int LuaGifImageImpl::Delay( lua_State* L )
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );
  auto pGifImage = CheckGifImage( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pGifImage->Delay() );

    return 1;
  }
  else
  {
    if( pGifImage->SingleImage() )
      return luaL_error( L, "cannot set delay time for single image" );

    auto Centisecond = LuaUtil::CheckUint16( L, 2 );
    pGifImage->Delay( Centisecond );

    return 0;  
  }
}

////////////////
// ret_bool = image:ColorTable()
/////////////////////////////////////////////
int LuaGifImageImpl::ColorTable( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushboolean( L, pGifImage->ColorTable() );

  return 1;  
}

/////////////////
// size = image:ColorTableSize()
// image:ColorTableSize( size )
///////////////////////////////////////////
int LuaGifImageImpl::ColorTableSize( lua_State* L )
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );

  auto pGifImage = CheckGifImage( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pGifImage->ColorTableSize() );

    return 1;
  }
  else
  {
    auto Size = LuaUtil::CheckUint16( L, 2 );
    LuaUtil::CheckValueRange( L, 2, Size, 0, 256 );

    pGifImage->ColorTableSize( Size );

    return 0;
  }
}

///////////////
// ret_bool = image:ColorTableSorted()
////////////////////////////////////////
int LuaGifImageImpl::ColorTableSorted( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushboolean( L, pGifImage->ColorTableSorted() );

  return 1;  
}

////////////////////
// image:SetColorTable( index, r, g, b )
//////////////////////////////////////////////
int LuaGifImageImpl::SetColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 5 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  if( !pGifImage->ColorTable() )
    luaL_error( L, "no local color table" );

  auto Index = LuaUtil::CheckUint8( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, pGifImage->ColorTableSize() );

  auto Red   = LuaUtil::CheckUint8( L, 3 );
  auto Green = LuaUtil::CheckUint8( L, 4 );
  auto Blue  = LuaUtil::CheckUint8( L, 5 );

  pGifImage->SetColorTable( Index, Red, Green, Blue );

  return 0;
}

///////////////////
// r, g, b = image:GetColorTable( index )
///////////////////////////////////////////////
int LuaGifImageImpl::GetColorTable( lua_State* L ) 
{
  LuaUtil::CheckArgs( L, 2 );

  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  if( !pGifImage->ColorTable() )
    luaL_error( L, "no local color table" );

  auto Index = LuaUtil::CheckUint8( L, 2 );
  LuaUtil::CheckValueUpper( L, 2, Index, pGifImage->ColorTableSize() );

  uint8_t Red, Green, Blue;
  pGifImage->GetColorTable( Index, Red, Green, Blue );

  lua_pushunsigned( L, Red );
  lua_pushunsigned( L, Green );
  lua_pushunsigned( L, Blue );

  return 3;
}

/////////////
// method_id = image:DisposalMethod()
// image.DisposalMethod( method_id )
///////////////////////////////////////////
int LuaGifImageImpl::DisposalMethod( lua_State* L )
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );
  auto pGifImage = CheckGifImage( L, 1 );
  if( argc == 1 )
  {
    lua_pushunsigned( L, pGifImage->DisposalMethod() );

    return 1;
  }
  else
  {
    if( pGifImage->SingleImage() )
      return luaL_error( L, "cannot set disposal method for single image" );

    auto MethodID = LuaUtil::CheckUint8( L, 2 );
    LuaUtil::CheckValueRange( L, 2, MethodID, 0, 3 );
    pGifImage->DisposalMethod( MethodID );

    return 0;
  }
}

/////////////////
// ret_bool = image:HasTransColor()
// img.HasTransColor( true|false )
//////////////////////////////////
int LuaGifImageImpl::HasTransColor( lua_State* L )
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );
  auto pGifImage = CheckGifImage( L, 1 );
  if( argc == 1 )
  {
    lua_pushboolean( L, pGifImage->HasTransColor() );

    return 1;
  }
  else
  {
    if( pGifImage->SingleImage() )
      return luaL_error( L, "cannot turn on transparent color for single image" );

    bool TurnOn = LuaUtil::CheckBoolean( L, 2 );
    pGifImage->HasTransColor( TurnOn );

    return 0;
  }
}

/////////////
// color_index = image:TransColor()
// image:TransColor( color_index )
///////////////////////////////////////////
int LuaGifImageImpl::TransColor( lua_State* L )
{
  auto argc = LuaUtil::CheckArgs( L, 1, 1 );

  auto pGifImage = CheckGifImage( L, 1 );
  if( pGifImage->SingleImage() )
    return luaL_error( L, "single image has no transparent color" );

  if( argc == 1 )
  {
    lua_pushunsigned( L, pGifImage->TransColor() );

    return 1;
  }
  else
  {
    auto ColorIndex = LuaUtil::CheckUint8( L, 2 );
    auto Size = CheckColorTable( L, pGifImage );
    LuaUtil::CheckValueUpper( L, 2, ColorIndex, Size );

    pGifImage->TransColor( ColorIndex );

    return 0;
  }
}

///////////////
// meta method __eq
// for operator '==' and '~='
////////////////////////////////////
int LuaGifImageImpl::Equal( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );

  auto pGifImage1 = CheckGifImage( L, 1 );
  auto pGifImage2 = CheckGifImage( L, 2 );
  lua_pushboolean( L, *pGifImage1 == *pGifImage2 );

  return 1;
}

//////////////////
// type name of argument arg
//////////////////////////////////////////////////
const char* LuaGifImageImpl::TypeName( lua_State* L, int arg )
{
  if( luaL_testudata( L, arg, ID ) != nullptr )  // LuaGifImage type
    return ID;
  else if( luaL_testudata( L, arg, LuaGifImpl::ID ) != nullptr )  // LuaGif type
    return LuaGifImpl::ID;
  else
    return luaL_typename( L, arg );
}

///////////////
// meta method __lt
// for operator '<', '<=', '>' and '>=', not supported
//////////////////////////////////////////////////////////
int LuaGifImageImpl::LessThan( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );
  const char* msg = lua_pushfstring( L, "unsupported comparison between '%s' and '%s'",
                                     TypeName(L, 1), TypeName(L, 2) );
  return luaL_error( L, msg );
}

///////////////
// meta method __tostring
////////////////////////////////////
int LuaGifImageImpl::ToString( lua_State* L )
{
  vp::GifImage* pGifImage = CheckGifImage( L, 1 );
  lua_pushfstring( L, "%s: bpp=%d (%d,%d) %dx%d colors=%d",
                   ID, pGifImage->BitsPerPixel(),
                   pGifImage->Left(),  pGifImage->Top(),
                   pGifImage->Width(), pGifImage->Height(),
                   pGifImage->ColorTableSize() );
  return 1;
}

///////////////
// meta method __gc
///////////////////////////////////////
int LuaGifImageImpl::Finalizer( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  void* pUD = luaL_checkudata( L, 1, ID );
  LuaGifImageUD* pImageUD = static_cast<LuaGifImageUD*>(pUD);
  if( pImageUD == nullptr )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object", ID );
    luaL_argerror( L, 1, msg );
  }

  // set itself to invalid and remove itself from the list
  if( pImageUD->status == Status::Normal )
  {
    pImageUD->status = Status::Invalid;
    pImageUD->pGifUD->pListImageUD->Remove( pImageUD );
  }

  // do not need to delete it, as it instantiated in vp::Gif
  if( pImageUD->pGifImage != nullptr )
    pImageUD->pGifImage = nullptr;

  return 0;
}

/////////////////////////
// check if the argument(a Lua object) is a LuaGifImageUD object
//////////////////////////////////////////////////////////////
LuaGifImageUD* LuaGifImageImpl::CheckGifImageUD( lua_State* L, int arg )
{
  void* pUD = luaL_checkudata( L, arg, ID );

  // this should no happen, just in case
  LuaGifImageUD* pImageUD = static_cast<LuaGifImageUD*>(pUD);
  if( pImageUD == nullptr )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object", ID );
    luaL_argerror( L, arg, msg );
  }

  // in case __gc() has been called directly, e.g. image:__gc()
  if( pImageUD->pGifImage == nullptr )
  {
    const char* msg = lua_pushfstring( L, "'%s' object is out of scope", ID );
    luaL_argerror( L, arg, msg );
  }

  if( pImageUD->status == Status::Orphaned )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object, the '%s' it belongs to is out of scope",
                                       ID, LuaGifImpl::ID );
    luaL_argerror( L, arg, msg );
  }

  if( pImageUD->status == Status::Abandoned )
  {
    const char* msg = lua_pushfstring( L, "invalid '%s' object, it has been removed from '%s' object",
                                       ID, LuaGifImpl::ID );
    luaL_argerror( L, arg, msg );
  }

  return pImageUD; 
}

//////////////////////////////
// check if the argument(a Lua object) is a LuaGifImageUD object
//////////////////////////////////////////////////////////////
vp::GifImage* LuaGifImageImpl::CheckGifImage( lua_State* L, int arg )
{
  LuaGifImageUD* pImageUD = CheckGifImageUD( L, arg );

  return pImageUD->pGifImage; 
}

//////////////////////////////////////////////////////
uint16_t LuaGifImageImpl::CheckColorTable( lua_State* L, vp::GifImage* pGifImage )
{
  auto Size = pGifImage->CheckColorTable();
  if( Size == 0 )
    return luaL_error( L, "there is neither global nor local color table" );

  return Size;
}
