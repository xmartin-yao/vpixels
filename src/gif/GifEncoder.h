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

#ifndef GifEncoder_h
#define GifEncoder_h

#include <cstdint>
#include "U8String.h"
#include "GifCodeWriter.h"
#include "GifStringTable.h"

// LZW encoding
// Functor that encodes pixels(bytes) into codes.
/////////////////////////////////////////////////
class GifEncoder
{
public:
  GifEncoder() = default;
  ~GifEncoder() = default;

  // not implemented
  GifEncoder( const GifEncoder& ) = delete;
  GifEncoder( GifEncoder&& ) = delete;
  GifEncoder& operator=( const GifEncoder& ) = delete;
  GifEncoder& operator=( GifEncoder&& ) = delete;

  void operator()( const uint8_t Bpp, const U8String&, U8String& );

private:
  void InitConstants( const uint8_t Bpp );
  void InitParameters();
  void UpdateParameters();

  // constants to certain bpp
  uint8_t  m_InitCodeSize; // initial code size in bits
  uint16_t m_ClearCode;    // clear code, max value 256 when bpp=8

  // dynamic parameters
  uint8_t  m_CodeSize;     // current code size in bits, max 12 bits
  uint16_t m_CodeLimit;    // upper bound of code under current code size
  uint16_t m_FreeCode;     // next available code 

  // string table
  GifStringTable m_StringTable;
};

#endif //GifEncoder_h
