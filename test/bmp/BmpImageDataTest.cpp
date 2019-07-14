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

// Unit tests for BmpImageData

#include "BmpImageDataTest.h"
#include "BmpImageData.h"
#include "BmpInfo.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpImageDataTest );

void BmpImageDataTest::testCtors()
{
  // ctor
  BmpImageData id1( *BmpInfo::Create( 1, 0, 0 ) );
  CPPUNIT_ASSERT( id1.Size() == 0 );
  CPPUNIT_ASSERT_THROW( id1[0], vp::Exception );

  BmpImageData id2( *BmpInfo::Create( 4, 10, 10 ) );
  auto Size = id2.Size();
  CPPUNIT_ASSERT( Size == 50 );
  for( uint8_t i = 0; i < Size; ++i )
  {
    CPPUNIT_ASSERT( id2[i] == 0 );  // default value
    id2[i] = i; 
  }

  // copy ctor
  BmpImageData id3 = id2;
  CPPUNIT_ASSERT( id3.Size() == 50 );
  for( size_t i = 0; i < Size; ++i )
    CPPUNIT_ASSERT( id3[i] == i );

  CPPUNIT_ASSERT_THROW( id3[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( id3[Size], vp::Exception );
}

void BmpImageDataTest::testInit()
{
  BmpImageData id( *BmpInfo::Create( 1, 1, 1 ) );
  CPPUNIT_ASSERT( id.Size() == 1 );

  id.Init( *BmpInfo::Create( 4, 10, 10 ) );
  CPPUNIT_ASSERT( id.Size() == 50 );
  for( size_t i = 0; i < id.Size(); ++i )
    CPPUNIT_ASSERT( id[i] == 0 );  // default value
}

void BmpImageDataTest::testIO()
{
  std::unique_ptr<BmpInfo> pBmpInfo = BmpInfo::Create( 4, 10, 10 );

  BmpImageData id1( *pBmpInfo );
  for( uint8_t i = 0; i < 50; ++i )
    id1[i] = i+1;

  // output
  std::stringstream strstream;
  strstream << id1;
  std::string str = strstream.str();

  CPPUNIT_ASSERT( str.size() == 80 );
  for( uint8_t i = 0; i < 80; ++i )
  {
    uint8_t j = i%8;
    if( j < 5 )
      CPPUNIT_ASSERT( str[i] != 0 );  // pixel bytes
    else 
      CPPUNIT_ASSERT( str[i] == 0 );  // padding bytes
  }

  // input
  BmpImageData id2( *pBmpInfo );
  for( uint8_t i = 0; i < 50; ++i )
    CPPUNIT_ASSERT( id2[i] == 0 );

  strstream >> id2;
  for( uint8_t i = 0; i < 50; ++i )
    CPPUNIT_ASSERT( id2[i] == i+1 );
}
