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

## Makefile.am for project root directory

ACLOCAL_AMFLAGS = -I m4

## build 'src' before 'test'
SUBDIRS = include src test example cmake

EXTRA_DIST = CMakeLists.txt VP_VERSION README.md

## Switch README to markdown version
## Add this rule, automake won't complain missing README file.
## 'make README' or 'make dist' will create a dummy README file.
README:
	@if test ! -f $@; then \
	  echo "Please see $@.md" > $@; \
	fi

## clean up dummy README file
CLEANFILES = README
