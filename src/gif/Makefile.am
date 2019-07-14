## Copyright (C) 2019 Xueyi Yao
## 
## This file is part of VPixels.
## 
## VPixels is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## VPixels is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with VPixels.  If not, see <https://www.gnu.org/licenses/>.

## Makefile.am for directory src/gif/

## for CMake build
EXTRA_DIST = CMakeLists.txt

## lib to be installed
lib_LIBRARIES = libvpgif.a

libvpgif_a_SOURCES = U8String.h GifBlockIO.h VecDefs.h \
                     GifCodeReader.h GifCodeReader.cpp \
                     GifCodeWriter.h GifCodeWriter.cpp \
                     GifStringTable.h GifStringTable.cpp \
                     GifDecoder.h GifDecoder.cpp \
                     GifEncoder.h GifEncoder.cpp \
                     GifHeader.h GifHeader.cpp \
                     GifScreenDescriptor.h GifScreenDescriptor.cpp \
                     GifColorTable.h GifColorTable.cpp \
                     GifComponent.h GifComponent.cpp \
                     GifExtension.h \
                     GifGraphicsControlExt.h GifGraphicsControlExt.cpp \
                     GifImageDescriptor.h GifImageDescriptor.cpp \
                     GifImageData.h GifImageData.cpp \
                     GifApplicationExt.h GifApplicationExt.cpp \
                     GifCommentExt.h GifCommentExt.cpp \
                     GifPlainTextExt.h GifPlainTextExt.cpp \
                     GifComponentVecUtil.h GifComponentVecUtil.cpp \
                     GifImageVecBuilder.h GifImageVecBuilder.cpp \
                     GifImageImpl.h GifImageImpl.cpp \
                     GifImpl.h GifImpl.cpp \
                     GifImage.cpp Gif.cpp \
                     @top_srcdir@/src/util/Exception.cpp

## include path
AM_CXXFLAGS = -I$(top_srcdir)/include/vp -I$(top_srcdir)/src/util

## if code coverage check is enabled (using configure --enable-gcov)
if CHECK_COVERAGE

## clean up compiler generated files
clean-local:
	$(RM) *.gcno *.gcda

endif  # CHECK_COVERAGE
