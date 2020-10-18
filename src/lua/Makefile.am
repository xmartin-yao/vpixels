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

## Makefile.am for src/lua/

## for CMake build
EXTRA_DIST = CMakeLists.txt

## Build Lua module, if Lua is installed and coverage check is disabled
## (link errors, when coverage check is enabled)
if BUILD_LUA

luaexec_LTLIBRARIES = vpixels.la

vpixels_la_SOURCES = LuaModule.cpp LuaUtil.h LuaUtil.cpp \
                     LuaBmp.h LuaBmp.cpp LuaDerive.h LuaDerive.cpp \
                     LuaGifDefs.h LuaGif.h LuaGif.cpp LuaGifImage.h LuaGifImage.cpp

## shared: build shared lib
## module: name will not be prefixed with 'lib'
## Paths to Lua header and Lua lib are set to CPPFLAGS and LDFLAGS in configure.ac.
## Lua lib is set to LIBS in configure.ac.
AM_CPPFLAGS = -shared -I@top_srcdir@/include/vp -I@top_srcdir@/src/util
AM_LDFLAGS = -shared -module -no-undefined -avoid-version @top_builddir@/src/libvpixels.la

else  # BUILD_LUA

## show notice
all-local:
	@echo ============================
	@echo Lua extensions build skipped
	@echo ============================

endif  # BUILD_LUA
