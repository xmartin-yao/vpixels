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

#include "GifScreenDescriptor.h"
#include "IOutil.h"
#include "Exception.h"
#include <istream>


//////////////////////////////////////////////////////////////////////
GifScreenDescriptor::GifScreenDescriptor( const uint8_t BitsPerPixel, 
                                          const uint16_t Width, const uint16_t Height, 
                                          const bool GlobalColorTable )
 : m_ScreenWidth( Width ), m_ScreenHeight( Height ),
   m_PackedByte( 0 ),
   m_BackgroundColor( 0 ),
   m_AspectRatio( 0 ),
   m_ColorTable( GlobalColorTable? (1 << BitsPerPixel) : 0 )
{
#ifndef VP_EXTENSION
  if( BitsPerPixel < 2 || BitsPerPixel > 8 )
    VP_THROW( "color resolution not supported" );
#endif

  uint8_t ResBits = BitsPerPixel - 1;
  m_PackedByte |= (ResBits << 4);  // color resolution bits
  if( GlobalColorTable )
  {
    m_PackedByte |= 0x80;    // global color table flag bit
    m_PackedByte |= ResBits; // color table size bits, match color resolution by default 
                             // sorted flag bit is not set
  }
}

//////////////////////////////////////////////////////////////////////
GifScreenDescriptor::GifScreenDescriptor( const GifScreenDescriptor& other )
 : m_ScreenWidth( other.m_ScreenWidth ), m_ScreenHeight( other.m_ScreenHeight ),
   m_PackedByte( other.m_PackedByte ),
   m_BackgroundColor( other.m_BackgroundColor ),
   m_AspectRatio( other.m_AspectRatio ),
   m_ColorTable( other.m_ColorTable )
{
}

///////////////////////////////////////
bool GifScreenDescriptor::GlobalColorTable() const
{
  return ((m_PackedByte & 0x80) == 0x80);
}

/////////////////////////////////////////
bool GifScreenDescriptor::ColorTableSorted() const
{
  return ((m_PackedByte & 0x08) == 0x08);
}

////////////////////////////////////////////////
uint8_t GifScreenDescriptor::ColorResolution() const
{
  uint8_t ResBits = m_PackedByte & 0x70;  // 0x70 = 01110000
  return (ResBits >> 4) + 1;
}

///////////////////////////////////////////////
uint16_t GifScreenDescriptor::ColorTableSize() const
{
  return m_ColorTable.Size();
}

// change color table size
///////////////////////////////////////////////
void GifScreenDescriptor::ColorTableSize( uint16_t Size )
{
#ifndef VP_EXTENSION
  if( Size > 256 )
    VP_THROW( "exceeds maximum size: 256" );
#endif

  m_ColorTable.Size( Size, m_PackedByte );

  // modify resolution bits
  uint8_t SizeBits = (m_PackedByte & 0x07);
  m_PackedByte &= 0x8F;
  m_PackedByte |= (SizeBits << 4);

  // set background color to 0, if no global color table
  if( Size == 0 && m_BackgroundColor != 0 )
    m_BackgroundColor = 0;
}

//////////////////////////////////////////////////////////////////////
void GifScreenDescriptor::SetColorTable( const uint8_t Index, const uint8_t Red,
                                         const uint8_t Green, const uint8_t Blue )
{
  m_ColorTable.Set( Index, Red, Green, Blue);
}

//////////////////////////////////////////////////////////////////////
void GifScreenDescriptor::GetColorTable( const uint8_t Index, uint8_t& Red,
                                         uint8_t& Green, uint8_t& Blue ) const
{
  m_ColorTable.Get( Index, Red, Green, Blue);
}

////////////////////////////////////////////////////////
void GifScreenDescriptor::BackgroundColor( const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( !GlobalColorTable() )
    VP_THROW( "No global color table" );

  if( ColorIndex >= ColorTableSize() )
    VP_THROW( "ColorIndex exceeds color table size" );
#endif

  m_BackgroundColor = ColorIndex;
}

//////////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const GifScreenDescriptor& sd )
{
  IOutil::Write( os, sd.m_ScreenWidth );
  IOutil::Write( os, sd.m_ScreenHeight );
  os.put( sd.m_PackedByte ); 
  os.put( sd.m_BackgroundColor ); 
  os.put( sd.m_AspectRatio ); 
  os << sd.m_ColorTable;

  return os;
}

//////////////////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, GifScreenDescriptor& sd )
{
  IOutil::Read( is, sd.m_ScreenWidth );
  IOutil::Read( is, sd.m_ScreenHeight );
  sd.m_PackedByte = is.get(); 
  sd.m_BackgroundColor = is.get(); 
  sd.m_AspectRatio = is.get(); 

  // global color table
  if( (sd.m_PackedByte & 0x80) == 0x80 )
  {
    uint8_t SizeBits = sd.m_PackedByte & 0x07; 
    sd.m_ColorTable.Size( 1 << (SizeBits + 1) );
    is >> sd.m_ColorTable;
  }
  else
    sd.m_ColorTable.Size( 0 );  // no global color table

  return is;
}
