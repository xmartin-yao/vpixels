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
#include "LuaUtil.h"
#include "config.h"

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
