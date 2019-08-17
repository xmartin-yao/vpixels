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

#include "GifColorTable.h"
#include "Exception.h"
#include <istream>
#include <algorithm>

///////////////////////////////////////
GifColorTable::GifColorTable( uint16_t Size )
 : m_ArraySize( 3*Size ),  // 3 times of color table entries
   m_ByteArray( (m_ArraySize > 0)? new uint8_t[m_ArraySize] : nullptr )
{
  std::fill_n( m_ByteArray.get(), m_ArraySize, 0xFF ); // default value 255
}

///////////////////////////////////////////////
GifColorTable::GifColorTable( const GifColorTable& other )
 : m_ArraySize(other.m_ArraySize),
   m_ByteArray( (m_ArraySize > 0)? new uint8_t[m_ArraySize] : nullptr )
{
  std::copy_n( other.m_ByteArray.get(), m_ArraySize, m_ByteArray.get() );
}

///////////////////////////////////////////////
GifColorTable& GifColorTable::operator=( const GifColorTable& other )
{
  if( this == &other )
    return *this;

  if( m_ArraySize != other.m_ArraySize )
  {
    m_ArraySize = other.m_ArraySize;
    if( m_ArraySize == 0 )
      m_ByteArray.reset();
    else
      m_ByteArray.reset( new uint8_t[m_ArraySize] );
  }

  std::copy_n( other.m_ByteArray.get(), m_ArraySize, m_ByteArray.get() );

  return *this;
}

/////////////
// Set color table size before importing color table from GIF file,
// don't need to copy old byte array
/////////////////////////////////////////////////////
void GifColorTable::Size( uint16_t Size )
{
  Size *= 3;  // size of byte array

  // no change
  if( m_ArraySize == Size )
    return;

  m_ArraySize = Size;
  if( m_ArraySize != 0 )
  {
    m_ByteArray.reset( new uint8_t[m_ArraySize] );
    std::fill_n( m_ByteArray.get(), m_ArraySize, 0xFF ); // default value 255
  }
  else
    m_ByteArray.reset();
}

//////////////////////////////
// Change size of color table and modify the 1st and 
// the last 3 bits of PackedByte accordingly.
// Size will be rounded up to the next highest power of 2.
// Return true, if size is changed; false, if not.
////////////////////////////////////////////////////////////
bool GifColorTable::Size( uint16_t Size, uint8_t& PackedByte )
{
  // disable color table
  if( Size == 0 )
  {
    if( m_ArraySize == 0 )
      return false;

    PackedByte &= 0x78;  // 01111000
    m_ArraySize = 0;
    m_ByteArray.reset();

    return true;
  }

  // round up size
  uint8_t SizeBits = GifColorTable::Roundup( Size );
  Size = 1 << (SizeBits + 1);  // number of entries of color table 

  // no change
  Size *= 3;  // size of byte array
  if( Size == m_ArraySize )
    return false;

  // set packed byte
  PackedByte |= 0x80;      // set color table flag bit
  PackedByte &= 0xF8;      // replace size bits
  PackedByte += SizeBits;

  // new byte array, 
  std::unique_ptr<uint8_t[]> ByteArray{ new uint8_t[3*Size] };

  // copy part or all of old byte array
  std::copy_n( m_ByteArray.get(), std::min(Size, m_ArraySize), ByteArray.get() );

  // size enlarged, set extra entries
  if( Size > m_ArraySize )
    std::fill_n( &ByteArray[m_ArraySize], (Size - m_ArraySize), 0xFF ); // default value 255

  // set member data
  m_ByteArray = std::move(ByteArray);
  m_ArraySize = Size;

  return true;
}

/////////////////////////////////////////////////////
void GifColorTable::Get( const uint8_t Index, uint8_t& Red,
                         uint8_t& Green, uint8_t& Blue ) const
{
#ifndef VP_EXTENSION
  if( m_ArraySize == 0 )
    VP_THROW( "color table size == 0" );
#endif

  // Index: [0, m_ArraySize/3)
  uint16_t i = 3*Index;
#ifndef VP_EXTENSION
  if( i >= m_ArraySize )
    VP_THROW( "index out of range" );
#endif

  Red   = m_ByteArray[i];
  Green = m_ByteArray[++i];
  Blue  = m_ByteArray[++i];
}

/////////////////////////////////////////////////////
void GifColorTable::Set( const uint8_t Index, const uint8_t Red,
                         const uint8_t Green, const uint8_t Blue )
{
#ifndef VP_EXTENSION
  if( m_ArraySize == 0 )
    VP_THROW( "color table size == 0" );
#endif

  // Index: [0, m_ArraySize/3)
  uint16_t i = 3*Index;
#ifndef VP_EXTENSION
  if( i >= m_ArraySize )
    VP_THROW( "index out of range" );
#endif

  m_ByteArray[i]   = Red;
  m_ByteArray[++i] = Green;
  m_ByteArray[++i] = Blue;
}

/////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const GifColorTable& ct )
{
  if( ct.m_ArraySize != 0 )
    os.write( reinterpret_cast<char*>(&ct.m_ByteArray[0]), ct.m_ArraySize );

  return os;
}

/////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, GifColorTable& ct )
{
  if( ct.m_ArraySize != 0 )
    is.read( reinterpret_cast<char*>(&ct.m_ByteArray[0]), ct.m_ArraySize );
 
  return is;
}

///////////////////////////////////////
// GifImageDescriptor and GifScreenDescriptor use 3 bits to store
// color table size, which results in that color table size is power of 2. 
// This function rounds up the passed in Size to the next highest power of 2,
// returns the SizeBits used in GifImageDescriptor and GifScreenDescriptor
////////////////////////////////////////////////////////////////////////////
uint8_t GifColorTable::Roundup( uint16_t Size )
{
  // special case
  if( Size == 1 ) return 0;

  // count bits that follow the most-significant bit, examples:
  //  if Size=4 (100), there are 2 bits following the most-significant bit
  //  therefore Bits=2, there is no bit set among the two, SetBits=0,
  //  function returns 1, Size will be 2^(1+1)=4.
  //
  //  if Size=5 (101), then Bits=2 and SetBits=1, returns 2,
  //  Size will be 2^(2+1)=8.
  uint8_t Bits = 0;    // number of bits
  uint8_t SetBits = 0; // number of set bits
  while( Size > 1 )
  {
    ++Bits;
    SetBits += (Size & 1);
    Size >>= 1;
  }

  if( SetBits == 0 )  // passed in Size already is power of 2
    return Bits - 1;
  else
    return Bits;
}
