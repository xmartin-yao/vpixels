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

# build C++ examples
message(STATUS "Configuring example/cpp/")

# path to include/
include_directories(${PROJECT_SOURCE_DIR}/include)

#
# target: bmpinfo
#
add_executable(bmpinfo BmpInfo.cpp)

# link to libvpbmp
set_target_properties(bmpinfo PROPERTIES LINK_LIBRARIES vpbmp)

#
# target: gifinfo
#
add_executable(gifinfo GifInfo.cpp)

#
# target: gifimages
#
add_executable(gifimages GifImages.cpp)

# link to libvpgif
set_target_properties(gifinfo gifimages PROPERTIES LINK_LIBRARIES vpgif)

#
# target: gifdownsize
#
add_executable(gifdownsize GifDownsize.cpp)

# link to libvpgif and libvpbmp
set_target_properties(gifdownsize PROPERTIES LINK_LIBRARIES "vpgif;vpbmp")

# static link to libstdc++ and libgcc
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_target_properties(bmpinfo gifinfo gifimages gifdownsize PROPERTIES LINK_FLAGS
                        "-static-libstdc++ -static-libgcc")
endif()
