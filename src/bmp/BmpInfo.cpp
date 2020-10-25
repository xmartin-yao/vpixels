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

#include "BmpInfo.h"
#include "BmpInfo1Bit.h"
#include "BmpInfo4Bit.h"
#include "BmpInfo8Bit.h"
#include "BmpInfo24Bit.h"
#include "Exception.h"


//////////////////////////////////////////////
BmpInfo::BmpInfo( const BPP bpp, const int32_t Width, const int32_t Height )
 : m_BitsPerPixel( static_cast<uint8_t>(bpp) ),
   m_Width( Width ), m_Height( Height ),
   m_RowLength( CalculateRowLength() )
{
}

//////////////////////////////
// image data size, including padding bytes
// (excluding file header, info header and color table)
//////////////////////////////////////////////////////////
uint32_t BmpInfo::ImageDataSize() const
{
  return (m_RowLength + PaddingBytes(m_RowLength))*static_cast<uint32_t>(m_Height);
}

//////////////////////////////
// size of byte array
// byte array is image data in memory, excluding padding bytes
///////////////////////////////////////////////////////////////
uint32_t BmpInfo::ByteArraySize() const
{
  return m_RowLength*static_cast<uint32_t>(m_Height);
}

#if 0
/*
 This is a general implementation that works for all color depths
 (1-, 4-, 8- and 24-bit). Because it is a frequently function, 
 implementations more specific for each color depth in subclasses
 will make this function faster.
*/
/////////////////////////
// indices of byte and bit of the pixel in byte array
///////////////////////////////////////////////////////////////////
void BmpInfo::ByteArrayIndices( const int32_t X, const int32_t Y,
                                uint32_t& ByteIndex, uint8_t& BitIndex )
{
#ifndef VP_EXTENSION
  if( X < 0 || X >= m_Width )
    VP_THROW( "x out of range" ); 

  if( Y < 0 || Y >= m_Height )
    VP_THROW( "y out of range" ); 
#endif

  ByteIndex = (X*m_BitsPerPixel)/8 + (m_Height - 1 - Y)*m_RowLength;
  BitIndex  = (X*m_BitsPerPixel) % 8;
}
#endif

/////////////////////////////////
// length of each row (in bytes), excluding padding bytes
/////////////////////////////////////////////////////////
uint32_t BmpInfo::CalculateRowLength() const
{
  uint32_t nBytes = static_cast<uint32_t>(m_Width)*m_BitsPerPixel / 8;
  if( (m_Width*m_BitsPerPixel % 8) != 0 ) 
    ++nBytes;

  return nBytes;
}

/////////////////////////////////////////////////
bool BmpInfo::Supported( const uint8_t BitsPerPixel )
{
  bool Ret = false;
  switch( static_cast<BPP>(BitsPerPixel) )
  {
    case BPP::BMP_1_BIT:
    case BPP::BMP_4_BIT:
    case BPP::BMP_8_BIT:
    case BPP::BMP_24_BIT:
      Ret = true;
  }

  return Ret;
}

///////////////////////////
// bytes to be padded to the end of each line
// such that bytes of each line is a multiple of 4
///////////////////////////////////////////////////////
uint8_t BmpInfo::PaddingBytes( const uint32_t RowLength )
{
  uint8_t Remainder = RowLength % 4;
  if( Remainder == 0 )
    return 0;
  else
    return 4 - Remainder;
}


//////////////////////////////////////////////////////////////////////
std::unique_ptr<BmpInfo>
BmpInfo::Create( const uint8_t BitsPerPixel,
                 const int32_t Width, const int32_t Height )
{
#ifndef VP_EXTENSION
  if( !BmpInfo::Supported(BitsPerPixel) )
    VP_THROW( "color depth not supported" )
#endif

  std::unique_ptr<BmpInfo> pInfo;
  switch( static_cast<BPP>(BitsPerPixel) )
  {
    case BPP::BMP_1_BIT:
      pInfo.reset( new BmpInfo1Bit(Width, Height) );
      break;

    case BPP::BMP_4_BIT:
      pInfo.reset( new BmpInfo4Bit(Width, Height) );
      break;

    case BPP::BMP_8_BIT:
      pInfo.reset( new BmpInfo8Bit(Width, Height) );
      break;

    case BPP::BMP_24_BIT:
      pInfo.reset( new BmpInfo24Bit(Width, Height) );
  }

  return pInfo;
}
