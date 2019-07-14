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

#include "BmpInfo4Bit.h"
#include "Exception.h"


//////////////////////////////////////////
BmpInfo4Bit::BmpInfo4Bit( const int32_t Width, const int32_t Height )
 : BmpInfo( BPP::BMP_4_BIT, Width, Height )
{
}

//////////////////////////////////////////
std::unique_ptr<BmpInfo> BmpInfo4Bit::Clone() const
{
  return std::unique_ptr<BmpInfo>( new BmpInfo4Bit(m_Width, m_Height) );
}

////////////////////////////////////////////////////////////////////
uint8_t BmpInfo4Bit::GetColorIndex( const uint8_t Byte, const uint8_t BitIndex ) const
{
#ifndef VP_EXTENSION
  // BitIndex: either 0th or 4th bit from left to right
  if( BitIndex != 0 && BitIndex != 4 )
    VP_THROW( "wrong bit index" ); 
#endif

  return (BitIndex == 0)? (Byte >> 4) : (Byte & 0x0F);
}

//////////////////////////////////////////////////////////////////////
void BmpInfo4Bit::SetColorIndex( uint8_t& Byte, const uint8_t BitIndex,
                                 const uint8_t ColorIndex ) const
{
#ifndef VP_EXTENSION
  //  BitIndex: either 0th or 4th bit from left to right
  if( BitIndex != 0 && BitIndex != 4 )
    VP_THROW( "wrong bit index" ); 

  // 4-bit color table only has 16 entries: 0 ~ 15
  if( ColorIndex > 15 )
    VP_THROW( "color index out of range" ); 
#endif

  if( BitIndex == 0 )
  {
    Byte &= 0x0F;
    Byte |= (ColorIndex << 4);
  }
  else
  {
    Byte &= 0xF0;
    Byte |= ColorIndex;
  }
}

/////////////////////////
// indices of byte and bit of the pixel in byte array
///////////////////////////////////////////////////////////////////
void BmpInfo4Bit::ByteArrayIndices( const int32_t X, const int32_t Y,
                                    uint32_t& ByteIndex, uint8_t& BitIndex )
{
#ifndef VP_EXTENSION
  if( X < 0 || X >= m_Width )
    VP_THROW( "x out of range" ); 

  if( Y < 0 || Y >= m_Height )
    VP_THROW( "y out of range" ); 
#endif

  ByteIndex = X/2 + (m_Height - 1 - Y)*m_RowLength;
  BitIndex  = (X%2 == 0)? 0 : 4;
}
