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

#include "BmpInfo24Bit.h"
#include "Exception.h"


//////////////////////////////////////////
BmpInfo24Bit::BmpInfo24Bit( const int32_t Width, const int32_t Height )
 : BmpInfo( BPP::BMP_24_BIT, Width, Height )
{
}

//////////////////////////////////////////
std::unique_ptr<BmpInfo> BmpInfo24Bit::Clone() const
{
  return std::unique_ptr<BmpInfo>( new BmpInfo24Bit(m_Width, m_Height) );
}

////////////////////////////////////////////////////////////////////
uint8_t BmpInfo24Bit::GetColorIndex( const uint8_t, const uint8_t ) const
{
  VP_THROW( "no color table" );
  return 0;
}

//////////////////////////////////////////////////////////////////////
void BmpInfo24Bit::SetColorIndex( uint8_t&, const uint8_t, const uint8_t ) const
{
  VP_THROW( "no color table" );
}

/////////////////////////
// indices of byte and bit of the pixel in byte array
///////////////////////////////////////////////////////////////////
void BmpInfo24Bit::ByteArrayIndices( const int32_t X, const int32_t Y,
                                     uint32_t& ByteIndex, uint8_t& BitIndex )
{
#ifndef VP_EXTENSION
  if( X < 0 || X >= m_Width )
    VP_THROW( "x out of range" ); 

  if( Y < 0 || Y >= m_Height )
    VP_THROW( "y out of range" ); 
#endif

  ByteIndex = 3*X + (m_Height - 1 - Y)*m_RowLength;
  BitIndex  = 0;
}
