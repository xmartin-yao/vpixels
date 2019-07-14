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

#include "GifCommentExt.h"
#include "GifBlockIO.h"

////////////////////////
GifCommentExt::GifCommentExt()
  : GifExtension( 0xFE )
{
}

////////////////////////////////////////////////
GifCommentExt::GifCommentExt( const GifCommentExt& other )
 : GifExtension( other.m_Label ),
   m_Comment( other.m_Comment )
{
}

////////////////////////////////////
GifComponent* GifCommentExt::Clone() const
{
  return new GifCommentExt( *this );
}

//////////////////////////////////////////////
void GifCommentExt::Read( std::istream& is )
{
  // m_ID and m_Label have already been read
  GifBlockIO::ReadSubBlocks( is, m_Comment );
}

/////////////////////////////////////////////////////
void GifCommentExt::Write( std::ostream& os ) const
{
  os.put( m_ID );
  os.put( m_Label );
  GifBlockIO::WriteSubBlocks( os, m_Comment );
}
