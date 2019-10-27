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

#include "BmpImpl.h"
#include "Exception.h"
#include <fstream>

/////////////////////////////////////////
BmpImpl::BmpImpl( const uint8_t BitsPerPixel, const int32_t Width, const int32_t Height )
 : m_pBmpInfo  ( BmpInfo::Create(BitsPerPixel, Width, Height) ),
   m_FileHeader( *m_pBmpInfo ),
   m_InfoHeader( *m_pBmpInfo ),
   m_ColorTable( m_pBmpInfo->ColorTableSize() ),
   m_ImageData ( *m_pBmpInfo )
{
}

/////////////////////////////////////////
BmpImpl::BmpImpl( const BmpImpl& other )
 : m_pBmpInfo  ( other.m_pBmpInfo->Clone() ),
   m_FileHeader( other.m_FileHeader ),
   m_InfoHeader( other.m_InfoHeader ),
   m_ColorTable( other.m_ColorTable ),
   m_ImageData ( other.m_ImageData )
{
}

//////////////////////////////////////////////
bool BmpImpl::Import( const std::string& FileName )
{
  std::ifstream File;

  // check existence
  File.open( FileName, std::ios::binary );
  if( !File.is_open() )
    return false;

  try
  {
    // read from file
    Read( File );

    if( File.fail() )
    {
      File.close();
      throw vp::Exception( "not a valid BMP file" );
    }

    File.close();
    return true;
  }
  catch( const vp::Exception& )
  {
    File.close();
    throw;
  }
}

///////////////////////////////////////////////////////////////
bool BmpImpl::Export( const std::string& FileName, const bool OverWrite ) const
{
  // check existence
  std::fstream File;
  bool Exist = false;
  File.open( FileName, std::ios::in|std::ios::binary );
  if( File.is_open() ) Exist = true;
  File.close();

  // over-writing not allowed
  if( Exist && !OverWrite )
    return false;

  // write to file
  File.open( FileName, std::ios::out|std::ios::binary );
  Write( File );
  File.close();

  return true;
}

/////////////////////////////////////////////////////////////
void BmpImpl::SetAllPixels( const uint8_t Blue, const uint8_t Green, const uint8_t Red )
{
#ifndef VP_EXTENSION
  if( m_pBmpInfo->ColorTableSize() != 0 )
    VP_THROW( "indexed BMP must use color index" );
#endif

  size_t i = 0;
  while( i < m_ImageData.Size() )
  {
    m_ImageData[i++] = Blue;
    m_ImageData[i++] = Green;
    m_ImageData[i++] = Red;   
  }
}

///////////////////////////////////////////////////////
void BmpImpl::SetPixel( const int32_t X, const int32_t Y, const uint8_t Blue,
                        const uint8_t Green, const uint8_t Red )
{
#ifndef VP_EXTENSION
  if( m_pBmpInfo->ColorTableSize() != 0 )
    VP_THROW( "indexed BMP must use color index" );
#endif

  uint32_t ByteIndex;
  uint8_t  BitIndex;
  m_pBmpInfo->ByteArrayIndices( X, Y, ByteIndex, BitIndex );
  m_ImageData[ByteIndex]   = Blue;
  m_ImageData[++ByteIndex] = Green;
  m_ImageData[++ByteIndex] = Red;   
}

///////////////////////////////////////////////////
void BmpImpl::GetPixel( const int32_t X, const int32_t Y,
                        uint8_t& Blue, uint8_t& Green, uint8_t& Red ) const
{
#ifndef VP_EXTENSION
  if( m_pBmpInfo->ColorTableSize() != 0 )
    VP_THROW( "indexed BMP must use color index" );
#endif

  uint32_t ByteIndex;
  uint8_t  BitIndex;
  m_pBmpInfo->ByteArrayIndices( X, Y, ByteIndex, BitIndex );
  Blue  = m_ImageData[ByteIndex];
  Green = m_ImageData[++ByteIndex];
  Red   = m_ImageData[++ByteIndex];   
}

///////////////////////////////////////////////
void BmpImpl::SetAllPixels( const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( m_pBmpInfo->ColorTableSize() == 0 )
    VP_THROW( "not an indexed BMP" );
#endif

  int32_t Width  = m_pBmpInfo->Width();
  int32_t Height = m_pBmpInfo->Height();
  for( int32_t Y = 0; Y < Height; ++Y )
  {
    for( int32_t X = 0; X < Width; ++X )
    {
      uint32_t ByteIndex;
      uint8_t  BitIndex;
      m_pBmpInfo->ByteArrayIndices( X, Y, ByteIndex, BitIndex );
      m_pBmpInfo->SetColorIndex( m_ImageData[ByteIndex], BitIndex, ColorIndex );
    }
  }
}

///////////////////////////////////////////////
void BmpImpl::SetPixel( const int32_t X, const int32_t Y, const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( m_pBmpInfo->ColorTableSize() == 0 )
    VP_THROW( "not an indexed BMP" );
#endif

  uint32_t ByteIndex;
  uint8_t  BitIndex;
  m_pBmpInfo->ByteArrayIndices( X, Y, ByteIndex, BitIndex );
  m_pBmpInfo->SetColorIndex( m_ImageData[ByteIndex], BitIndex, ColorIndex );
}

///////////////////////////////////////////////
uint8_t BmpImpl::GetPixel( const int32_t X, const int32_t Y ) const
{
#ifndef VP_EXTENSION
  if( m_pBmpInfo->ColorTableSize() == 0 )
    VP_THROW( "not an indexed BMP" );
#endif

  uint32_t ByteIndex;
  uint8_t  BitIndex;
  m_pBmpInfo->ByteArrayIndices( X, Y, ByteIndex, BitIndex );
  return m_pBmpInfo->GetColorIndex( m_ImageData[ByteIndex], BitIndex );
}

//////////////////////////////////////////////////////////////
void BmpImpl::SetColorTable( const uint8_t ColorIndex, const uint8_t Blue,
                             const uint8_t Green, const uint8_t Red )
{
#ifndef VP_EXTENSION
  if( ColorTableSize() == 0 )
    VP_THROW( "not an indexed BMP" );
#endif

  m_ColorTable.Set( ColorIndex, Blue, Green, Red );
}

//////////////////////////////////////////////////////////////
void BmpImpl::GetColorTable( const uint8_t ColorIndex, uint8_t& Blue,
                             uint8_t& Green, uint8_t& Red ) const
{
#ifndef VP_EXTENSION
  if( ColorTableSize() == 0 )
    VP_THROW( "not an indexed BMP" );
#endif

  m_ColorTable.Get( ColorIndex, Blue, Green, Red );
}

////////////////////////////////////////////
void BmpImpl::Read( std::istream& is )
{
  is >> m_FileHeader;
  if( !m_FileHeader )
    throw vp::Exception( "not a BMP file" );

  is >> m_InfoHeader;
  if( !m_InfoHeader )
    throw vp::Exception( "wrong file header size" );

  if( !BmpInfo::Supported(m_InfoHeader.BitsPerPixel()) )
    throw vp::Exception( "color depth not supported" );

  m_pBmpInfo = BmpInfo::Create( m_InfoHeader.BitsPerPixel(), m_InfoHeader.Width(), 
                                m_InfoHeader.Height() );

  if( !m_FileHeader.Check( *m_pBmpInfo ) )
    throw vp::Exception( "invalid file header" );

  if( !m_InfoHeader.Check( *m_pBmpInfo ) )
    throw vp::Exception( "invalid info header" );

  m_ColorTable.Size( m_pBmpInfo->ColorTableSize() );
  m_ImageData.Init( *m_pBmpInfo );
  is >> m_ColorTable >> m_ImageData;
}

/////////////////////////////////////////////
void BmpImpl::Write( std::ostream& os ) const
{
  os << m_FileHeader << m_InfoHeader << m_ColorTable << m_ImageData;
}
