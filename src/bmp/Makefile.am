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

## Makefile.am for directory src/bmp/

## for CMake build
EXTRA_DIST = CMakeLists.txt

## lib to be installed
lib_LIBRARIES = libvpbmp.a

libvpbmp_a_SOURCES = BmpInfo.h BmpInfo.cpp \
                     BmpInfo1Bit.h BmpInfo1Bit.cpp \
                     BmpInfo4Bit.h BmpInfo4Bit.cpp \
                     BmpInfo8Bit.h BmpInfo8Bit.cpp \
                     BmpInfo24Bit.h BmpInfo24Bit.cpp \
                     BmpFileHeader.h BmpFileHeader.cpp \
                     BmpInfoHeader.h BmpInfoHeader.cpp \
                     BmpColorTable.h BmpColorTable.cpp \
                     BmpImageData.h BmpImageData.cpp \
                     BmpImpl.h BmpImpl.cpp Bmp.cpp \
                     @top_srcdir@/src/util/Exception.cpp

## include path
AM_CXXFLAGS = -I$(top_srcdir)/include/vp -I$(top_srcdir)/src/util

## if code coverage check is enabled (using configure --enable-gcov)
if CHECK_COVERAGE

## clean up compiler generated files
clean-local:
	$(RM) *.gcno *.gcda

endif # CHECK_COVERAGE
