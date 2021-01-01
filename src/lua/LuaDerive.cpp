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
#include "config.h"
#include <algorithm>

///////////////////////////////
namespace LuaDeriveImpl
{
  // ID of LuaDerive and metatable
  constexpr char ID[] = {PACKAGE_NAME ".derived"};

  // key of a field in LuaDerive, the field stores the userdata
  constexpr char SuperKey[] = { "super" };

  // closure
  int Caller( lua_State* L );

  // meta methods
  int Indexing( lua_State* L );
  int ToString( lua_State* L );
  int LengthOp( lua_State* L );
  int IPairs( lua_State* L );

  // utils
  int CheckDerived( lua_State* L, int arg );
  int PushSuper( lua_State* L, int index );   // not in use
  int ReplaceWithSuper( lua_State* L, int index );
  int CallMetamethod( lua_State* L, const char* method );
}

////////////////////////
namespace
{
  // __tostring of metatable of LuaDerive's metatable
  int ToString( lua_State* L )
  {
    luaL_checktype( L, 1, LUA_TTABLE );
    lua_pushfstring( L, "metatable of %s", LuaDeriveImpl::ID );

    return 1;
  }
}

////////////////
// register metatable
////////////////////////////////////
void LuaDerive::Register( lua_State* L )
{
  luaL_newmetatable( L, LuaDeriveImpl::ID ); 

  // meta fields
  lua_pushstring( L, "__id" );
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

  lua_pushstring( L, "__ipairs" );
  lua_pushcfunction( L, LuaDeriveImpl::IPairs );
  lua_settable( L, -3 );

  // add a metatable to the metatable
  lua_createtable( L, 0, 1 );
  lua_pushstring( L, "__tostring" );
  lua_pushcfunction( L, ToString );
  lua_settable( L, -3 );
  lua_setmetatable( L, -2 );

  //pop the metatable
  lua_pop( L, 1 );
}

//////////////////
// Create a vp.derived object: a table with field 'super' pointing
// to a userdata or another table
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
  lua_pushstring( L, LuaDeriveImpl::SuperKey );
  lua_pushvalue( L, 1 ); 
  lua_settable( L, 2 );

  // set metatable to the table
  luaL_getmetatable( L, LuaDeriveImpl::ID );
  lua_setmetatable( L, 2 );

  return 1;
}

//////////////////////////
// This function replaces the implicit argument 'self' (which is
// a vp.derived object) with its super and calls the method. This
// is how methods of super get called from vp.derived, i.e.
// this function makes calling
//   vp.derived:method(arg1, arg2)
// actually calls
//   super:method(arg1, arg2)
//
// When super["key"] is a function, this function, together with
// a upvalue (super["key"]), gets returned by Indexing().
////////////////////////////////////////////////////////////////
int LuaDeriveImpl::Caller( lua_State* L )
{
  // stack: vp.derived + args
  CheckDerived( L, 1 );
  ReplaceWithSuper( L, 1 );

  // insert upvalue (method to be called) to the bottom
  lua_pushvalue( L, lua_upvalueindex(1) );
  lua_insert( L, 1 );

  // stack: method + super + args
  // call method(super, args)
  lua_call( L, lua_gettop(L) - 1, LUA_MULTRET );
  return lua_gettop(L);
}

////////////////////////
// meta method __index
// It gets called, when vp.derived itself does not contain the inquired field.
// It searches for the field from its super, i.e. super[key].
// If super[key] is a function, it returns a closure; otherwise, super[key].
/////////////////////////////////////////////////////////////////
int LuaDeriveImpl::Indexing( lua_State* L )
{
  // stack: vp.derived, key
  LuaUtil::CheckArgs( L, 2 );
  CheckDerived( L, 1 );
  ReplaceWithSuper( L, 1 );

  // stack: super, key
  // store a copy of key for use in error message
  lua_pushvalue( L, 2 );
  lua_insert( L, 1 );

  // stack: key, super, key
  // search for super[key]
  lua_gettable( L, 2 );        // lua_gettable() may or may not return,
  if( lua_isnil( L, 3 ) )      // depending on __index of super
    return luaL_error( L, "'%s' object has no field '%s'",
                       ID, luaL_tolstring(L, 1, nullptr) );

  // stack: key, super, super[key]
  // if super[key] is a function, use it as upvalue to form a closure
  if( lua_type( L, 3 ) == LUA_TFUNCTION )
    lua_pushcclosure( L, Caller, 1 );

  return 1;  // closure or super[key]
}

////////////////////
// meta method __tostring
///////////////////////////////////////
int LuaDeriveImpl::ToString( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  CheckDerived( L, 1 );
  ReplaceWithSuper( L, 1 );
  lua_pushfstring( L, "%s(%s)", ID, luaL_tolstring(L, 1, nullptr) );
  return 1;
}

///////////////////
// meta method __len
// It gets called when applying operator # to vp.derived.
//////////////////////////////////////////////////////
int LuaDeriveImpl::LengthOp( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1, 1 );  // why 2 args, see LuaGifImpl::Images()
  return CallMetamethod( L, "__len" );
}

///////////////////
// meta method __ipairs
// It gets called when calling ipairs() with vp.derived.
/////////////////////////////////////////////////////
int LuaDeriveImpl::IPairs( lua_State* L )
{
  LuaUtil::CheckArgs( L, 1 );
  return CallMetamethod( L, "__ipairs" );
}

/////////////////////
// call to meta method() of super
// Assume initial stack: vp.derived + args
///////////////////////////////////////////////
int LuaDeriveImpl::CallMetamethod( lua_State* L, const char* method )
{
  // stack: vp.derived + args
  CheckDerived( L, 1 );
  ReplaceWithSuper( L, 1 );

  // stack: super + args
  // get meta method of super
  if( !luaL_getmetafield( L, 1, method ) )
    return luaL_error( L, "object '%s' has no metamethod '%s'",
                       luaL_tolstring(L, 1, nullptr), method );

  if( !lua_isfunction( L, -1 ) )
    return luaL_error( L, "'%s' of object '%s' is not a function",
                       method, luaL_tolstring(L, -2, nullptr) );

  // put method to bottom
  lua_insert( L, 1 );

  // stack: method + super + args
  // call method(super, args)
  lua_call( L, lua_gettop(L) - 1, LUA_MULTRET );
  return lua_gettop(L);
}

//////////////////
// Check if arg is a vp.derived
/////////////////////////////////////////////////
int LuaDeriveImpl::CheckDerived( lua_State* L, int arg )
{
  bool IsDerived = false;
  // vp.derived is a table with __id == ID
  if( lua_istable(L, arg) && luaL_getmetafield(L, arg, "__id") )
  {
    if( lua_type(L, -1) == LUA_TSTRING && std::equal(ID, ID + sizeof(ID), lua_tostring(L, -1)) )
      IsDerived = true;

    lua_pop( L, 1 );  // meta field
  }

  if( IsDerived  )
    return 0;
  else
    return luaL_argerror( L, arg, lua_pushfstring(L, "expected '%s' object, got '%s'",
                                                  ID, lua_typename(L, lua_type(L, arg))) );
}

///////////////////////////
// Pushes field 'super' of vp.derived at the index onto the stack.
/////////////////////////////////////////////////////////////////
int LuaDeriveImpl::PushSuper( lua_State* L, int index )
{
  lua_pushstring( L, SuperKey );
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

///////////////////////////
// Replace vp.derived object at the index with its super object.
////////////////////////////////////////////////////////////////
int LuaDeriveImpl::ReplaceWithSuper( lua_State* L, int index )
{
  lua_pushstring( L, SuperKey );
  lua_rawget( L, index );
  if( lua_isnoneornil( L, -1 ) )
    return luaL_error( L, "'%s' object should have field 'super' but does not", ID );

  int Type = lua_type( L, -1 );
  if( Type != LUA_TUSERDATA && Type != LUA_TTABLE )
    return luaL_error( L, "field 'super' of '%s' should be '%s' or '%s'(got '%s')",
                       ID, lua_typename( L, LUA_TUSERDATA ),
                       lua_typename( L, LUA_TTABLE ), lua_typename( L, Type ) );

  lua_replace( L, index );

  return 0;
}
