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

#include "BmpColorTable.h"
#include "BmpInfo.h"
#include "Exception.h"
#include <iostream>
#include <algorithm>

////////////////////////////////////////////////////////
BmpColorTable::BmpColorTable( uint16_t Size )
 : m_ArraySize( 4*Size ),  // 4 times of color table entries
   m_ByteArray( m_ArraySize > 0 ? new uint8_t[m_ArraySize]{0} : nullptr ) //default value 0
{
}

///////////////////////////////////////////////
BmpColorTable::BmpColorTable( const BmpColorTable& other )
 : m_ArraySize( other.m_ArraySize ),
   m_ByteArray( m_ArraySize > 0 ? new uint8_t[m_ArraySize] : nullptr )
{
  std::copy_n( other.m_ByteArray.get(), m_ArraySize, m_ByteArray.get() );
}

/////////////
// This function is called to set color table size before importing
// color table from BMP file, don't need to copy previous byte array
/////////////////////////////////////////////////////
void BmpColorTable::Size( uint16_t Size )
{
  Size *= 4;  // size of byte array

  // same size
  if( Size == m_ArraySize )
    return;

  m_ArraySize = Size;
  if( m_ArraySize == 0 )
    m_ByteArray.reset();
  else
    m_ByteArray.reset( new uint8_t[m_ArraySize]{0} ); // default value 0
}

//////////////////////////////////////////////////
void BmpColorTable::Get( const uint8_t Index, uint8_t& Blue,
                         uint8_t& Green, uint8_t& Red ) const
{
#ifndef VP_EXTENSION
  if( m_ArraySize == 0 )
    VP_THROW( "color table size == 0" );
#endif

  // Index: [0, m_ArraySize/4)
  uint16_t i = 4*Index;
#ifndef VP_EXTENSION
  if( i >= m_ArraySize )
    VP_THROW( "index out of range" );
#endif

  Blue  = m_ByteArray[i];
  Green = m_ByteArray[++i];
  Red   = m_ByteArray[++i];
}

////////////////////////////////////////////////////
void BmpColorTable::Set( const uint8_t Index, const uint8_t Blue,
                         const uint8_t Green, const uint8_t Red )
{
#ifndef VP_EXTENSION
  if( m_ArraySize == 0 )
    VP_THROW( "color table size == 0" );
#endif

  // Index: [0, m_ArraySize/4)
  uint16_t i = 4*Index;
#ifndef VP_EXTENSION
  if( i >= m_ArraySize )
    VP_THROW( "index out of range" );
#endif

  m_ByteArray[i]   = Blue;
  m_ByteArray[++i] = Green;
  m_ByteArray[++i] = Red;
}

//////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const BmpColorTable& ct )
{
  if( ct.m_ArraySize != 0 )
    os.write( reinterpret_cast<char*>(&ct.m_ByteArray[0]), ct.m_ArraySize );

  return os;
}

/////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, BmpColorTable& ct )
{
  if( ct.m_ArraySize != 0 )
    is.read( reinterpret_cast<char*>(&ct.m_ByteArray[0]), ct.m_ArraySize );

  return is;
}
