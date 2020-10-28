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

#include "GifImageDescriptor.h"
#include "IOutil.h"
#include "Exception.h"

////////////////////////////////////////
GifImageDescriptor::GifImageDescriptor()
 : GifComponent( 0x2C ), 
   m_Left( 0 ), m_Top( 0 ),      // left = 0, top = 0
   m_Width( 0 ), m_Height( 0 ),  // 0x0
   m_PackedByte( 0 ),
   m_ColorTable( 0 ), // no local color table
   m_ImageData()      // bpp = 2, no pixels
{
}

///////////////////////////////////////////////////////////////////////
GifImageDescriptor::GifImageDescriptor( const uint8_t BitsPerPixel,
                                        const uint16_t Width, const uint16_t Height,
                                        const uint16_t Left, const uint16_t Top,
                                        const bool ColorTable )
 : GifComponent( 0x2C ),
   m_Left( Left ), m_Top( Top ), 
   m_Width( Width ), m_Height( Height ),
   m_PackedByte( 0 ),
   m_ColorTable( ColorTable? (1 << BitsPerPixel) : 0 ),
   m_ImageData( BitsPerPixel, static_cast<uint32_t>(Width*Height) )
{
  // if there is local color table, its default size = 2^BitsPerPixel
  if( ColorTable )
    // set local color table flag bit and size bits
    m_PackedByte |= 0x80 + (BitsPerPixel - 1);
}

////////////////////////////////////////////////////////
GifImageDescriptor::GifImageDescriptor( const GifImageDescriptor& other )
 : GifComponent( 0x2C ),
   m_Left( other.m_Left ), m_Top( other.m_Top ),
   m_Width( other.m_Width ), m_Height( other.m_Height ),
   m_PackedByte( other.m_PackedByte ),
   m_ColorTable( other.m_ColorTable ),
   m_ImageData( other.m_ImageData )
{
}

//////////////////////////////////////////////////////////////////////
GifImageDescriptor& GifImageDescriptor::operator=( const GifImageDescriptor& other )
{
  if( this != &other )
  {
    m_Left = other.m_Left;
    m_Top  = other.m_Top;
    m_Width  = other.m_Width;
    m_Height = other.m_Height;
    m_PackedByte = other.m_PackedByte;
    m_ColorTable = other.m_ColorTable;
    m_ImageData  = other.m_ImageData;
  }

  return *this;
}

/////////////////////////////////////////
bool GifImageDescriptor::LocalColorTable() const
{
  return ((m_PackedByte & 0x80) == 0x80);
}

////////////////////////////////////////
bool GifImageDescriptor::Interlaced() const
{
  return (m_PackedByte & 0x40) != 0;  // 0x40 = 01000000
}

///////////////////////////////////
bool GifImageDescriptor::ColorTableSorted() const
{
  return (m_PackedByte & 0x20) != 0;  // 0x20 = 00100000
}

//////////////////////////////////////////////////////////////
void GifImageDescriptor::Crop( const uint16_t Left, const uint16_t Top,
                               const uint16_t Width, const uint16_t Height )
{
#ifndef VP_EXTENSION
  if( Left < m_Left )
    VP_THROW( "argument 'Left' exceeds lower limit" );

  if( Left >= m_Left + m_Width )
    VP_THROW( "argument 'Left' exceeds upper limit" );

  if( Top < m_Top )
    VP_THROW( "argument 'Top' exceeds lower limit" );

  if( Top >= m_Top + m_Height )
    VP_THROW( "argument 'Top' exceeds upper limit" );

  if( Width == 0 )
    VP_THROW( "argument 'Width' exceeds lower limit" );

  if( Width + Left > m_Width + m_Left )
    VP_THROW( "argument 'Width' exceeds upper limit" );

  if( Height == 0 )
    VP_THROW( "argument 'Height' exceeds lower limit" );

  if( Height + Top > m_Height + m_Top )
    VP_THROW( "argument 'Height' exceeds upper limit" );
#endif

  // no change
  if( Left == m_Left && Top == m_Top && Width == m_Width && Height == m_Height )
    return;

  // new image data
  GifImageData ImageData( BitsPerPixel(), static_cast<uint32_t>(Width*Height) );

  // copy pixels from old image data
  auto dx = Left - m_Left;
  auto dy = Top - m_Top;
  for( uint16_t x = 0; x < Width; ++x )
  {
    for( uint16_t y = 0; y < Height; ++y )
    {
      uint8_t Color = m_ImageData.GetPixel( static_cast<uint32_t>((x + dx) + m_Width*(y + dy)) );
      ImageData.SetPixel( static_cast<uint32_t>(x + Width*y), Color );
    }
  }

  // replace old image data
  m_ImageData = std::move(ImageData);

  // set parameters to new values
  m_Left = Left;
  m_Top = Top;
  m_Width = Width;
  m_Height = Height;
}

////////////////////////////////////////////////
uint16_t GifImageDescriptor::ColorTableSize() const
{
  return m_ColorTable.Size();
}

////////////////////////////////////////////////
bool GifImageDescriptor::ColorTableSize( uint16_t Size )
{
#ifndef VP_EXTENSION
  if( Size > 256 )
    VP_THROW( "exceeds maximum size: 256" );
#endif

  // size not changed
  if( !m_ColorTable.Size( Size, m_PackedByte ) )
    return false;

  // if color table enabled, set bpp based on m_PackedByte
  if( LocalColorTable() )
  {
    uint8_t Bpp = (m_PackedByte & 0x07) + 1;

    // special case when color table size set to 0 or 2
    if( Bpp == 1 ) Bpp = 2;

    m_ImageData.BitsPerPixel( Bpp );
  }

  return true;
}

///////////////////////////////////////////////////////////////
void GifImageDescriptor::SetColorTable( const uint8_t Index, const uint8_t Red, 
                                        const uint8_t Green, const uint8_t Blue )
{
  m_ColorTable.Set( Index, Red, Green, Blue);
}

//////////////////////////////////////////////////////////////////////
void GifImageDescriptor::GetColorTable( const uint8_t Index, uint8_t& Red, 
                                        uint8_t& Green, uint8_t& Blue ) const
{
  m_ColorTable.Get( Index, Red, Green, Blue);
}

/////////////////////////////////////
uint8_t GifImageDescriptor::BitsPerPixel() const
{
  return m_ImageData.BitsPerPixel();
}

//////////////////////////////////////
void GifImageDescriptor::BitsPerPixel( const uint8_t Bpp )
{
  m_ImageData.BitsPerPixel( Bpp );
}

/////////////////////
// coordinate to index of image data
///////////////////////////////////////////////////////
uint32_t GifImageDescriptor::PixelIndex( const uint16_t X, const uint16_t Y ) const
{
#ifndef VP_EXTENSION
  if( X >= m_Width )
    VP_THROW( "x out of range" );

  if( Y >= m_Height )
    VP_THROW( "y out of range" );
#endif

  return static_cast<uint32_t>(X + m_Width*Y);
}

//////////////////////////////////////////////////
void GifImageDescriptor::SetAllPixels( const uint8_t ColorIndex )
{
  m_ImageData.SetAllPixels( ColorIndex );
}

//////////////////////////////////////////////////////////////////////
void GifImageDescriptor::SetPixel( uint16_t X, uint16_t Y, uint8_t ColorIndex )
{
  m_ImageData.SetPixel( PixelIndex(X, Y), ColorIndex );
}

///////////////////////////////////////////////////////////////
uint8_t GifImageDescriptor::GetPixel( uint16_t X, uint16_t Y ) const
{
  return m_ImageData.GetPixel( PixelIndex(X, Y) );
}

////////////////////////////////////////
GifComponent* GifImageDescriptor::Clone() const
{
  return new GifImageDescriptor( *this );
}

/////////////////////////////////////////
void GifImageDescriptor::Read( std::istream& is )
{
  // m_ID has already been read
  IOutil::Read( is, m_Left );
  IOutil::Read( is, m_Top );
  IOutil::Read( is, m_Width );
  IOutil::Read( is, m_Height );
  m_PackedByte = is.get(); 

  if( Interlaced() )
    throw vp::Exception( "interlaced image not supported" );

  // local color table
  if( LocalColorTable() )
  {
    uint8_t SizeBits = m_PackedByte & 0x07; 
    m_ColorTable.Size( 1 << (SizeBits + 1) );
    is >> m_ColorTable;
  }
  else
    m_ColorTable.Size( 0 );  // no local color table

  // image data
  m_ImageData.Init( static_cast<size_t>(m_Width*m_Height) );
  is >> m_ImageData;
  if( m_ImageData.Size() != static_cast<size_t>(m_Width*m_Height) )
    throw vp::Exception( "wrong image data size" );
}

///////////////////////////////////////////
void GifImageDescriptor::Write( std::ostream& os ) const
{
  IOutil::Write( os, m_ID );
  IOutil::Write( os, m_Left );
  IOutil::Write( os, m_Top );
  IOutil::Write( os, m_Width );
  IOutil::Write( os, m_Height );
  IOutil::Write( os, m_PackedByte );
  os << m_ColorTable;
  os << m_ImageData;
}
