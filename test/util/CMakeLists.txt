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

# test utils
message(STATUS "Configuring test/util/")
if(NOT ${CPPUNIT_FOUND})
  message(STATUS "libvpgif tests skipped")
  return()
endif()

# path to CppUnit lib
link_directories(${CPPUNIT_LIBRARY_DIRS})

#
# target: UtilTest, build tests
#
add_executable(UtilTest EXCLUDE_FROM_ALL
               IOutilTest.cpp SimpleListTest.cpp
               ${PROJECT_SOURCE_DIR}/test/UnitTestMain.cpp)

target_compile_options(UtilTest PUBLIC ${CPPUNIT_CFLAGS})
target_link_libraries(UtilTest ${CPPUNIT_LIBRARIES})

#
# target: check-util, run tests
#
add_custom_target(check-util DEPENDS UtilTest
                  COMMENT "Util tests"
                  COMMAND UtilTest) # use target name to invoke it, do not need path
