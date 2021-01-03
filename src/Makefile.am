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

## Makefile.am for src/

## build '.' before 'lua' and 'py'
SUBDIRS = bmp gif . lua py util

## for CMake build
EXTRA_DIST = CMakeLists.txt

## non-install lib for Lua and Python extension module
noinst_LTLIBRARIES = libvpixels.la

libvpixels_la_SOURCES = bmp/BmpInfo.cpp bmp/BmpInfo1Bit.cpp \
                        bmp/BmpInfo4Bit.cpp bmp/BmpInfo8Bit.cpp \
                        bmp/BmpInfo24Bit.cpp bmp/BmpFileHeader.cpp \
                        bmp/BmpInfoHeader.cpp bmp/BmpColorTable.cpp \
                        bmp/BmpImageData.cpp bmp/BmpImpl.cpp bmp/Bmp.cpp \
                        gif/GifCodeReader.cpp gif/GifCodeWriter.cpp \
                        gif/GifStringTable.cpp gif/GifDecoder.cpp \
                        gif/GifEncoder.cpp gif/GifHeader.cpp \
                        gif/GifScreenDescriptor.cpp gif/GifColorTable.cpp \
                        gif/GifComponent.cpp gif/GifGraphicsControlExt.cpp \
                        gif/GifImageDescriptor.cpp gif/GifImageData.cpp \
                        gif/GifApplicationExt.cpp gif/GifCommentExt.cpp \
                        gif/GifPlainTextExt.cpp gif/GifComponentVecUtil.cpp \
                        gif/GifImageVecBuilder.cpp \
                        gif/GifImageImpl.cpp gif/GifImage.cpp \
                        gif/GifImpl.cpp gif/Gif.cpp \
                        util/Exception.cpp util/Util.cpp

## shared: build shared lib
## VP_EXTENSION: define VP_EXTENSION to exclude some of the verifications
##               that will be done in Lua and Python wrappers.
AM_CPPFLAGS = -shared -I@top_srcdir@/include/vp -I@top_srcdir@/src/util \
              -DVP_EXTENSION
AM_LDFLAGS = -shared -avoid-version
