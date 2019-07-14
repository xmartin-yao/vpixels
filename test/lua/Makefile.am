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

## Makefile.am for test/lua/

EXTRA_DIST = BmpTest.lua GifTest.lua DeriveTest.lua CMakeLists.txt

## Lua test scripts
SCRIPT_LIST = BmpTest.lua GifTest.lua DeriveTest.lua

## Lua extension module to be tested
if OS_MINGW
  MODULE_LIST = vpixels.dll
else
  MODULE_LIST = vpixels.so
endif

## test Lua modules,
## if Lua is installed and coverage check is disabled
if TEST_LUA

## run Lua scripts
check-local: copy-scripts copy-modules
	@echo ===============================================
	@list='$(SCRIPT_LIST)'; \
	for file in $$list; do \
	  echo "run" $$file; \
	  lua$(EXEEXT) $$file; \
	done
	@echo ===============================================

else  # TEST_LUA

## show notice
check-local:
	@echo =========================
	@echo  Lua module test skipped
	@echo =========================

endif  # TEST_LUA

## target of "make clean"
clean-local: remove-scripts remove-modules remove-tmp-files

## copy Lua extension module from src/lua,
## if it doesn't exist or older than the one in src/lua
copy-modules:
	@list='$(MODULE_LIST)'; \
	for file in $$list; do \
	  if test ! -f ./$$file || test ./$$file -ot $(top_builddir)/src/lua/.libs/$$file; then \
	    echo "copy" $$file; \
	    cp -p $(top_builddir)/src/lua/.libs/$$file . ; \
	  fi \
	done

remove-modules:
	@list='$(MODULE_LIST)'; \
	for file in $$list; do \
	  if test -f ./$$file; then \
	    echo "remove" $$file; \
	    rm -f ./$$file; \
	  fi \
	done

## copy Lua scripts, if build tree is different than source tree,
## if it doesn't exist or older than the one in source tree
copy-scripts:
	@if test "$(top_srcdir)" != "$(top_builddir)"; then \
	  list='$(SCRIPT_LIST)'; \
	  for file in $$list; do \
	    if test ! -f ./$$file || test ./$$file -ot $(srcdir)/$$file; then \
	      echo "copy" $$file; \
	      cp -p $(srcdir)/$$file . ; \
	    fi \
	  done \
	fi

## remove Lua scripts, if build tree is different than source tree,
remove-scripts:
	@if test "$(top_srcdir)" != "$(top_builddir)"; then \
	  list='$(SCRIPT_LIST)'; \
	  for file in $$list; do \
	    if test -f ./$$file; then \
	      echo "remove" $$file; \
	      rm -f ./$$file; \
	    fi \
	  done \
	fi

## remove generated files
remove-tmp-files:
	@$(RM) *.bmp *.gif

## targets defined in this file
.PHONY: copy-scripts remove-scripts copy-modules remove-modules remove-tmp-files
