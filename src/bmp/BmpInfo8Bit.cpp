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

#include "BmpInfo8Bit.h"
#include "Exception.h"


//////////////////////////////////////////
BmpInfo8Bit::BmpInfo8Bit( const int32_t Width, const int32_t Height )
 : BmpInfo( BPP::BMP_8_BIT, Width, Height )
{
}

//////////////////////////////////////////
std::unique_ptr<BmpInfo> BmpInfo8Bit::Clone() const
{
  return std::unique_ptr<BmpInfo>( new BmpInfo8Bit(m_Width, m_Height) );
}

////////////////////////////////////////////////////////////////////
uint8_t BmpInfo8Bit::GetColorIndex( const uint8_t Byte, const uint8_t BitIndex ) const
{
#ifndef VP_EXTENSION
  // 8-bit pixel uses one byte 
  if( BitIndex != 0 ) VP_THROW( "wrong bit index" );
#else
  (void)BitIndex;
#endif

  return Byte;
}

//////////////////////////////////////////////////////////////////////
void BmpInfo8Bit::SetColorIndex( uint8_t& Byte, const uint8_t BitIndex,
                                 const uint8_t ColorIndex ) const
{
#ifndef VP_EXTENSION
  // 8-bit pixel uses one byte 
  if( BitIndex != 0 ) VP_THROW( "wrong bit index" );
#else
  (void)BitIndex;
#endif

  Byte = ColorIndex;
}

/////////////////////////
// indices of byte and bit of the pixel in byte array
///////////////////////////////////////////////////////////////////
void BmpInfo8Bit::ByteArrayIndices( const int32_t X, const int32_t Y,
                                    uint32_t& ByteIndex, uint8_t& BitIndex )
{
#ifndef VP_EXTENSION
  if( X < 0 || X >= m_Width )
    VP_THROW( "x out of range" ); 

  if( Y < 0 || Y >= m_Height )
    VP_THROW( "y out of range" ); 
#endif

  ByteIndex = static_cast<uint32_t>(X) + static_cast<uint32_t>(m_Height - 1 - Y)*m_RowLength;
  BitIndex = 0;
}
