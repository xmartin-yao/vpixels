#######################################################################
# Copyright (C) 2019 Xueyi Yao
#
# This file is part of VPixels.
#
# VPixels is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# VPixels is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
#######################################################################

# build Lua extension module
message(STATUS "Configuring src/lua/")
if(NOT ${LUA_FOUND} )
  message(STATUS "Lua extension module build skipped")
  return()
endif()

# path to Lua header
include_directories(${LUA_INCLUDE_DIR})

#
# target: vpixels-lua
#
add_library(vpixels-lua MODULE LuaModule.cpp LuaBmp.cpp LuaGif.cpp
                               LuaGifImage.cpp LuaDerive.cpp LuaUtil.cpp)

# libs to link
target_link_libraries(vpixels-lua vpixels-lib ${LUA_LIBRARIES})

# name it after the project
set_target_properties(vpixels-lua PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# don't use 'lib' as a prefix 
set_target_properties(vpixels-lua PROPERTIES PREFIX "")

# static link to libstdc++ and libgcc
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_target_properties(vpixels-lua PROPERTIES LINK_FLAGS
                        "-static-libstdc++ -static-libgcc")
endif()

# static link to MSVC runtime
if(MSVC)
  set_target_properties(vpixels-lua PROPERTIES COMPILE_FLAGS "/MT")
endif()
