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

#include "BmpFileHeader.h"
#include "BmpInfo.h"
#include "IOutil.h"
#include <iostream>

///////////////////////////////////
BmpFileHeader::BmpFileHeader( const BmpInfo& BmpInfo )
 : m_Identifier( m_BmpID ),
   m_Filesize( 0 ),
   m_Reserved1( 0 ),
   m_Reserved2( 0 ),
   m_ImageOffset( 0 )
{
  // size of BITMAPFILEHEADER + BITMAPINFOHEADER = 14 + 40 = 54 bytes
  m_ImageOffset = 54u + 4u*BmpInfo.ColorTableSize();
  m_Filesize = m_ImageOffset + BmpInfo.ImageDataSize();
}

//////////////////////////////////////////
BmpFileHeader::BmpFileHeader( const BmpFileHeader& other )
 : m_Identifier( other.m_Identifier ),
   m_Filesize( other.m_Filesize ),
   m_Reserved1( other.m_Reserved1 ),
   m_Reserved2( other.m_Reserved2 ),
   m_ImageOffset( other.m_ImageOffset )
{
}

////////////////////////////
BmpFileHeader::operator bool() const
{
  return m_Identifier == m_BmpID;
}

/////////////////////////////////////////////
bool BmpFileHeader::Check( const BmpInfo& BmpInfo ) const
{
  return BmpInfo.ImageDataSize() == (m_Filesize - m_ImageOffset);
}

///////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, BmpFileHeader& fh )
{
  IOutil::Read( is, fh.m_Identifier );
  IOutil::Read( is, fh.m_Filesize );
  IOutil::Read( is, fh.m_Reserved1 );
  IOutil::Read( is, fh.m_Reserved2 );
  IOutil::Read( is, fh.m_ImageOffset );

  return is;
}

////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const BmpFileHeader& fh )
{
  IOutil::Write( os, fh.m_Identifier );
  IOutil::Write( os, fh.m_Filesize );
  IOutil::Write( os, fh.m_Reserved1 );
  IOutil::Write( os, fh.m_Reserved2 );
  IOutil::Write( os, fh.m_ImageOffset ); 

  return os;
}
