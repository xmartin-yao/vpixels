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
#include "LuaGif.h"
#include "LuaGifImage.h"
#include "LuaDerive.h"
#include "LuaUtil.h"
#include "Util.h"
#include "config.h"
#include <string>
#include <iostream>

#ifdef _MSC_VER  // for VC++
#define VP_MODULE_INIT extern "C" __declspec(dllexport)
#else  // for g++
#define VP_MODULE_INIT extern "C"
#endif

///////////////////////
namespace
{
  const std::string Notice{ PACKAGE_STRING "\n"
                            "Copyright (C) 2019 Xueyi Yao\n"
                            "License: GNU GPL version 3 or later\n"
                            "Bug report: " PACKAGE_BUGREPORT "\n"
                            "[" LUA_RELEASE ", " COMPILER_STRING ", " __DATE__ ", " __TIME__ "]" };

  inline void ShowNotice() 
  {
    std::cout << Notice << std::endl;
  }

  // verify the module's version
  int CheckVersion( lua_State* L )
  {
    luaL_checktype( L, 1, LUA_TSTRING );
    auto ReqVersion = lua_tostring(L, 1);
    auto Exact = LuaUtil::OptBoolean( L, 2, false );

    auto Result = Util::CheckVersion( ReqVersion, PACKAGE_VERSION, Exact );
    lua_pushboolean( L, Result );
    if( Result )
      return 1;
    else
    {
      // include a message, so it can used by assert()
      lua_pushfstring( L, "require version %s, got %s", ReqVersion, PACKAGE_VERSION );
      return 2;
    }
  }

  // return or verify the module's version
  int Version( lua_State* L )
  {
    auto argc = LuaUtil::CheckArgs( L, 0, 2 );
    if( argc == 0 )
    {
      lua_pushstring( L, PACKAGE_VERSION );
      return 1;
    }
    else
      return CheckVersion( L );
  }

  // about the module
  int About( lua_State* L ) 
  {
    LuaUtil::CheckArgs( L, 0 );
    ShowNotice();
    return 0;
  }

  // __call of the module
  int Call( lua_State* ) 
  {
    ShowNotice();
    return 0;
  }

  // __tostring of the module
  int ToString( lua_State* L ) 
  {
    lua_pushstring( L, Notice.c_str() );
    return 1;
  }

  // methods of the module
  const luaL_Reg Methods[] = {
    { "bmp",        LuaBmp::New },
    { "gif",        LuaGif::New },
    { "derive",     LuaDerive::New },
    { "version",    Version },
    { "about",      About },
    { "__call",     Call },
    { "__tostring", ToString },
    { nullptr, nullptr }
  };
}

////////////////////////////////
VP_MODULE_INIT int luaopen_vpixels( lua_State *L )
{
  // register meta tables
  LuaBmp::Register( L );
  LuaGif::Register( L );
  LuaGifImage::Register( L );
  LuaDerive::Register( L );

  // new lib
  luaL_newlib( L, Methods );

  // set lib itself as its metatable
  lua_pushvalue( L, -1 );
  lua_setmetatable( L, -2 );

  return 1;
}
