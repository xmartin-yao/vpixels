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

# build libvpixels for Lua and Python extension module
message(STATUS "Configuring src/")

# source files
set(VPIXELS_SRCS bmp/BmpInfo.cpp bmp/BmpInfo1Bit.cpp bmp/BmpInfo4Bit.cpp
                 bmp/BmpInfo8Bit.cpp bmp/BmpInfo24Bit.cpp bmp/BmpFileHeader.cpp
                 bmp/BmpFileHeader.cpp bmp/BmpInfoHeader.cpp bmp/BmpColorTable.cpp
                 bmp/BmpImageData.cpp bmp/BmpImpl.cpp bmp/Bmp.cpp
                 gif/GifCodeReader.cpp gif/GifCodeWriter.cpp gif/GifStringTable.cpp
                 gif/GifDecoder.cpp gif/GifEncoder.cpp gif/GifHeader.cpp
                 gif/GifScreenDescriptor.cpp gif/GifColorTable.cpp gif/GifComponent.cpp
                 gif/GifGraphicsControlExt.cpp gif/GifImageDescriptor.cpp
                 gif/GifImageData.cpp gif/GifApplicationExt.cpp gif/GifCommentExt.cpp
                 gif/GifPlainTextExt.cpp gif/GifComponentVecUtil.cpp gif/GifImageVecBuilder.cpp
                 gif/GifImageImpl.cpp gif/GifImpl.cpp gif/GifImage.cpp gif/Gif.cpp
                 util/Exception.cpp)

#
# target: vpixels-lib
#
add_library(vpixels-lib STATIC EXCLUDE_FROM_ALL ${VPIXELS_SRCS} )

# name it after the project
set_target_properties(vpixels-lib PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# define VP_EXTENSION to exclude some verifications,
# as they will be done in Lua and Python bindings
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_target_properties(vpixels-lib PROPERTIES COMPILE_FLAGS "-DVP_EXTENSION")
endif()

# static link to MSVC runtime: /MT
if(MSVC)
  set_target_properties(vpixels-lib PROPERTIES COMPILE_FLAGS "/MT /DVP_EXTENSION")
endif()
