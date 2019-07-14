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

# build libvpbmp
message(STATUS "Configuring src/bmp/")

# bmp source files
set(BMP_SRCS BmpInfo.cpp BmpInfo1Bit.cpp BmpInfo4Bit.cpp BmpInfo8Bit.cpp
             BmpInfo24Bit.cpp BmpFileHeader.cpp BmpInfoHeader.cpp
             BmpColorTable.cpp BmpImageData.cpp BmpImpl.cpp Bmp.cpp
             ${PROJECT_SOURCE_DIR}/src/util/Exception.cpp)

#
# target: vpbmp
#
add_library(vpbmp STATIC ${BMP_SRCS} )
