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

#include "BmpInfoHeader.h"
#include "BmpInfo.h"
#include "IOutil.h"

#include <iostream>

/////////////////////////////////////////////////
BmpInfoHeader::BmpInfoHeader( const BmpInfo& BmpInfo )
 : m_Size( 40 ),
   m_Width( BmpInfo.Width() ),
   m_Height( BmpInfo.Height() ),
   m_Planes( 1 ),
   m_BitsPerPixel( BmpInfo.BitsPerPixel() ), 
   m_Compression( 0 ),
   m_ImageDataSize( BmpInfo.ImageDataSize() ),
   m_Xresolution( 3780 ),
   m_Yresolution( 3780 ),
   m_ColorsUsed( 0 ),
   m_ImportantColors( 0 )
{
}

////////////////////////////////////////
BmpInfoHeader::BmpInfoHeader( const BmpInfoHeader& other )
 : m_Size( other.m_Size ),
   m_Width( other.m_Width ),
   m_Height( other.m_Height ),
   m_Planes( other.m_Planes ),
   m_BitsPerPixel( other.m_BitsPerPixel ),
   m_Compression( other.m_Compression ),
   m_ImageDataSize( other.m_ImageDataSize ),
   m_Xresolution( other.m_Xresolution ),
   m_Yresolution( other.m_Yresolution ),
   m_ColorsUsed( other.m_ColorsUsed ),
   m_ImportantColors( other.m_ImportantColors )
{
}

/////////////////////////////
BmpInfoHeader::operator bool() const
{
  return m_Size == 40;  // size of BITMAPINFOHEADER must be 40
}

///////////////////////////////////////////////
bool BmpInfoHeader::Check( BmpInfo& BmpInfo ) const
{
  if( !BmpInfo::Supported( static_cast<uint8_t>(m_BitsPerPixel) ) )
    return false;

  if( m_Width != BmpInfo.Width() )
    return false;

  if( m_Height != BmpInfo.Height() )
   	return false;

  if( m_BitsPerPixel != BmpInfo.BitsPerPixel() )
   	return false;

  if( m_ImageDataSize != BmpInfo.ImageDataSize() )
   	return false;

  return true;
}

///////////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, BmpInfoHeader& ih )
{
  IOutil::Read( is, ih.m_Size );
  IOutil::Read( is, ih.m_Width );
  IOutil::Read( is, ih.m_Height );
  IOutil::Read( is, ih.m_Planes );
  IOutil::Read( is, ih.m_BitsPerPixel );
  IOutil::Read( is, ih.m_Compression );
  IOutil::Read( is, ih.m_ImageDataSize );
  IOutil::Read( is, ih.m_Xresolution );
  IOutil::Read( is, ih.m_Yresolution );
  IOutil::Read( is, ih.m_ColorsUsed );
  IOutil::Read( is, ih.m_ImportantColors );

  return is;
}

///////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const BmpInfoHeader& ih )
{
  IOutil::Write( os, ih.m_Size );
  IOutil::Write( os, ih.m_Width );
  IOutil::Write( os, ih.m_Height );
  IOutil::Write( os, ih.m_Planes );
  IOutil::Write( os, ih.m_BitsPerPixel );
  IOutil::Write( os, ih.m_Compression );
  IOutil::Write( os, ih.m_ImageDataSize );
  IOutil::Write( os, ih.m_Xresolution );
  IOutil::Write( os, ih.m_Yresolution );
  IOutil::Write( os, ih.m_ColorsUsed );
  IOutil::Write( os, ih.m_ImportantColors );

  return os;
}
