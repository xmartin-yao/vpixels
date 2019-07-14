////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 Xueyi Yao
//
// This file is part of VPixels.
//
// VPixels is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// VPixels is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#ifndef PyUtil_h
#define PyUtil_h

////////////////////
// define a method
//////////////////////////////////////////
#define MDef(name, func, sigature, doc ) \
  {#name, (PyCFunction)func, sigature, doc},

//////////////////////////////
// check if value >= lower
////////////////////////////////////////////////
#define Value_CheckLower( arg, value, lower ) \
  if( (value) < (lower) )        \
  { \
    PyErr_Format( PyExc_ValueError, "argument #%d expected >= %d (%d given)", \
                  (arg), (lower), (value) ); \
    return nullptr; \
  }

//////////////////////////////
// check if value < upper
////////////////////////////////////////////////
#define Value_CheckUpper( arg, value, upper ) \
  if( (value) >= (upper) )        \
  { \
    PyErr_Format( PyExc_ValueError, "argument #%d expected < %d (%d given)", \
                  (arg), (upper), (value) ); \
    return nullptr; \
  }

/////////////////////////
// check if value is within range [lower, upper]
/////////////////////////////////////////////////////
#define Value_CheckRange( arg, value, lower, upper ) \
  if( (value) < (lower) || (value) > (upper) )  \
  { \
    PyErr_Format( PyExc_ValueError, "argument #%d expected within [%d,%d] (%d given)", \
                  (arg), (lower), (upper), (value) ); \
    return nullptr; \
  }

/////////////////////////
// check if value is within range [lower, upper)
/////////////////////////////////////////////////////
#define Value_CheckRangeEx( arg, value, lower, upper ) \
  if( (value) < (lower) || (value) >= (upper) )  \
  { \
    PyErr_Format( PyExc_ValueError, "argument #%d expected within [%d,%d] (%d given)", \
                  (arg), (lower), (upper) - 1, (value) ); \
    return nullptr; \
  }

#endif //PyUtil_h
