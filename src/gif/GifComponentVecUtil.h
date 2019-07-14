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

#ifndef GifComponentVecUtil_h
#define GifComponentVecUtil_h

#include <cstdint>
#include <iosfwd>
#include "VecDefs.h"


// GifComponentVec related functions
namespace GifComponentVecUtil
{
  // create a GifComponentVec
  GifComponentVec Create( const uint8_t BitsPerPixel,
                          const uint16_t Width, const uint16_t Height,
                          const size_t Images = 1,
                          const bool GlobalColorTable = true );

  // copy a GifComponentVec
  GifComponentVec Create( const GifComponentVec& other );

  // read GifComponentVec from stream
  uint8_t Read( std::istream&, GifComponentVec& );

  // write GifComponentVec to stream
  void Write( std::ostream&, GifComponentVec& );
}

#endif //GifComponentVecUtil_h
