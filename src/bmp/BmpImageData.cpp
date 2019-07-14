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

#include "BmpImageData.h"
#include "BmpInfo.h"
#include "Exception.h"
#include <iostream>
#include <algorithm>

///////////////////////////////////////////
BmpImageData::BmpImageData( const BmpInfo& Info )
 : m_Size( Info.ByteArraySize() ),
   m_ByteArray( m_Size > 0 ? new uint8_t[m_Size]{0} : nullptr ), //default value 0
   m_RowLength( Info.RowLength() ),
   m_Height( Info.Height() )
{
}

/////////////////////////////////////////////
BmpImageData::BmpImageData( const BmpImageData& other )
 : m_Size( other.m_Size ),
   m_ByteArray( m_Size > 0 ? new uint8_t[m_Size] : nullptr ),
   m_RowLength( other.m_RowLength ),
   m_Height( other.m_Height )
{
  std::copy_n( other.m_ByteArray.get(), m_Size, m_ByteArray.get() );
}

////////////////////////////////////////
void BmpImageData::Init( const BmpInfo& Info )
{
  m_Size = Info.ByteArraySize();
  if( m_Size > 0)
    m_ByteArray.reset( new uint8_t[m_Size]{0} );  // default value: 0
  else
    m_ByteArray.reset();

  m_RowLength = Info.RowLength();
  m_Height = Info.Height();
}

//////////////////////////////////////////////
uint8_t& BmpImageData::operator[]( size_t Index ) const
{
#ifndef VP_EXTENSION
  // Index: 0 ~ (m_Size - 1)
  if( Index >= m_Size )
    VP_THROW( "index out of range" );
#endif

  return m_ByteArray[Index];
}

/////////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, BmpImageData& id )
{
  const auto nBytes = BmpInfo::PaddingBytes( id.m_RowLength );
  size_t i = 0;
  for( int32_t row = 0; row < id.m_Height; ++row, i += id.m_RowLength )
  {
    // read a row
    is.read( reinterpret_cast<char*>(&id.m_ByteArray[i]), id.m_RowLength );

    // skip padding bytes
    if( nBytes > 0 ) is.ignore( nBytes );
  }

  return is;
}

///////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const BmpImageData& id )
{
  const auto nBytes = BmpInfo::PaddingBytes( id.m_RowLength );
  std::unique_ptr<char[]> Bytes{nBytes > 0 ? new char[nBytes]{0} : nullptr };

  size_t i = 0;
  for( int32_t row = 0; row < id.m_Height; ++row, i += id.m_RowLength )
  {
    // write a row
    os.write( reinterpret_cast<char*>(&id.m_ByteArray[i]), id.m_RowLength );

    // write padding bytes
    if( nBytes > 0 ) os.write( &Bytes[0], nBytes );
  }

  return os;
}
