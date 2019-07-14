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

## Makefile.am for directory src/py/

## for CMake build
EXTRA_DIST = CMakeLists.txt

## Build Python module, if Python is installed and coverage check is disabled
if BUILD_PY

pyexec_LTLIBRARIES = vpixels.la

vpixels_la_SOURCES = PyModule.cpp PyUtil.h \
                     PyBmp.h PyBmp.cpp \
                     PyGif.h PyGif.cpp PyGifImage.h PyGifImage.cpp

## shared: build shared lib
## module: name will not be prefixed with 'lib'
## fno-strict-aliasing: suppress warning on Py_INCREF
AM_CPPFLAGS = -shared -fno-strict-aliasing -I@top_srcdir@/include/vp \
              -I@top_srcdir@/src/util `python-config --cflags`
AM_LDFLAGS = -shared -module -no-undefined -avoid-version \
             @top_builddir@/src/libvpixels.la `python-config --libs`

## shrext: specify lib name extension
##         use '.pyd', instead of '.dll', as name extension
if OS_MINGW
  AM_LDFLAGS +=-shrext .pyd
endif

else  # BUILD_PY

## show notice
all-local:
	@echo =============================
	@echo  Python module build skipped
	@echo =============================

endif  # BUILD_PY
