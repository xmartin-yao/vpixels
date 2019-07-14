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

#include "GifCodeReader.h"
#include "Exception.h"

/////////////////////////////////////////
GifCodeReader::GifCodeReader( const U8String& CodeStr )
 : m_CodeStr( CodeStr ),
   m_CodeStrSize( CodeStr.size() ),
   m_ByteIndex( 0 ),
   m_BitCounter( 0 ),
   m_Buffer( 0 )
{
}

//////////////////////////
// read a code of CodeSize (in bits) from CodeStr
///////////////////////////////////////////////////
uint16_t GifCodeReader::operator()( const uint8_t CodeSize )
{
  // buffer the code
  while( m_BitCounter < CodeSize )
  {
    if( m_ByteIndex >= m_CodeStrSize )
      throw vp::Exception( "no more code" );

    // add a byte to buffer
    m_Buffer |= (m_CodeStr[m_ByteIndex] << m_BitCounter);
    ++m_ByteIndex;
    m_BitCounter += 8;
  }

  // extract the code
  uint16_t Code = m_Buffer & (0xFFFF >> (16 - CodeSize));

  // update buffer
  m_Buffer >>= CodeSize;
  m_BitCounter -= CodeSize;

  return Code;
}
