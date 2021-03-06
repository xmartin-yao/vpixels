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

#ifndef LuaUtil_h
#define LuaUtil_h

#include <cstdint>

////////////////////////
namespace LuaUtil
{
  // number of arguments
  int CheckArgs( lua_State* L, int args, int opts = 0 );

  // uint8_t argument
  uint8_t CheckUint8( lua_State* L, int arg );

  // uint16_t argument
  uint16_t CheckUint16( lua_State* L, int arg );
  uint16_t OptUint16( lua_State* L, int arg, uint16_t def );

  // bool argument
  bool CheckBoolean( lua_State* L, int arg );
  bool OptBoolean( lua_State* L, int arg, bool def );

  // check value >= lower
  void CheckValueLower( lua_State* L, int arg, uint16_t value, uint16_t lower );

  // check value < upper
  void CheckValueUpper( lua_State* L, int arg, uint16_t value, uint16_t upper );

  // check value within [lower, upper]
  void CheckValueRange( lua_State* L, int arg, lua_Number value, uint16_t lower,
                        uint16_t upper );

  // check value within [lower, upper)
  void CheckValueRangeEx( lua_State* L, int arg, uint16_t value, uint16_t lower, 
                          uint16_t upper );


  ////////////////////////////
  // The following functions are to make a full userdata extendable.
  //
  // The idea is to create and associate a table to an instance of
  // a userdata type, therefore new methods and data can be added
  // to this table, in turn, the functionality of the userdata type
  // is extended or customized.
  //////////////////////////////////////////////////////////////////////

  // Make a userdata type extendable
  void Extend( lua_State* L, int Index );

  // Implementation of __index
  int Indexing( lua_State* L, const char* ID );

  // Implementation of __newindex
  int NewIndex( lua_State* L, const char* ID );
}

#endif //LuaUtil_h
