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

#include "GifImpl.h"
#include "GifImage.h"
#include "Exception.h"
#include "GifComponentVecUtil.h"
#include "GifImageVecBuilder.h"
#include <fstream>
#include <sstream>


///////////////////////////////////////////////////////////
GifImpl::GifImpl( const uint8_t BitsPerPixel,
                  const uint16_t Width, const uint16_t Height,
                  const size_t Images, const bool GlobalColor )
 : m_Header( GifVersion::V89a ),
   m_ScreenDescriptor( BitsPerPixel, Width, Height, GlobalColor ),
   m_ComponentVec( GifComponentVecUtil::Create(BitsPerPixel, Width, Height, Images, GlobalColor) ),
   m_ImageVec( GifImageVecBuilder()( *this, m_ComponentVec ) )
{
}

GifImpl::GifImpl( const GifImpl& other )
 : m_Header( other.m_Header ),
   m_ScreenDescriptor( other.m_ScreenDescriptor ),
   m_ComponentVec( GifComponentVecUtil::Create(other.m_ComponentVec) ),
   m_ImageVec( GifImageVecBuilder()( *this, m_ComponentVec ) )
{
}

///////////////////////////////////
std::string GifImpl::Version() const
{
  switch( m_Header.Version() )
  {
    case GifVersion::V87a :
      return "GIF87a";

    case GifVersion::V89a :
      return "GIF89a";

    default:
      return "Unknown";
  }
}

/////////////////////////////////
uint8_t GifImpl::BitsPerPixel() const
{
  return m_ScreenDescriptor.ColorResolution();
}

/////////////////////////////////
void GifImpl::BitsPerPixel( const uint8_t Bpp )
{
#ifndef VP_EXTENSION
  if( Bpp < 2 || Bpp > 8 )
    VP_THROW( "color resolution not supported" );
#endif

  // set bpp
  m_ScreenDescriptor.ColorResolution( Bpp );
  for( auto& pImage : m_ImageVec )
  {
    if( !pImage->ColorTable() )
      pImage->BitsPerPixel( Bpp );
  }

  // set global color table size
  if( m_ScreenDescriptor.GlobalColorTable() )
    m_ScreenDescriptor.ColorTableSize( 1 << Bpp );
}

//////////////////////////
uint16_t GifImpl::Width() const
{
  return m_ScreenDescriptor.Width();
}

//////////////////////////
uint16_t GifImpl::Height() const
{
  return m_ScreenDescriptor.Height();
}

//////////////////////////////////
bool GifImpl::ColorTable() const
{
  return m_ScreenDescriptor.GlobalColorTable();
}

////////////////////////////////////
uint16_t GifImpl::ColorTableSize() const
{
  return m_ScreenDescriptor.ColorTableSize();
}

////////////////////////////////////////
void GifImpl::ColorTableSize( uint16_t Size )
{
  // change size
  if( !m_ScreenDescriptor.ColorTableSize( Size ) )
    return;

  // set bpp of images that have no local color table
  for( auto& pImage : m_ImageVec )
  {
    if( !pImage->ColorTable() )
      pImage->BitsPerPixel( m_ScreenDescriptor.ColorResolution() );
  }
}

//////////////////////////////////
bool GifImpl::ColorTableSorted() const
{
  return m_ScreenDescriptor.ColorTableSorted();
}

/////////////////////////////////////////////////////////
void GifImpl::SetColorTable( const uint8_t Index, const uint8_t Red,
                             const uint8_t Green, const uint8_t Blue )
{
  m_ScreenDescriptor.SetColorTable( Index, Red, Green, Blue );
}

/////////////////////////////////////////////////////////
void GifImpl::GetColorTable( const uint8_t Index, uint8_t& Red,
                             uint8_t& Green, uint8_t& Blue ) const
{
  m_ScreenDescriptor.GetColorTable( Index, Red, Green, Blue );
}

//////////////////////////////////
void GifImpl::BackgroundColor( const uint8_t ColorIndex )
{
  return m_ScreenDescriptor.BackgroundColor( ColorIndex );
}

//////////////////////////////////
uint8_t GifImpl::BackgroundColor() const
{
  return m_ScreenDescriptor.BackgroundColor();
}

//////////////////////////////////
uint8_t GifImpl::AspectRatio() const
{
  return m_ScreenDescriptor.AspectRatio();
}

////////////////////////////
size_t GifImpl::Images() const
{
  return m_ImageVec.size();
}

//////////////////////////////////////////////////
vp::GifImage& GifImpl::operator[]( const size_t Index ) &
{
#ifndef VP_EXTENSION
  if( Index >= m_ImageVec.size() )
    VP_THROW( "index out of range" );
#endif

  return *m_ImageVec[Index];
}

//////////////////////////////////////////////////
const vp::GifImage& GifImpl::operator[]( const size_t Index ) const &
{
#ifndef VP_EXTENSION
  if( Index >= m_ImageVec.size() )
    VP_THROW( "index out of range" );
#endif

  return *m_ImageVec[Index];
}

//////////////////////////////////////////////
bool GifImpl::Import( const std::string& FileName )
{
  std::ifstream File;

  // check existence
  File.open( FileName, std::ios::binary );
  if( !File.is_open() )
    return false;

  try
  {
    // read from file
    uint8_t LastByte = Read( File );

    File.close();
    if( LastByte != m_GifTrailer )
      throw vp::Exception( "not a valid GIF file" );

    // create GifImageVec
    m_ImageVec = GifImageVecBuilder()( *this, m_ComponentVec );

    return true;
  }
  catch( const vp::Exception& )
  {
    File.close();
    throw;
  }
}

///////////////////////////////////////////////////////////////
bool GifImpl::Export( const std::string& FileName, const bool OverWrite )
{
  std::fstream File;

  // check existence
  bool Exist = false;
  File.open( FileName, std::ios::in|std::ios::binary );
  if( File.is_open() ) Exist = true;
  File.close();
  
  if( Exist && !OverWrite )
    return false;

  File.open( FileName, std::ios::out|std::ios::binary );
  Write( File );
  File.close();

  return true;
}

// read all elements from stream
////////////////////////////////////////////
uint8_t GifImpl::Read( std::istream& is )
{
  is >> m_Header;
  if( !m_Header )
    throw vp::Exception( "not a GIF file" );

  is >> m_ScreenDescriptor;

  if( is.good() )
    return GifComponentVecUtil::Read( is, m_ComponentVec );
  else
    return 0x00;
}

// write all elements into stream
/////////////////////////////////////////////
void GifImpl::Write( std::ostream& os )
{
  os << m_Header;
  os << m_ScreenDescriptor;
  GifComponentVecUtil::Write( os, m_ComponentVec );
  os.put( m_GifTrailer );
}

// size of the resulting GIF file in byte
//////////////////////////////////////////
size_t GifImpl::Size()
{
  std::ostringstream os;
  Write( os );

  return static_cast<size_t>( os.tellp() );
}
