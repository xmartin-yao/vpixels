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

#ifndef GifDecoder_h
#define GifDecoder_h

#include <cstdint>
#include "U8String.h"

// LZW decoding
// Functor that decodes codes into pixels(bytes).
/////////////////////////////////////////////////
class GifDecoder
{
public:
  GifDecoder()  = default;
  ~GifDecoder() = default;

  // not implemented
  GifDecoder( const GifDecoder& ) = delete;
  GifDecoder( GifDecoder&& ) = delete;
  GifDecoder& operator=( const GifDecoder& ) = delete;
  GifDecoder& operator=( GifDecoder&& ) = delete;

  void operator()( const uint8_t Bpp, const U8String&, U8String& );

private:
  void     InitConstants( const uint8_t Bpp );
  void     InitParameters();
  void     OutputPixel( const uint16_t Code, U8String& OutStream );
  uint8_t  OutputString( uint16_t Code, U8String& OutStream );

  // constants to certain bpp
  uint16_t m_ClearCode;    // clear code, max value 256 when bpp=8
  uint8_t  m_InitCodeSize; // initial code size

  // dynamic parameters
  uint8_t  m_CodeSize;     // current code size in bits, max 12 bits
  uint16_t m_CodeLimit;    // upper bound of code under current code size
  uint16_t m_FreeCode;     // next available code 

  // pixel buffer
  uint8_t m_Buffer[4092];  // 4092 is maximum number of codes
                           // (when BPP = 1, codes: [4, 4095])
  // string table
  uint16_t m_Prefix[4096];
  uint8_t  m_Suffix[4096];
};

#endif //GifDecoder_h
