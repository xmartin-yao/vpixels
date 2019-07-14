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

#include "GifCodeWriter.h"

// caller's responsibility to initialize the string
//////////////////////////////////////////
GifCodeWriter::GifCodeWriter( U8String& CodeStr )
 : m_CodeStr( CodeStr ),
   m_BitCounter( 0 ),
   m_Buffer( 0 )
{
}

/////////////////////////
// write Code of CodeSize (in bits) to CodeStr
/////////////////////////////////////////////////////////////
void GifCodeWriter::operator()( const uint16_t Code, const uint8_t CodeSize )
{
  // buffer the code
  m_Buffer += (Code << m_BitCounter);
  m_BitCounter += CodeSize;

  // if buffered codes can form byte(s)
  while( m_BitCounter >= 8 )
  {
    // add a byte to string
    m_CodeStr += (m_Buffer & 0xFF);

    // update buffer
    m_Buffer >>= 8;
    m_BitCounter -= 8;
  }
}

// end code writing
void GifCodeWriter::End()
{
  if( m_BitCounter > 0 )
  {
    // add the last byte to code string
    m_CodeStr += (m_Buffer & 0xFF);

    m_Buffer = 0;
    m_BitCounter = 0;
  }
}
