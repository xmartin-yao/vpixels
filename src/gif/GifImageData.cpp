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

#include "GifImageData.h"
#include "GifEncoder.h"
#include "GifDecoder.h"
#include "GifBlockIO.h"
#include "IOutil.h"
#include "Exception.h"
#include <istream>

//////////////////////////////////////////////////////////////////////
GifImageData::GifImageData( const uint8_t BitsPerPixel, const uint32_t Size )
 : m_BitsPerPixel( BitsPerPixel ),
   m_Pixels( U8String() )  // initial size is zero
{
  if( Size != 0)
    m_Pixels.resize( Size );  // resize() zero-initializes all elements
}

////////////////////////////////////////////
GifImageData::GifImageData( const GifImageData& other )
 : m_BitsPerPixel( other.m_BitsPerPixel ),
   m_Pixels( other.m_Pixels )
{
}

////////////////////////////////////////////////////////
GifImageData& GifImageData::operator=( const GifImageData& other )
{
  if( this != &other )
  {
    m_BitsPerPixel = other.m_BitsPerPixel;
    m_Pixels = other.m_Pixels;
  }

  return *this;
}

///////////////////////////////////////
void GifImageData::SetAllPixels( const uint8_t ColorIndex )
{
  m_Pixels.assign( m_Pixels.size(), ColorIndex );
}

///////////////////////////////////////////////////////////
void GifImageData::SetPixel( const uint32_t Index, const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( Index >= m_Pixels.size() )
    VP_THROW( "index out of range" )
#endif

  m_Pixels[Index] = ColorIndex;
}

//////////////////////////////////////////////////////
uint8_t GifImageData::GetPixel( const uint32_t Index ) const
{
#ifndef VP_EXTENSION
  if( Index >= m_Pixels.size() )
    VP_THROW( "index out of range" )
#endif

  return m_Pixels[Index];
}

//////////////////////////////////////
void GifImageData::Init( size_t Capacity )
{
  // clean up
  if( m_Pixels.size() != 0 )
    m_Pixels.clear();

  // reserve space for decoding
  m_Pixels.reserve( Capacity );
}

///////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, GifImageData& ImageData )
{
  // read bpp
  ImageData.m_BitsPerPixel = is.get(); 

  // read codes
  U8String Codes;
  GifBlockIO::ReadSubBlocks( is, Codes );

  // decoding
  GifDecoder Decoder{};  // through still causes warnings of -Weffc++,
                         // with empty initializer the generated default ctor
                         // is able to initialize all data members.
  Decoder( ImageData.m_BitsPerPixel, Codes, ImageData.m_Pixels );

  return is;
}

//////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const GifImageData& ImageData )
{
  // write bpp
  IOutil::Write( os, ImageData.m_BitsPerPixel );

  // reserve space for codes
  U8String Codes;
  Codes.reserve( ImageData.m_Pixels.size() );

  // encoding
  GifEncoder Encoder{};  // through still causes warnings of -Weffc++,
                         // with empty initializer the generated default ctor
                         // is able to initialize all data members.
  Encoder( ImageData.m_BitsPerPixel, ImageData.m_Pixels, Codes );

  // write codes
  GifBlockIO::WriteSubBlocks( os, Codes );

  return os;
}
