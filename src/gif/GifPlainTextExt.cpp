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

#include "GifPlainTextExt.h"
#include "GifBlockIO.h"

///////////////////////////////////
GifPlainTextExt::GifPlainTextExt()
  : GifExtension( 0x01 )
{
}

/////////////////////////////////////////////////////
GifPlainTextExt::GifPlainTextExt( const GifPlainTextExt& other )
  : GifExtension( other.m_Label ),
    m_Data( other.m_Data ),
    m_Text( other.m_Text )
{
}

////////////////////////////////////////////
GifComponent* GifPlainTextExt::Clone() const
{
  return( new GifPlainTextExt( *this ) );
}

/////////////////////////////////////////
void GifPlainTextExt::Read( std::istream& is )
{
  // m_ID and m_Label have already been read
  GifBlockIO::ReadBlock( is, m_Data );
  GifBlockIO::ReadSubBlocks( is, m_Text );
}

////////////////////////////////////////////////
void GifPlainTextExt::Write( std::ostream& os ) const
{
  os.put( m_ID );
  os.put( m_Label );
  GifBlockIO::WriteBlock( os, m_Data );
  GifBlockIO::WriteSubBlocks( os, m_Text );
}
