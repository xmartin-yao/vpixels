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

# build libvpgif
message(STATUS "Configuring src/gif/")

# gif source files
set(GIF_SRCS GifCodeReader.cpp GifCodeWriter.cpp GifStringTable.cpp GifDecoder.cpp
             GifEncoder.cpp GifHeader.cpp GifScreenDescriptor.cpp GifColorTable.cpp
             GifComponent.cpp GifGraphicsControlExt.cpp GifImageDescriptor.cpp
             GifImageData.cpp GifApplicationExt.cpp GifCommentExt.cpp
             GifPlainTextExt.cpp GifComponentVecUtil.cpp GifImageVecBuilder.cpp
             GifImageImpl.cpp GifImpl.cpp GifImage.cpp Gif.cpp
             ${PROJECT_SOURCE_DIR}/src/util/Exception.cpp)

#
# target: vpgif
#
add_library(vpgif STATIC ${GIF_SRCS})
