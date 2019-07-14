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

# test Lua module
message(STATUS "Configuring test/lua/")
if(NOT (${LUA_EXE_FOUND} AND ${LUA_FOUND}))
  message(STATUS "Lua extension module tests skipped")
  return()
endif()

#
# target: check-lua, run Lua module tests
#
add_custom_target(check-lua DEPENDS check-bmp-lua check-gif-lua check-derive-lua)

#
# target: check-bmp-lua
#
add_custom_target(check-bmp-lua DEPENDS pre-bmp-lua
                  COMMENT "Lua BMP tests"
                  COMMAND ${LUA_EXECUTABLE} BmpTest.lua)

#
# target: pre-bmp-lua
# generator expressions make it depend on target vpixels-lua
#
add_custom_target(pre-bmp-lua
                  COMMENT "Prepare Lua BMP tests"
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          $<TARGET_FILE:vpixels-lua> $<TARGET_FILE_NAME:vpixels-lua>
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${CMAKE_CURRENT_SOURCE_DIR}/BmpTest.lua BmpTest.lua)

#
# target: check-gif-lua
#
add_custom_target(check-gif-lua DEPENDS pre-gif-lua
                  COMMENT "Lua GIF tests"
                  COMMAND ${LUA_EXECUTABLE} GifTest.lua)

#
# target: pre-gif-lua
#
add_custom_target(pre-gif-lua
                  COMMENT "Prepare Lua GIF tests"
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          $<TARGET_FILE:vpixels-lua> $<TARGET_FILE_NAME:vpixels-lua>
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${CMAKE_CURRENT_SOURCE_DIR}/GifTest.lua GifTest.lua)

#
# target: check-derive-lua
#
add_custom_target(check-derive-lua DEPENDS pre-derive-lua
                  COMMENT "Lua Derive tests"
                  COMMAND ${LUA_EXECUTABLE} DeriveTest.lua)

#
# target: pre-derive-lua
#
add_custom_target(pre-derive-lua
                  COMMENT "Prepare Lua Derive tests"
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          $<TARGET_FILE:vpixels-lua> $<TARGET_FILE_NAME:vpixels-lua>
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${CMAKE_CURRENT_SOURCE_DIR}/DeriveTest.lua DeriveTest.lua)

#
# for 'make clean'
#
list(APPEND CLEAN_LIST BmpTest.lua GifTest.lua DeriveTest.lua temp.bmp temp.gif)
list(APPEND CLEAN_LIST $<TARGET_FILE_NAME:vpixels-lua>)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_LIST}")
