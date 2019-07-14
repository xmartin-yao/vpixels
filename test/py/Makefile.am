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

## Makefile.am for test/py/

EXTRA_DIST = BmpTest.py GifTest.py CMakeLists.txt

## Python test scripts
SCRIPT_LIST = BmpTest.py GifTest.py

## Python modules to be tested
if OS_MINGW
  MODULE_LIST = vpixels.pyd
else
  MODULE_LIST = vpixels.so
endif

## test Python modules,
## if Python is installed and coverage check is disabled
if TEST_PY

## run Python scripts
check-local: copy-scripts copy-modules
	@echo ===============================================
	@list='$(SCRIPT_LIST)'; \
	for file in $$list; do \
	  echo "run" $$file; \
	  python$(EXEEXT) $$file; \
	done
	@echo ===============================================

else  # TEST_PY

## show notice
check-local:
	@echo ============================
	@echo  Python module test skipped
	@echo ============================

endif  # TEST_PY

## target of "make clean"
clean-local: remove-scripts remove-modules remove-tmp-files

## copy Python extension module from src/py,
## if it doesn't exist or older than the one in src/py
copy-modules:
	@list='$(MODULE_LIST)'; \
	for file in $$list; do \
	  if test ! -f ./$$file || test ./$$file -ot $(top_builddir)/src/py/.libs/$$file; then \
	    echo "copy" $$file; \
	    cp -p $(top_builddir)/src/py/.libs/$$file . ; \
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

## copy Python scripts, if build tree is different than source tree,
## and if it doesn't exist or older than the one in source tree
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

## remove Python scripts, if build tree is different than source tree,
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
