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

# test Python module
message(STATUS "Configuring test/py/")
if(NOT (${PYTHONINTERP_FOUND} AND ${PYTHONLIBS_FOUND}))
  message(STATUS "Python extension module tests skipped")
  return()
endif()

#
# target: check-py, run Python module tests
#
add_custom_target(check-py DEPENDS check-bmp-py check-gif-py)

#
# target: check-bmp-py
#
add_custom_target(check-bmp-py DEPENDS pre-bmp-py
                  COMMENT "Python BMP tests"
                  COMMAND ${PYTHON_EXECUTABLE} BmpTest.py)

#
# target: pre-bmp-py
# generator expressions make it target depend on target vpixels-py
#
add_custom_target(pre-bmp-py
                  COMMENT "Prepare Python BMP tests"
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          $<TARGET_FILE:vpixels-py> $<TARGET_FILE_NAME:vpixels-py>
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${CMAKE_CURRENT_SOURCE_DIR}/BmpTest.py BmpTest.py)

#
# target: check-gif-py
#
add_custom_target(check-gif-py DEPENDS pre-gif-py
                  COMMENT "Python GIF tests"
                  COMMAND ${PYTHON_EXECUTABLE} GifTest.py)

#
# target: pre-gif-py
# generator expressions make it depend on target vpixels-py
#
add_custom_target(pre-gif-py
                  COMMENT "Prepare Python GIF tests"
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          $<TARGET_FILE:vpixels-py> $<TARGET_FILE_NAME:vpixels-py>
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different
                          ${CMAKE_CURRENT_SOURCE_DIR}/GifTest.py GifTest.py)

#
# for 'make clean'
#
list(APPEND CLEAN_LIST BmpTest.py GifTest.py temp.bmp temp.gif)
list(APPEND CLEAN_LIST $<TARGET_FILE_NAME:vpixels-py>)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_LIST}")
