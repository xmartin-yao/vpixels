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

# Get version number from file 'VP_VERSION' in root directory.
# The following variables get set:
#   PROJECT_VERSION        <PROJECT-NAME>_VERSION
#   PROJECT_VERSION_MAJOR  <PROJECT-NAME>_VERSION_MAJOR
#   PROJECT_VERSION_MINOR  <PROJECT-NAME>_VERSION_MINOR
#   PROJECT_VERSION_PATCH  <PROJECT-NAME>_VERSION_PATCH
#   PROJECT_VERSION_TWEAK  <PROJECT-NAME>_VERSION_TWEAK

# set PROJECT_VERSION from the 1st line of the file
file(STRINGS "VP_VERSION" PROJECT_VERSION LIMIT_COUNT 1)

# set PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH
string(REGEX MATCHALL "[0-9]+" _VERSION_LIST "${PROJECT_VERSION}")
list(GET _VERSION_LIST 0 PROJECT_VERSION_MAJOR)
list(GET _VERSION_LIST 1 PROJECT_VERSION_MINOR)
list(GET _VERSION_LIST 2 PROJECT_VERSION_PATCH)

# set <PROJECT-NAME>_VERSION <PROJECT-NAME>_VERSION_MAJOR
#     <PROJECT-NAME>_VERSION_MINOR <PROJECT-NAME>_VERSION_PATCH
set(${CMAKE_PROJECT_NAME}_VERSION ${PROJECT_VERSION})
set(${CMAKE_PROJECT_NAME}_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(${CMAKE_PROJECT_NAME}_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(${CMAKE_PROJECT_NAME}_VERSION_PATCH ${PROJECT_VERSION_PATCH})
