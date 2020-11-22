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

# build Python extension module
message(STATUS "Configuring src/py/")
if(NOT ${PYTHONLIBS_FOUND} )
  message(STATUS "Python extension module build skipped")
  return()
endif()

# suppress warning on Py_INCREF
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions(-fno-strict-aliasing)
endif()

# Python needs _hypot defined on MinGW
if(MSYS OR MINGW)
  add_definitions(-D_hypot=hypot)
endif()

# path to Python header
include_directories(${PYTHON_INCLUDE_DIRS})

#
# target: vpixels-py
#
add_library(vpixels-py MODULE PyModule.cpp PyBmp.cpp PyGif.cpp PyGifImage.cpp)

# libs to link
target_link_libraries(vpixels-py vpixels ${PYTHON_LIBRARIES})

# name it 'vpixels'
set_target_properties(vpixels-py PROPERTIES OUTPUT_NAME "vpixels")

# don't use 'lib' as prefix
set_target_properties(vpixels-py PROPERTIES PREFIX "")

# use suffix '.pyd', instead of '.dll'
if(MSYS OR MINGW OR MSVC)
  set_target_properties(vpixels-py PROPERTIES SUFFIX ".pyd")
endif()

# static link libstdc++ and libgcc
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_target_properties(vpixels-py PROPERTIES LINK_FLAGS
                        "-static-libstdc++ -static-libgcc")
endif()

# static link to MSVC runtime
if(MSVC)
  set_target_properties(vpixels-py PROPERTIES COMPILE_FLAGS "/MT")
endif()

#
# target: pack-py
#         pack the extension module into a wheel file
#

# configure setup.py.in
configure_file(${PROJECT_SOURCE_DIR}/cmake/setup.py.in setup.py)

# build a wheel file
# when using MSVC, the extension module needs to be copied to current directory
add_custom_target(pack-py
                  DEPENDS vpixels-py
                  COMMENT "Pack Python extension module"
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          $<TARGET_FILE:vpixels-py> $<TARGET_FILE_NAME:vpixels-py>
                  COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel)

# for 'make clean'
list(APPEND CLEAN_LIST build/ dist/ vpixels.egg-info/)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_LIST}")
