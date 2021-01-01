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
#include <cmath>
#include <cstring>
#include <algorithm>
#include "LuaUtil.h"

///////////////////////////////
// check number of arguments
//   args: number of mandatory arguments
//   opts: number of optional arguments
///////////////////////////////////////////////////////////
int LuaUtil::CheckArgs( lua_State* L, int args, int opts )
{
  int actual = lua_gettop(L);

  // all are mandatory arguments
  if( actual == args )
    return actual;

  // optional arguments provided
  if( actual > args )
  {
    // number of arguments doesn't exceed
    if( actual <= args + opts )
      return actual;
    else
      args += opts;
  }

  lua_Debug ar;
  if( lua_getstack(L, 0, &ar) == 0 )
  {
    if( args > 1 )
      return luaL_error( L, "%d arguments expected (got %d)", args, actual );
    else
      return luaL_error( L, "%d argument expected (got %d)", args, actual );
  }

  lua_getinfo(L, "n", &ar);
  if( strcmp(ar.namewhat, "method") == 0 )
  {
    --args;
    --actual;
  }

  if( ar.name == nullptr ) ar.name = "?";

  if( args > 1 )
    return luaL_error( L, "'%s' expects %d arguments (got %d)", ar.name, args, actual );
  else
    return luaL_error( L, "'%s' expects %d argument (got %d)", ar.name, args, actual );
}

///////////////////////////////////////////////
uint8_t LuaUtil::CheckUint8( lua_State* L, int arg )
{
  luaL_checktype( L, arg, LUA_TNUMBER );
  lua_Number n = lua_tonumber( L, arg );
  luaL_argcheck( L, (n >= 0 && n == floor(n)), arg, "unsigned integer expected" );
  CheckValueRange( L, arg, n, 0, 0xFF );

  return static_cast<uint8_t>(n);
}

//////////////////////////////////////
// Unlike luaL_checkunsigned(lua_State *L, int arg),
// this function makes sure that the argument 
//  1) is exactly a number (strings that can be converted to numbers
//     are rejected)
//  2) is not a float (does not round up, e.g. 2.1 is not allowed, 
//     2.0 is treated as integer 2)
//  3) is not negative (luaL_checkunsigned() casts -1 to 4294967295 )
////////////////////////////////////////////////////////////
uint16_t LuaUtil::CheckUint16( lua_State* L, int arg )
{
  luaL_checktype( L, arg, LUA_TNUMBER );
  lua_Number n = lua_tonumber( L, arg );
  luaL_argcheck( L, (n >= 0 && n == floor(n)), arg, "unsigned integer expected" );
  CheckValueRange( L, arg, n, 0, 0xFFFF );

  return static_cast<uint16_t>(n);
}

//////////////////////////////////////////
// luaL_optunsigned(lua_State *L, int arg, lua_Unsigned u) is not suitable,
// for the reason as luaL_checkunsigned() is not.
//////////////////////////////////////////////////////////////
uint16_t LuaUtil::OptUint16( lua_State* L, int arg, uint16_t def )
{
  return (lua_isnoneornil( L, arg ))? def : CheckUint16( L, arg );
}

///////////////////////////////
// there is no luaL_checkboolean() 
////////////////////////////////////////////////
bool LuaUtil::CheckBoolean( lua_State* L, int arg )
{
  luaL_checktype( L, arg, LUA_TBOOLEAN );
  return lua_toboolean( L, arg );
}

////////////////////////////////
// there is no luaL_optboolean()
/////////////////////////////////////////////////////
bool LuaUtil::OptBoolean( lua_State* L, int arg, bool def )
{
  return (lua_isnoneornil( L, arg ))? def : CheckBoolean( L, arg );
}

////////////////////////////
// check if value >= lower
/////////////////////////////////////////////////////////////////
void LuaUtil::CheckValueLower( lua_State* L, int arg, uint16_t value, uint16_t lower )
{
  if( value < lower )
  {
    const char* msg = lua_pushfstring( L, "expected value >= %d, got %d",
                                       lower, value );
    luaL_argerror( L, arg, msg );
  }
}

////////////////////////////
// check if value < upper
/////////////////////////////////////////////////////////////////
void LuaUtil::CheckValueUpper( lua_State* L, int arg, uint16_t value, uint16_t upper )
{
  if( value >= upper )
  {
    const char* msg = lua_pushfstring( L, "expected value < %d, got %d",
                                       upper, value );
    luaL_argerror( L, arg, msg );
  }
}

///////////////////////////////////////
// check if value is within range [lower, upper]
/////////////////////////////////////////////////////////////////
void LuaUtil::CheckValueRange( lua_State* L, int arg, lua_Number value,
                               uint16_t lower, uint16_t upper )
{
  if( value < lower || value > upper )
  {
    const char* msg = lua_pushfstring( L, "expected within [%d,%d], got %d",
                                       lower, upper, value );
    luaL_argerror( L, arg, msg );
  }
}

///////////////////////////////////////
// check if value is within range [lower, upper)
/////////////////////////////////////////////////////////////////
void LuaUtil::CheckValueRangeEx( lua_State* L, int arg, uint16_t value,
                                 uint16_t lower, uint16_t upper )
{
  if( value < lower || value >= upper )
  {
    const char* msg = lua_pushfstring( L, "expected within [%d,%d], got %d",
                                       lower, upper - 1, value );
    luaL_argerror( L, arg, msg );
  }
}


// utils for LuaUtil::Indexing() and LuaUtil::NewIndex()
namespace
{
  //////////////////
  // Key of a field in the table associated to userdata
  //
  // The field stores a boolean value. LuaUtil::NewIndex() keeps the field
  // unmodifiable from outside and LuaUtil::Indexing() does searches based
  // on its value.
  /////////////////////////////////////////////////////////////////////////
  constexpr char ReservedKey[] = { "base" };

  /////////////
  // compare Key to ReservedKey
  ////////////////////////////////
  inline bool IsReserved( const char* Key )
  {
    return std::equal( ReservedKey, ReservedKey + sizeof(ReservedKey), Key );
  }

  /////////////
  // Return true, if the value at Index is a string and is the same as
  // the reserved key; false, otherwise.
  /////////////////////////////////////////////
  inline bool IsReserved( lua_State* L, int Index )
  {
    return lua_type(L, Index) == LUA_TSTRING && IsReserved(lua_tostring(L, Index));
  }

  /////////////
  // Retrieve the value of the reserved field in uservalue.
  // Index: index to userdata
  ///////////////////////////////////////////
  bool MetatableOnly( lua_State* L, int Index )
  {
    lua_getuservalue( L, Index );

    // get the field
    lua_pushstring( L, ReservedKey );
    lua_rawget( L, -2 );

    // retrieve its value
    bool Result = lua_toboolean( L, -1 );

    lua_pop( L, 2 );  // uservalue, reserved field

    return Result;
  }

  //////////////
  // Set the value of the reserved field in uservalue.
  // Index: index to userdata
  // Value: value to be set
  /////////////////////////////////////////////////////////////////
  void SetMetatableOnly( lua_State* L, int Index, bool Value = true )
  {
    lua_getuservalue( L, Index );

    // set the field using Value
    lua_pushstring( L, ReservedKey );
    lua_pushboolean( L, Value );
    lua_rawset( L, -3 );

    lua_pop( L, 1 );  // uservalue
  }

  //////////////
  // Retrieve a field from a table.
  // If successful, replace the table with the field and return true;
  // otherwise, remove the table and return false.
  /////////////////////////////////////////////////////////
  bool GetTableField( lua_State* L )
  {
    // stack: userdata, key, table
    lua_pushvalue( L, 2 );
    lua_rawget( L, 3 );
    if( lua_isnil(L, 4) )
    {
      lua_pop( L, 2 );  // table, nil

      // stack: userdata, key
      return false;
    }
    else
    {
      lua_remove( L, 3 );  // table

      // stack: userdata, key, table[key]
      return true;
    }
  }

  //////////////
  // Retrieve a field from uservalue of userdata.
  /////////////////////////////////////////
  bool GetUservalueField( lua_State* L )
  {
    // stack: userdata, key
    lua_getuservalue( L, 1 );

    return GetTableField( L );
  }

  //////////////
  // Retrieve a field from metatable of userdata.
  /////////////////////////////////////////
  bool GetMetafield( lua_State* L )
  {
    // stack: userdata, key
    if( lua_getmetatable(L, 1) == 0 )
      return false;

    return GetTableField( L );
  }

  /////////////////
  // Set the value of the reserved field to true and return userdata.
  // This function prepares LuaUtil::Indexing() to be called again,
  // in the case that reserved key is explicitly referred, e.g.
  //    ud.base:func()
  // In this case, LuaUtil::Indexing() gets called twice:
  // 1) To resolve ud.base. This function gets called.
  // 2) To resolve ud.func. LuaUtil::Indexing() searches only metatable of ud.
  //////////////////////////////////////////////////
  int PrepNextRound( lua_State* L, const char* ID )
  {
    // stack: userdata, key

    // the field already set to true, e.g. calling ud.base.base:func(),
    // though syntactically correct, is treated as an error.
    if( MetatableOnly(L, 1) )
    {
      // set to false, so if the method is called using pcall(),
      //   e.g. pcall(ud.base.base.func, ud)
      // ud is still in default/valid state.
      SetMetatableOnly( L, 1, false );

      return luaL_error( L, "'%s' of '%s' object has no field '%s'",
                         ReservedKey, ID, ReservedKey );
    }

    // switch to metatable only
    SetMetatableOnly( L, 1 );
    lua_pop( L, 1 );  // key

    // stack: userdata
    return 1;
  }

  /////////////
  // Check if the argument at arg is a valid userdata.
  /////////////////////////////////////////////
  void CheckUserdata( lua_State* L, int arg, const char* ID )
  {
    // argument is a full userdata
    if( lua_type(L, arg) != LUA_TUSERDATA )
      luaL_argerror( L, arg,
                     lua_pushfstring( L, "expected %s, got %s",
                                      ID, luaL_typename(L, arg) ) );

    // userdata has a table as its uservalue
    lua_getuservalue( L, arg );
    if( !lua_istable(L, -1) )
      luaL_argerror( L, arg, "it has no uservalue" );

    // uservalue has a reserved field that stores a boolean
    lua_pushstring( L, ReservedKey );
    lua_rawget( L, -2 );
    if( !lua_isboolean(L, -1) )
      luaL_argerror( L, arg,
                     lua_pushfstring(L, "it has no '%s' field", ReservedKey) );

    lua_pop( L, 2 );  // uservalue, value of reserved field
  }
} // anonymous namespace

////////////////
// Create and initialize a table, associate it to userdata.
//   Index: index to userdata
////////////////////////////////////////////////////////////
void LuaUtil::Extend( lua_State* L, int Index )
{
  lua_newtable( L );

  // add a field
  lua_pushstring( L, ReservedKey );
  lua_pushboolean( L, false );  // default value
  lua_rawset( L, -3 );

  // associate the table to userdata
  lua_setuservalue( L, Index );
}

////////////////
// Implementation of __index
//   ID: ID of userdata
//
// This function supports accessing data and calling methods in the following ways:
//   ud.var     -- var/func could be a field of ud's metatable
//   ud:func()  -- or the table associated to ud
//   ud.base.var  -- specify var/func is a field of ud's metatable
//   ud.base:func()
//
// ud.var or ud:func
//   First, this function searches the table associated to ud for var/func;
//   if not found, it searches the metatable of ud.
//
// ud.base.var or ud.base:func
//   This function gets called twice to accomplish the task.
//   1st time: it sets the value of ReservedKey to true and return ud
//   2nd time: it searches the metatable of ud for var/func
//
// Caveat:
//   In the following case, if var is a field in the associated table,
//   this function won't be able to find it and error message indicates
//   that it is not in 'base' of ud.
//     x = ud.base -- x gets ud, a mistake to do this or just for curiosity
//     y = ud.var  -- error out
///////////////////////////////////////////////////////////////////
int LuaUtil::Indexing( lua_State* L, const char* ID )
{
  // stack: userdata, key
  CheckArgs( L, 2 );
  CheckUserdata( L, 1, ID );

  // key is reserved
  if( IsReserved(L, 2) )
    return PrepNextRound( L, ID );

  if( MetatableOnly(L, 1) )
  {
    SetMetatableOnly( L, 1, false );

    // search metatable
    if( GetMetafield( L ) )
      return 1;
    else
      return luaL_error( L, "'%s' of '%s' object has no field '%s'",
                         ReservedKey, ID, luaL_tolstring(L, 2, nullptr) );
  }
  else
  {
    // search uservalue
    if( GetUservalueField( L ) )
      return 1;

    // search metatable
    if( GetMetafield( L ) )
      return 1;
    else
      return luaL_error( L, "'%s' object has no field '%s'", ID,
                         luaL_tolstring(L, 2, nullptr) );
  }
}

//////////////
// Implementation of __newindex
//   ID: ID of userdata
//
// Add whatever key and value to uservalue, except the reserved key.
/////////////////////////////////////////////////////////
int LuaUtil::NewIndex( lua_State* L, const char* ID )
{
  // stack: userdata, key, value
  CheckArgs( L, 3 );
  CheckUserdata( L, 1, ID );

  // modifying the reserved field is not allowed
  if( IsReserved(L, 2) )
    return luaL_error( L, "'%s' of '%s' object is a read-only field", ReservedKey, ID );

  // add to uservalue
  lua_getuservalue( L, 1 );
  lua_insert( L, 2 );
  lua_rawset( L, 2 );

  return 0;
}
