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

# CMake build for vpixels
cmake_minimum_required(VERSION 3.5)

project(vpixels LANGUAGES CXX)

# set version number
include(${PROJECT_SOURCE_DIR}/cmake/SetVersionNumber.cmake)

message(STATUS "Configuring ${CMAKE_PROJECT_NAME} ${PROJECT_VERSION}")

# set email address
set(PROJECT_BUGREPORT "xmartinyao@gmail.com")

# Lua executable
include(${PROJECT_SOURCE_DIR}/cmake/FindLuaExe.cmake)
FindLuaExe(5.2)
#   LUA_EXE_FOUND
#   LUA_EXE_VERSION_STRING
#   LUA_EXECUTABLE  full path to the executable

# Lua lib
find_package(Lua 5.2 QUIET)
#   LUA_FOUND
#   LUA_LIBRARIES  full path to the lib
#   LUA_INCLUDE_DIR
#   LUA_VERSION_STRING

# Python executable
find_package(PythonInterp 2.7 QUIET)
#   PYTHONINTERP_FOUND
#   PYTHON_EXECUTABLE  full path to the executable
#   PYTHON_VERSION_STRING 

# Python lib
find_package(PythonLibs 2.7 QUIET)
#   PYTHONLIBS_FOUND
#   PYTHON_LIBRARIES  full path to the lib
#   PYTHON_INCLUDE_DIRS
#   PYTHONLIBS_VERSION_STRING

# CppUnit
find_package(PkgConfig QUIET)  # find pkg-config first
if(${PKG_CONFIG_FOUND})
  pkg_check_modules(CPPUNIT QUIET cppunit)
else()
  set(CPPUNIT_FOUND FALSE)
endif()
#   CPPUNIT_FOUND
#   CPPUNIT_VERSION
#   CPPUNIT_LIBRARY_DIRS
#   CPPUNIT_CFLAGS
#   CPPUNIT_LIBRARIES

# instead of filename.cpp.obj, generating filename.obj
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

# flags for g++
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -O2")
endif()

# release build only for VC++
if(MSVC)
  set(CMAKE_CONFIGURATION_TYPES "Release")
  set(CMKAE_BUILD_TYPE "Release")
endif()

# local include directory
include_directories(${PROJECT_SOURCE_DIR}/include/vp
                    ${PROJECT_SOURCE_DIR}/src/util
                    ${PROJECT_BINARY_DIR})  # for config.h

# configure config.h
configure_file(${PROJECT_SOURCE_DIR}/cmake/config.h.in config.h)

# sub-directories
add_subdirectory(src)
add_subdirectory(src/bmp)
add_subdirectory(src/gif)
add_subdirectory(src/lua)
add_subdirectory(src/py)
add_subdirectory(example/cpp)
add_subdirectory(test/bmp)
add_subdirectory(test/gif)
add_subdirectory(test/util)
add_subdirectory(test/lua)
add_subdirectory(test/py)

#
# target: check, run all tests
#
add_custom_target(check COMMENT "All tests passed"
                  DEPENDS check-bmp check-gif check-util check-lua check-py)

#
# waring messages
#
if(NOT ${LUA_EXE_FOUND} )
  message("=========== Warning ==========")
  message("Failed to find Lua interpreter")
  message("Unable to test Lua extensions")
  message("==============================")
endif()

if(NOT ${LUA_FOUND} )
  message("=========== Warning ==========")
  message("Failed to find Lua libs")
  message("Unable to build Lua extensions")
  message("==============================")
endif()

if(NOT ${PYTHONINTERP_FOUND} )
  message("============ Warning ============")
  message("Failed to find Python interpreter")
  message("Unable to test Python extensions")
  message("====================================")
endif()

if(NOT ${PYTHONLIBS_FOUND} )
  message("============ Warning ============")
  message("Failed to find Python libs")
  message("Unable to build Python extensions")
  message("=================================")
endif()

if(NOT ${CPPUNIT_FOUND} )
  message("======== Warning =========")
  message("Failed to find CppUnit")
  message("Unable to build unit tests")
  message("==========================")
endif()
