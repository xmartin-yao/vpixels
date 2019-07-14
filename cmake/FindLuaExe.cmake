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

# Function to find Lua interpreter
# These variables get set if it is found
#   LUA_EXE_FOUND
#   LUA_EXECUTABLE  full path to the interpreter
#   LUA_EXE_VERSION_STRING
function(FindLuaExe version)
  set(LUA_EXE_FOUND FALSE PARENT_SCOPE)
  set(LUA_EXE_VERSION_STRING "" PARENT_SCOPE)

  # search for Lua interpreter
  find_program(LUA_EXECUTABLE NAMES lua lua.exe)
  if(LUA_EXECUTABLE)
    # run lua -e "print(_VERSION)", get "Lua <major>.<minor>"
    execute_process(COMMAND "${LUA_EXECUTABLE}" -e "print(_VERSION)"
                    OUTPUT_VARIABLE _VERSION
                    RESULT_VARIABLE _RET
                    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(_RET EQUAL 0)
      # strip off "Lua ", get version number
      string(REGEX REPLACE "Lua[ ]+" "" _VERSION "${_VERSION}")

      if(_VERSION EQUAL version)
        # set variables
        set(LUA_EXE_FOUND TRUE PARENT_SCOPE)
        set(LUA_EXE_VERSION_STRING ${_VERSION} PARENT_SCOPE)
      endif()
    endif()
  endif()

  # clean up
  unset(_VERSION)
  unset(_RET)
endfunction()
