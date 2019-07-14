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

## Makefile.am for example/gif

## for CMake build
EXTRA_DIST = CMakeLists.txt

## non-install executables
noinst_PROGRAMS = gifinfo gifimages

gifinfo_SOURCES = GifInfo.cpp
gifimages_SOURCES = GifImages.cpp

## includes, flags and libs
AM_CXXFLAGS = -I@top_srcdir@/include/
AM_LDFLAGS  = -L@top_builddir@/src/gif/
LIBS = -lvpgif
