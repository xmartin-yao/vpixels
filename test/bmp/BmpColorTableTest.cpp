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

// Unit tests for BmpColorTable

#include "BmpColorTableTest.h"
#include "BmpColorTable.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpColorTableTest );

void BmpColorTableTest::testCtors()
{
  uint16_t Size = 256;

  // ctor
  BmpColorTable ct1( Size );
  CPPUNIT_ASSERT( ct1.Size() == Size );
  uint8_t B, G, R;
  for( uint16_t i = 0; i < Size; ++i )
  {
    ct1.Get( i, B, G, R );
    CPPUNIT_ASSERT( R == 0 );
    CPPUNIT_ASSERT( G == 0 );
    CPPUNIT_ASSERT( B == 0 );
  }

  B = 23;
  G = 123;
  R = 223;
  ct1.Set( 0, B, G, R );
  ct1.Get( 0, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( R == 223 );

  ct1.Set( Size-1, B, G, R );
  ct1.Get( Size-1, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( R == 223 );
  
  // copy ctor
  BmpColorTable ct2 = ct1;
  CPPUNIT_ASSERT( ct2.Size() == Size );

  ct2.Get( 0, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( R == 223 );

  ct2.Get( Size-1, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( R == 223 );
}

void BmpColorTableTest::testSize()
{
  uint8_t B, G, R;

  BmpColorTable ct( 16 );
  CPPUNIT_ASSERT( ct.Size() == 16 );

  ct.Set( 0, 253, 254, 255 );
  ct.Set( 15, 23, 24, 25 );

  ct.Get( 0, B, G, R );
  CPPUNIT_ASSERT( B == 253 );
  CPPUNIT_ASSERT( G == 254 );
  CPPUNIT_ASSERT( R == 255 );
  ct.Get( 15, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  // same size
  ct.Size( 16 );
  CPPUNIT_ASSERT( ct.Size() == 16 );

  // these entries keep unchanged
  ct.Get( 0, B, G, R );
  CPPUNIT_ASSERT( B == 253 );
  CPPUNIT_ASSERT( G == 254 );
  CPPUNIT_ASSERT( R == 255 );
  ct.Get( 15, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  // enlarge size
  ct.Size( 20 );
  CPPUNIT_ASSERT( ct.Size() == 20 );

  // these entries reset to default value
  ct.Get( 0, B, G, R );
  CPPUNIT_ASSERT( B == 0 );
  CPPUNIT_ASSERT( G == 0 );
  CPPUNIT_ASSERT( R == 0 );
  ct.Get( 15, B, G, R );
  CPPUNIT_ASSERT( B == 0 );
  CPPUNIT_ASSERT( G == 0 );
  CPPUNIT_ASSERT( R == 0 );

  // these entries get default value
  ct.Get( 16, B, G, R );
  CPPUNIT_ASSERT( B == 0 );
  CPPUNIT_ASSERT( G == 0 );
  CPPUNIT_ASSERT( R == 0 );
  ct.Get( 19, B, G, R );
  CPPUNIT_ASSERT( B == 0 );
  CPPUNIT_ASSERT( G == 0 );
  CPPUNIT_ASSERT( R == 0 );
}

void BmpColorTableTest::testGetSet()
{
  uint8_t B, G, R;

  // size = 0
  BmpColorTable ct( 0 );
  CPPUNIT_ASSERT_THROW( ct.Set( 1, 23, 24, 25 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( ct.Get( 0, B, G, R ), vp::Exception );

  // resize
  ct.Size( 16 );
  ct.Set( 1, 23, 24, 25 );
  ct.Get( 1, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  CPPUNIT_ASSERT_THROW( ct.Set( -1, 23, 24, 25 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( ct.Set( 16, 23, 24, 25 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( ct.Get( -1, B, G, R ), vp::Exception );
  CPPUNIT_ASSERT_THROW( ct.Get( 16, B, G, R ), vp::Exception );
}

void BmpColorTableTest::testInput()
{
  // set stream
  std::stringstream strstream;
  for( uint8_t i = 0; i < 8; ++i )
  {
    strstream << static_cast<uint8_t>(i+2);
    strstream << static_cast<uint8_t>(i+4);
    strstream << static_cast<uint8_t>(i+6);
    strstream << static_cast<uint8_t>(0);
  }

  // input color table
  BmpColorTable ct( 8 );
  strstream >> ct;
  for( uint8_t i = 0; i < 8; ++i )
  {
    uint8_t B, G, R;
    ct.Get( i, B, G, R );
    CPPUNIT_ASSERT( B == i+2 );
    CPPUNIT_ASSERT( G == i+4 );
    CPPUNIT_ASSERT( R == i+6 );
  }
}

void BmpColorTableTest::testOutput()
{
  // set color table
  BmpColorTable ct( 8 );
  CPPUNIT_ASSERT( ct.Size() == 8 );
  for( uint8_t i = 0; i < 8; ++i )
    ct.Set( i, i+1, i+3, i+5 );

  // output color table
  std::stringstream strstream;
  strstream << ct;
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 32 );
  for( uint8_t i = 0; i < 8; ++i )
  {
    CPPUNIT_ASSERT( str[4*i]   == i+1 );
    CPPUNIT_ASSERT( str[4*i+1] == i+3 );
    CPPUNIT_ASSERT( str[4*i+2] == i+5 );
    CPPUNIT_ASSERT( str[4*i+3] == 0 );
  }
}
