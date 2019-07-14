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

#include "GifHeader.h"
#include <istream>
#include <algorithm>


/////////////////
GifHeader::GifHeader( GifVersion Version )
 : m_Signature{'G', 'I', 'F'},
   m_Version{'8', ' ', 'a'}
{
  if( Version == GifVersion::V89a )
    m_Version[1] = '9';
  else if( Version == GifVersion::V87a )
    m_Version[1] = '7';
}

///////////////////////
GifHeader::GifHeader( const GifHeader& other )
{
  std::copy_n( other.m_Signature, 3, m_Signature );
  std::copy_n( other.m_Version, 3, m_Version );
}

///////////////////////////
GifHeader::operator bool() const
{
  if( !std::equal( m_Signature, m_Signature + 3, "GIF") )
    return false;

  if( std::equal( m_Version, m_Version + 3, "89a" ) )
    return true;

  if( std::equal( m_Version, m_Version + 3, "87a" ) )
    return true;

  return false;
}

///////////////////////////////////
GifVersion GifHeader::Version() const
{
  GifVersion Verion = GifVersion::Invalid;
  if( m_Version[0] == '8' && m_Version[2] == 'a' )
  {
    if( m_Version[1] == '9' )
      Verion = GifVersion::V89a;
    else if( m_Version[1] == '7' )
      Verion = GifVersion::V87a;
  }

  return Verion;
}

///////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const GifHeader& Header )
{
  os.write( Header.m_Signature, 3 );
  os.write( Header.m_Version, 3 );

  return os;
}

////////////////////////////////////////////////////////////
std::istream& operator>>( std::istream& is, GifHeader& Header )
{
  is.read( Header.m_Signature, 3 );
  is.read( Header.m_Version, 3 );

  return is;
}
