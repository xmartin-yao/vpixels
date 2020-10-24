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

AC_PREREQ(2.68)
AC_INIT([vpixels], [m4_esyscmd_s([head -1 VP_VERSION])], [xmartinyao@gmail.com])
AC_CONFIG_AUX_DIR([admin])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([VP_VERSION])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([-Wall])
LT_INIT([win32-dll])

dnl enable silent rules
AM_SILENT_RULES([yes])

# check for OS system
dnl support Linux and MinGW
os_linux=no
os_mingw=no
AC_CANONICAL_HOST
case "${host_os}" in
  *linux*)
    os_linux=yes
    ;;
  *mingw*)
    os_mingw=yes
    ;;
  *)
    AC_MSG_ERROR(["OS $host_os is not supported"])
    ;;
esac

# set OS_LINUX or OS_WINDOWS
AM_CONDITIONAL([OS_LINUX], [test x"$os_linux" = x"yes"])
AM_CONDITIONAL([OS_MINGW], [test x"$os_mingw" = x"yes"])

# Set CXXFLAGS, if not set by user
dnl Must be done before AC_PROG_CXX, which set it to "-O2 -g"
AS_IF([test -z "$CXXFLAGS"], [CXXFLAGS="-O2 -Wall -Wextra -Wsign-conversion -Wpedantic"])

# Set CPPFLAGS and LDFLAGS, if not set by user
dnl These are the default locations of Lua headers and Lua lib
AS_IF([test -z "$CPPFLAGS"], [CPPFLAGS="-I/usr/local/include"])
AS_IF([test -z "$LDFLAGS"],  [LDFLAGS="-L/usr/local/lib"])

# Checks for C++ compiler
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

# Checks for types
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_SIZE_T

# Checks for tools
AC_PROG_RANLIB
AC_PROG_INSTALL

# Check for CppUnit
dnl Call pkg-config, CPPUNIT_CFLAGS and CPPUNIT_LIBS will be used Makefile.am
PKG_CHECK_MODULES([CPPUNIT], [cppunit], [HAVE_CPPUNIT="yes"], [HAVE_CPPUNIT="no"])

# Check for Lua interpreter
AC_CHECK_PROG([HAVE_LUA_EXE], [lua$EXEEXT], yes, no)
AC_MSG_CHECKING([whether Lua version is 5.2])
AS_IF([test x"$HAVE_LUA_EXE" = x"yes"], [LUA_VERSION=`lua$EXEEXT -e "print(_VERSION)" | sed 's/Lua //g'`])
AS_IF([test x"$LUA_VERSION" = x"5.2"], [AC_MSG_RESULT([yes])],
      [HAVE_LUA_EXE="no"; AC_MSG_RESULT([no])])

# Check for luaunit
AS_IF([test x"$HAVE_LUA_EXE" = x"yes"],
      [AC_MSG_CHECKING([for luaunit])
       lua$EXEEXT -e "require('luaunit')" &>/dev/null && HAVE_LUA_UNIT="yes"
       AS_IF([test x"$HAVE_LUA_UNIT" = x"yes"], [AC_MSG_RESULT([yes])],
             [HAVE_LUA_UNIT="no"; AC_MSG_RESULT([no])])])

# Check for Lua header and lib
AS_IF([test x"$HAVE_LUA_EXE" = x"yes"],
      [AC_LANG_PUSH([C++])
       AC_CHECK_HEADER([lua.hpp], [HAVE_LUA_HPP="yes"], [HAVE_LUA_HPP="no"])
       AS_IF([test x"$os_mingw" = x"yes"], [LUA_LIB=lua52.dll], [LUA_LIB=lua])
       dnl LIBS gets set if Lua lib is found;
       dnl otherwise set "no" to HAVE_LUA_LIB
       AC_CHECK_LIB([$LUA_LIB], [luaL_setfuncs], ,[HAVE_LUA_LIB="no"])
       AC_LANG_POP])

# Install path for Lua extension module: luaexecdir
dnl If prefix is not set and the path to where Lua is installed is different
dnl from default prefix (i.e. /usr/local), use Lua installation path to set
dnl luaexecdir; otherwise, use prefix to set luaexecdir.
AS_IF([test x"$HAVE_LUA_HPP" = x"yes" && test x"$HAVE_LUA_LIB" != x"no"],
      [AC_MSG_CHECKING([for Lua extension module directory])
       LUA_EXEC_PREFIX='${exec_prefix}'
       dnl if prefix is not set
       AS_IF([test x"$prefix" = x"NONE"],
             [lua_install_prefix=`which lua$EXEEXT | sed "s/\/bin\/lua$EXEEXT//g"`
              dnl if Lua is installed in a location other than ac_default_prefix,
              dnl use the location to install Lua extension module
              AS_IF([test x"$lua_install_prefix" != x"$ac_default_prefix"],
                    [LUA_EXEC_PREFIX=$lua_install_prefix])])
       dnl set luaexecdir
       AS_IF([test x"$os_mingw" = x"yes"],
             [luaexecdir=$LUA_EXEC_PREFIX/bin],
             [luaexecdir=$LUA_EXEC_PREFIX/lib/lua/$LUA_VERSION])
       AC_SUBST([luaexecdir])
       AC_MSG_RESULT([$luaexecdir])
      ])

# Check for Python
dnl PYTHON, PYTHON_VERSION, pythondir and pyexecdir, etc get set
dnl PYTHON is the full path of Python interpreter
AM_PATH_PYTHON([2.7], [HAVE_PY_EXE="yes"], [HAVE_PY_EXE="no"])

# Check for Python version: 2.7
AC_MSG_CHECKING([whether Python version is 2.7])
AS_IF([test x"$PYTHON_VERSION" = x"2.7"], [AC_MSG_RESULT([yes])],
      [HAVE_PY_EXE="no"; AC_MSG_RESULT([no])])

# Check for Python unittest.py
AS_IF([test x"$HAVE_PY_EXE" = x"yes"],
      [AC_MSG_CHECKING([for python unittest])
       $PYTHON -c "import unittest" &>/dev/null && HAVE_PY_UNIT="yes"
       AS_IF([test x"$HAVE_PY_UNIT" = x"yes"], [AC_MSG_RESULT([yes])],
             [HAVE_PY_UNIT="no"; AC_MSG_RESULT([no])])])

# python-config
AS_IF([test x"$HAVE_PY_EXE" = x"yes"],
      [AC_CHECK_PROG([HAVE_PY_CONFIG], [python-config], yes, no)])

# Tweaks are needed on MinGW 
AS_IF([test x"$PYTHON_PLATFORM" = x"win32" && test x"$HAVE_PY_CONFIG" = x"yes"],
      [dnl replace '\' with '/'
       pythondir=${pythondir//\\//}
       pyexecdir=${pyexecdir//\\//}
       AC_MSG_CHECKING([for python directories on MinGW])
       dnl if prefix is not set
       AS_IF([test x"$prefix" = x"NONE"],
             [AC_MSG_RESULT([yes])
              dnl path to where Python is installed
              py_install_prefix=`dirname $PYTHON`
              py_install_prefix=${py_install_prefix%/bin}  # rip off /bin
              AS_IF([test x"$py_install_prefix" != x"$ac_default_prefix"],
                    [AC_MSG_CHECKING([for python script directory])
                     pythondir=$py_install_prefix/${pythondir#*/}
                     AC_MSG_RESULT([$pythondir])
                     AC_MSG_CHECKING([for python extension module directory])
                     pyexecdir=$py_install_prefix/${pyexecdir#*/}
                     AC_MSG_RESULT([$pyexecdir])])],
             [AC_MSG_RESULT([no])])
      ])

# enable debug
AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug], [Enable debug (-g -O0) build (default=-O2).]),
  [AS_IF([test x"$enableval" = x"yes"],
         [CXXFLAGS="-g -O0 -Wall -Wextra -Wsign-conversion -Wpedantic"; enable_debug=yes],
         [CXXFLAGS="-O2 -Wall -Wextra -Wsign-conversion -Wpedantic"; enable_debug=no])],
  [enable_debug=no])

# enable code coverage check with gcov
AC_ARG_ENABLE(gcov, 
  AS_HELP_STRING([--enable-gcov], [Enable code coverage check using gcov.(default=no)]), 
  [enable_gcov="$enableval"], [enable_gcov=no])

AS_IF([test x"$enable_gcov" != x"no"],
      [AC_CHECK_PROG([HAVE_GCOV], [gcov], yes, no)
       AS_IF([test x"$HAVE_GCOV" = x"no"],
             [AC_MSG_WARN([Unable to find gcov, unable to check code coverage.])])])

# flags to build code for code coverage check
AS_IF([test x"$enable_gcov" = x"yes"], [CXXFLAGS="-g -O0 -fprofile-arcs -ftest-coverage"])

# set CHECK_COVERAGE, which will be used in makefile.am
AM_CONDITIONAL([CHECK_COVERAGE], [test x"$enable_gcov" = x"yes"] )

# enable Weffc++
AC_ARG_ENABLE(weffcpp,
  AS_HELP_STRING([--enable-weffcpp], [Enable -Weffc++ (Warn violations of guidelines from Scott Meyers' Effect C++ books, default=no).]),
  [enable_weffcpp="$enableval"], [enable_weffcpp=no]
)

AS_IF([test x"$enable_weffcpp" != x"no"],[CXXFLAGS="$CXXFLAGS -Weffc++"])

# enable Werror
AC_ARG_ENABLE(werror,
  AS_HELP_STRING([--enable-werror], [Enable -Werror (Warnings treated as errors, default=no).]),
  [enable_werror="$enableval"], [enable_werror=no]
)

AS_IF([test x"$enable_werror" != x"no"],[CXXFLAGS="$CXXFLAGS -Werror"])

dnl When Lua is installed and coverage check is disabled
# set BUILD_LUA and TEST_LUA
AS_IF([test x"$HAVE_LUA_HPP" = x"yes" && test x"$HAVE_LUA_LIB" != x"no" &&
       test x"$enable_gcov" = x"no"], [TO_BUILD_LUA="yes"])
AM_CONDITIONAL([BUILD_LUA], [test x"$TO_BUILD_LUA" = x"yes"])
AM_CONDITIONAL([TEST_LUA], [test x"$TO_BUILD_LUA" = x"yes" &&
                            test x"$HAVE_LUA_EXE" = x"yes" &&
                            test x"$HAVE_LUA_UNIT" = x"yes"])

dnl When Python is installed and coverage check is disabled
# set BUILD_PY and TEST_PY
AS_IF([test x"$HAVE_PY_CONFIG" = x"yes" && test x"$enable_gcov" = x"no"], [TO_BUILD_PY="yes"])
AM_CONDITIONAL([BUILD_PY], [test x"$TO_BUILD_PY" = x"yes"])
AM_CONDITIONAL([TEST_PY], [test x"$TO_BUILD_PY" = x"yes" &&
                           test x"$HAVE_PY_EXE" = x"yes" &&
                           test x"$HAVE_PY_UNIT" = x"yes"])

# Files to be generated
AC_CONFIG_FILES([Makefile
                 include/Makefile
                 include/vp/Makefile
                 src/Makefile
                 src/bmp/Makefile src/gif/Makefile src/util/Makefile
                 src/lua/Makefile src/py/Makefile
                 test/Makefile
                 test/bmp/Makefile test/gif/Makefile test/util/Makefile
                 test/lua/Makefile test/py/Makefile
                 example/Makefile
                 example/cpp/Makefile example/img/Makefile
                 example/lua/Makefile example/py/Makefile
                 cmake/Makefile])
AC_OUTPUT

dnl Notices
if test x"$enable_gcov" = x"yes" && test x"$enable_debug" = x"no"; then
AC_MSG_NOTICE([
====================== Notice =======================
 Code optimization is disabled, because code coverage
 check requires debug build without optimization.
])
enable_debug=yes
fi

if test x"$enable_gcov" = x"yes"; then
AC_MSG_NOTICE([
================ Notice =================
 Lua and Python module will not build,
 because code coverage check is enabled.
])
fi

dnl Warnings
if test x"$HAVE_CPPUNIT" = x"no"; then
AC_MSG_WARN([
============================
 Unable to find CppUnit,
 unit tests will not build.
])
fi

if test x"$HAVE_LUA_HPP" = x"no"; then
AC_MSG_WARN([
=============================
 Unable to find lua.hpp,
 Lua module will not build.
])
fi

if test x"$HAVE_LUA_LIB" = x"no"; then
AC_MSG_WARN([
=============================
 Unable to find Lua library,
 Lua module will not build.
])
fi

if test x"$HAVE_LUA_EXE" = x"no"; then
AC_MSG_WARN([
=================================
 Unable to find Lua interpreter,
 Lua module tests will not run.
])
fi

if test x"$HAVE_LUA_UNIT" = x"no"; then
AC_MSG_WARN([
=================================
 Unable to find luaunit,
 Lua module tests will not run.
])
fi

if test x"$HAVE_PY_CONFIG" != x"yes"; then
AC_MSG_WARN([
================================
 Unable to find python-config,
 Python module will not build.
])
fi

if test x"$HAVE_PY_EXE" != x"yes"; then
AC_MSG_WARN([
====================================
 Unable to find Python interpreter,
 Python module tests will not run.
])
fi

if test x"$HAVE_PY_UNIT" != x"yes"; then
AC_MSG_WARN([
====================================
 Unable to find Python unittest,
 Python module tests will not run.
])
fi

dnl Summary
AC_MSG_NOTICE([
============ Summary =============
 ${PACKAGE_STRING}
 Build configuration:
   prefix:        ${prefix}
   debug:         ${enable_debug}
   Weffc++:       ${enable_weffcpp}
   Werror:        ${enable_werror}
   code coverage: ${enable_gcov}
   CXXFLAGS     : ${CXXFLAGS}
])
