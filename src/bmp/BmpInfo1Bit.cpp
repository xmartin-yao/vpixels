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

#include "BmpInfo1Bit.h"
#include "Exception.h"


//////////////////////////////////////////
BmpInfo1Bit::BmpInfo1Bit( const int32_t Width, const int32_t Height )
 : BmpInfo( BPP::BMP_1_BIT, Width, Height )
{
}

//////////////////////////////////////////
std::unique_ptr<BmpInfo> BmpInfo1Bit::Clone() const
{
  return std::unique_ptr<BmpInfo>( new BmpInfo1Bit(m_Width, m_Height) );
}

//////////////////////////////////////////////////////////////////////
uint8_t BmpInfo1Bit::GetColorIndex( const uint8_t Byte, const uint8_t BitIndex ) const
{
#ifndef VP_EXTENSION
  // BitIndex: 0~7 from left to right
  if( BitIndex > 7 )
    VP_THROW( "bit index out of range" ); 
#endif

  return (Byte >> (7 - BitIndex)) & 0x01;
}

//////////////////////////////////////////////////////////////////////
void BmpInfo1Bit::SetColorIndex( uint8_t& Byte, const uint8_t BitIndex,
                                 const uint8_t ColorIndex ) const
{
#ifndef VP_EXTENSION
  // BitIndex: 0~7 from left to right
  if( BitIndex > 7 )
    VP_THROW( "bit index out of range" ); 

  // 1-bit color table only has two entries: 0, 1
  if( ColorIndex > 1 )
    VP_THROW( "color index out of range" ); 
#endif

  uint8_t Mask = (0x80 >> BitIndex);
  if( ColorIndex == 0 )
    Byte &= ~Mask;
  else
    Byte |= Mask;
}

/////////////////////////
// indices of byte and bit of the pixel in byte array
///////////////////////////////////////////////////////////////////
void BmpInfo1Bit::ByteArrayIndices( const int32_t X, const int32_t Y,
                                    uint32_t& ByteIndex, uint8_t& BitIndex )
{
#ifndef VP_EXTENSION
  if( X < 0 || X >= m_Width )
    VP_THROW( "x out of range" ); 

  if( Y < 0 || Y >= m_Height )
    VP_THROW( "y out of range" ); 
#endif

  ByteIndex = X/8 + (m_Height - 1 - Y)*m_RowLength;
  BitIndex  = X % 8;
}
