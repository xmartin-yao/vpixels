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

#include "GifGraphicsControlExt.h"
#include "GifBlockIO.h"
#include "Exception.h"

/////////////////////////////////
GifGraphicsControlExt::GifGraphicsControlExt()
 : GifExtension( 0xF9 ),
   m_PackedByte( 0x04 ),   // 00000100: disposal method = 1
   m_DelayTime( 0x0000 ),  // 0 millisecond
   m_TransColor( 0x00 )
{
}

//////////////////////////////////////////////////////////////
GifGraphicsControlExt::GifGraphicsControlExt( const GifGraphicsControlExt& other )
 : GifExtension( other.m_Label ), 
   m_PackedByte( other.m_PackedByte ),
   m_DelayTime( other.m_DelayTime ),
   m_TransColor( other.m_TransColor )
{
}

//////////////////////////////////////////////////////////////////////
GifGraphicsControlExt& GifGraphicsControlExt::operator=( const GifGraphicsControlExt& other )
{
  if( this != &other )
  {
    this->m_PackedByte = other.m_PackedByte;
    this->m_DelayTime  = other.m_DelayTime;
    this->m_TransColor = other.m_TransColor;
  }

  return *this;
}

/////////////////////////////////////////
GifComponent* GifGraphicsControlExt::Clone() const
{
  return new GifGraphicsControlExt( *this );
}

/////////////////////////////////////////////
void GifGraphicsControlExt::Read( std::istream& is )
{
  // m_ID and m_Label have already been read

  U8String Data;
  GifBlockIO::ReadSubBlocks( is, Data );
  if( Data.size() != 4 )
    VP_THROW( "Data size not equal to 4." );

  m_PackedByte = Data[0];
  m_DelayTime  = (Data[2] << 8) | Data[1];
  m_TransColor = Data[3];
}

//////////////////////////////////////////////////////////////////////
void GifGraphicsControlExt::Write( std::ostream& os ) const
{
  os.put( m_ID );
  os.put( m_Label );

  U8String Data;
  Data += m_PackedByte;
  Data += static_cast<uint8_t>(m_DelayTime);
  Data += static_cast<uint8_t>(m_DelayTime >> 8);
  Data += m_TransColor;
  GifBlockIO::WriteSubBlocks( os, Data );
}
