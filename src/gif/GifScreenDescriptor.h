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

#ifndef GifScreenDescriptor_h
#define GifScreenDescriptor_h

#include <cstdint>
#include <iosfwd>
#include "GifColorTable.h"


/////////////////////////
class GifScreenDescriptor
{
public:
  GifScreenDescriptor( const uint8_t BitsPerPixel, 
                       const uint16_t Width, const uint16_t Height, 
                       const bool GlobalColorTable = true );
  GifScreenDescriptor( const GifScreenDescriptor& other );

  ~GifScreenDescriptor() = default;

  // not implemented
  GifScreenDescriptor( GifScreenDescriptor&& ) = delete;
  GifScreenDescriptor& operator=( const GifScreenDescriptor& ) = delete;
  GifScreenDescriptor& operator=( GifScreenDescriptor&& ) = delete;

  uint8_t  ColorResolution() const;

  uint16_t Width() const  { return m_ScreenWidth; }
  uint16_t Height() const { return m_ScreenHeight; }

  bool     GlobalColorTable() const;
  uint16_t ColorTableSize() const;
  void     ColorTableSize( uint16_t Size );
  bool     ColorTableSorted() const;
  void     SetColorTable( const uint8_t Index, const uint8_t Red,
                          const uint8_t Green, const uint8_t Blue );
  void     GetColorTable( const uint8_t Index, uint8_t& Red,
                          uint8_t& Green, uint8_t& Blue ) const;

  void     BackgroundColor( const uint8_t ColorIndex );
  uint8_t  BackgroundColor() const { return m_BackgroundColor; }

  uint8_t  AspectRatio() const     { return m_AspectRatio; }

  friend std::ostream& operator<<( std::ostream&, const GifScreenDescriptor& );
  friend std::istream& operator>>( std::istream&, GifScreenDescriptor& );

private:
  uint16_t m_ScreenWidth;
  uint16_t m_ScreenHeight;
  uint8_t  m_PackedByte; 
  uint8_t  m_BackgroundColor; 
  uint8_t  m_AspectRatio; 

  // global color table, optional(when size=0, no global color table)
  GifColorTable m_ColorTable;
};

// m_PackedByte
//  Global Color Table Flag     1 Bit
//  Color Resolution            3 Bits(value = bpp-1, bpp could be 1~8, though GIf supports 2~8)
//  Sorted Flag                 1 Bit (not set in ctor)
//  Size of Global Color Table  3 Bits(size = 2^(SizeBits + 1))

#endif //GifScreenDescriptor_h

