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

#ifndef GifCodeReader_h
#define GifCodeReader_h

#include "U8String.h"

///////////////////
// Functor that reads codes from encoded string.
////////////////////////////////////////////////////
class GifCodeReader
{
public:
  explicit GifCodeReader( const U8String& CodeStr );
  ~GifCodeReader() = default;

  // not implemented
  GifCodeReader( const GifCodeReader& ) = delete;
  GifCodeReader( GifCodeReader&& ) = delete;
  GifCodeReader& operator=( const GifCodeReader& ) = delete;
  GifCodeReader& operator=( GifCodeReader&& ) = delete;

  uint16_t operator()( const uint8_t CodeSize );

private:
  const U8String& m_CodeStr;  // encoded string
  size_t   m_CodeStrSize;     // size of m_CodeStr
  size_t   m_ByteIndex;       // index to next byte in m_CodeStr
  uint8_t  m_BitCounter;      // number of bits in m_Buffer
  uint32_t m_Buffer;          // codes buffer, need 32-bit to accommodate
                              // two more bytes while 3 bits are left
};

#endif //GifCodeReader_h
