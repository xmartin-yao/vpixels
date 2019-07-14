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
#include "LuaDerive.h"
#include "LuaUtil.h"
#include <algorithm>

///////////////////////////////
namespace LuaDeriveImpl
{
  // ID of LuaDerive and metatable
  const char ID[] = "vp.derived";

  // closure
  int Caller( lua_State* L );

  // meta methods
  int Indexing( lua_State* L );
  int ToString( lua_State* L );
  int LengthOp( lua_State* L );

  // utils
  int CheckDerived( lua_State* L, int arg );
  int PushSuper( lua_State* L, int index );
}

////////////////
// register metatable
////////////////////////////////////
void LuaDerive::Register( lua_State* L )
{
  luaL_newmetatable( L, LuaDeriveImpl::ID ); 

  // meta fields
  lua_pushstring( L, "__metatable" );
  lua_pushstring ( L, LuaDeriveImpl::ID );
  lua_settable( L, -3 ); 

  lua_pushstring( L, "__index" );
  lua_pushcfunction( L, LuaDeriveImpl::Indexing );
  lua_settable( L, -3 );

  lua_pushstring( L, "__tostring" );
  lua_pushcfunction( L, LuaDeriveImpl::ToString );
  lua_settable( L, -3 );

  lua_pushstring( L, "__len" );
  lua_pushcfunction( L, LuaDeriveImpl::LengthOp );
  lua_settable( L, -3 );

  //pop the metatable
  lua_pop( L, 1 );
}

//////////////////
// Create a derived object: a table with field 'super' pointing to userdata
/////////////////////////////////////////////////
int LuaDerive::New( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1, 1 );

  // 1st arg: userdata or table
  int Type = lua_type( L, 1 );
  if( Type != LUA_TUSERDATA && Type != LUA_TTABLE )
  {
    const char* msg = lua_pushfstring( L, "expected '%s' or '%s', got '%s'",
                                       lua_typename(L, LUA_TUSERDATA),
                                       lua_typename(L, LUA_TTABLE),
                                       lua_typename(L, Type) );
    return luaL_argerror( L, 1, msg );
  }

  // 2nd arg: table (optional) 
  if( lua_isnoneornil( L, 2 ) )
    lua_newtable( L );
  else
    luaL_checktype( L, 2, LUA_TTABLE );

  // add field 'super' to the table
  lua_pushstring( L, "super" );
  lua_pushvalue( L, 1 ); 
  lua_settable( L, 2 );

  // set metatable to the table
  luaL_getmetatable( L, LuaDeriveImpl::ID );
  lua_setmetatable( L, 2 );

  return 1;
}

//////////////////////////
// This function replaces the implicit argument 'self', which is 
// a vp.derived object, with super and calls super["key"]. This
// is how methods of super get called from vp.derived, i.e.
// this function makes calling
//   vp.derived:method(arg1, ...)
// actually call
//   super:methods(arg1, ...)
//
// When super["key"] is a function, this function, together with
// two upvalues (super["key"] and super), get returned by Indexing(). 
////////////////////////////////////////////////////////////////
int LuaDeriveImpl::Caller( lua_State* L )
{
  // initial stack (from bottom to top):
  // vp.derived + other args passed to closure
  CheckDerived( L, 1 );

  // replace vp.derived with upvalue #1 (super)
  lua_pushvalue(L, lua_upvalueindex(1));  // super
  lua_replace(L, 1);

  // insert upvalue #2 (the function to be called) to the bottom
  lua_pushvalue(L, lua_upvalueindex(2));
  lua_insert(L, 1);

  // stack (from bottom up):
  // the function + super + other args passed to closure

  // call the function
  lua_call( L, lua_gettop(L) - 1, LUA_MULTRET );
  return lua_gettop(L);
}

////////////////////////
// meta method __index
// This function gets called, when vp.derived does not 
// contain the field. This function searches for super["key"].
///////////////////////////////////////////////////////////
int LuaDeriveImpl::Indexing( lua_State* L )
{
  LuaUtil::CheckArgs( L, 2 );
  CheckDerived( L, 1 );

  // vp.derived["super"]
  PushSuper( L, 1 );

  // search for super["key"]
  lua_pushvalue( L, 2 );  // key
  lua_gettable( L, 3 );
  if( lua_isnoneornil( L, -1 ) )
    return luaL_error( L, "'%s' object has no field '%s'", ID, lua_tostring(L,2) );

  // if super["key"] is a function, return a closure with two upvalues:
  //   upvalue #2 -> super["key"], upvalue #1 -> super
  if( lua_type( L, -1 ) == LUA_TFUNCTION )
    lua_pushcclosure( L, Caller, 2 );

  return 1;
}

////////////////////
// meta method __tostring
///////////////////////////////////////
int LuaDeriveImpl::ToString( lua_State* L )
{
  CheckDerived( L, 1 );

  // vp.derived["super"]
  PushSuper( L, 1 );
  lua_pushfstring( L, "%s (%s)", ID, luaL_tolstring(L, -1, nullptr) ); 
  return 1;
}

///////////////////
// meta method __len
// if super.__len is a function, call it and
// return result; otherwise, return 0.
////////////////////////////////////////
int LuaDeriveImpl::LengthOp( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1, 1 );
  CheckDerived( L, 1 );

  // vp.derived["super"]
  PushSuper( L, 1 );

  // super["__len"]
  if( luaL_getmetafield( L, -1, "__len" ) )
  {
    if( lua_isfunction( L, -1 ) )
    {
      lua_pushvalue( L, -2 );
      lua_call( L, 1, 1 );  // call super:__len()
    }

    // return other types as-is
  }
  else
    lua_pushunsigned( L, 0 );

  return 1;
}

//////////////////
// Check if arg is a vp.derived
/////////////////////////////////////////////////
int LuaDeriveImpl::CheckDerived( lua_State* L, int arg )
{
  bool IsDerived = false;
  // a table and __metatable == ID
  if( lua_type(L, arg) == LUA_TTABLE && luaL_getmetafield(L, arg, "__metatable") )
  {
    if( lua_isstring(L, -1) && std::equal(ID, ID + sizeof(ID), lua_tostring(L, -1)) )
      IsDerived = true;

    lua_pop( L, 1 );  // pop meta field
  }

  if( !IsDerived  )
  {
    const char* msg = lua_pushfstring( L, "expected '%s' object, got '%s'",
                                       ID, lua_typename(L, lua_type(L, arg)) );
    return luaL_argerror( L, arg, msg );
  }

  return 0;
}

///////////////////////////
// Pushes field 'super' of vp.derived at the index onto the stack.
/////////////////////////////////////////////////////////////////
int LuaDeriveImpl::PushSuper( lua_State* L, int index )
{
  lua_pushstring( L, "super" );
  lua_rawget( L, index );
  if( lua_isnoneornil( L, -1 ) )
    return luaL_error( L, "'%s' object should have field 'super' but does not", ID );

  int Type = lua_type( L, -1 );
  if( Type != LUA_TUSERDATA && Type != LUA_TTABLE )
    return luaL_error( L, "field 'super' of '%s' should be '%s' or '%s'(got '%s')",
                       ID, lua_typename( L, LUA_TUSERDATA ),
                       lua_typename( L, LUA_TTABLE ), lua_typename( L, Type ) );
  return 0;
}
