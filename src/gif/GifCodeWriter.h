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

#ifndef GifCodeWriter_h
#define GifCodeWriter_h

#include <cstdint>
#include "U8String.h"

// Functor that write codes to string
//////////////////////////////////////////
class GifCodeWriter
{
public:
  explicit GifCodeWriter( U8String& CodeStr );
  ~GifCodeWriter() = default;

  // not implemented
  GifCodeWriter( const GifCodeWriter& ) = delete;
  GifCodeWriter( GifCodeWriter&& ) = delete;
  GifCodeWriter& operator=( const GifCodeWriter& ) = delete;
  GifCodeWriter& operator=( GifCodeWriter&& ) = delete;

  void operator()( const uint16_t Code, const uint8_t CodeSize );
  void End();

private:
  U8String& m_CodeStr;    // encoded string
  uint8_t   m_BitCounter; // number of bits in m_Buffer
  uint32_t  m_Buffer;     // codes buffer, need 32-bit to accommodate
                          // a 12-bit code while 7 bits are occupied
};

#endif //GifCodeWriter_h
