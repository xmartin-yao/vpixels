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

#include "GifApplicationExt.h"
#include "GifBlockIO.h"
#include "IOutil.h"
#include <fstream>

////////////////////////////////
GifApplicationExt::GifApplicationExt()
  : GifExtension( 0xFF ),
    m_AppInfo( "NETSCAPE2.0" ),
    m_AppData{0x01, 0x00, 0x00}
{
}

///////////////////////////////////////////////////////////
GifApplicationExt::GifApplicationExt( const GifApplicationExt& other )
  : GifExtension( other.m_Label ),
    m_AppInfo( other.m_AppInfo ),
    m_AppData( other.m_AppData )
{
}

////////////////////////////////////// 
uint16_t GifApplicationExt::LoopTimes() const
{
  // a sub-block of looping times
  if( m_AppData.size() == 3 && m_AppData[0] == 0x01 )
    return (m_AppData[2] << 8) | m_AppData[1];
  else
    return 0;
}

/////////////////////////////////////////////////////
bool GifApplicationExt::LoopTimes( const uint16_t LoopTimes )
{
  // a sub-block of looping times
  if( m_AppData.size() == 3 && m_AppData[0] == 0x01 )
  {
    m_AppData[1] = 0x00FF & LoopTimes;
    m_AppData[2] = LoopTimes >> 8;
    return true;
  }
  else
    return false;
}

////////////////////////////////////////
GifComponent* GifApplicationExt::Clone() const
{
  return new GifApplicationExt( *this );
}

////////////////////////////////////////////////////
void GifApplicationExt::Read( std::istream& is )
{
  // m_ID and m_Label have already been read

  // application info block
  GifBlockIO::ReadBlock( is, m_AppInfo );

  // application data sub-blocks
  GifBlockIO::ReadSubBlocks( is, m_AppData );
}

////////////////////////////////////////////////////////
void GifApplicationExt::Write( std::ostream& os ) const
{
  IOutil::Write( os, m_ID );
  IOutil::Write( os, m_Label );

  // application info block
  GifBlockIO::WriteBlock( os, m_AppInfo );

  // application data sub-blocks
  GifBlockIO::WriteSubBlocks( os, m_AppData );
}
