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

#include <iostream>
#include "GifColorTableTest.h"
#include "GifColorTable.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifColorTableTest );

void GifColorTableTest::testCtors()
{
  uint16_t Size = 256;

  // ctor
  GifColorTable ct1( Size );
  CPPUNIT_ASSERT( ct1.Size() == Size );
  uint8_t R, G, B;
  for( uint16_t i = 0; i < Size; ++i )
  {
    ct1.Get( i, R, G, B );
    CPPUNIT_ASSERT( R == 0xFF );
    CPPUNIT_ASSERT( G == 0xFF );
    CPPUNIT_ASSERT( B == 0xFF );
  }

  // caveat: these still can pass
  ct1.Get( -1, R, G, B );   // -1 implicitly converted to 255
  ct1.Get( Size, R, G, B ); // uint16_t 256 implicitly converted to 0

  R = 23;
  G = 123;
  B = 223;
  ct1.Set( 0, R, G, B );
  ct1.Get( 0, R, G, B );
  CPPUNIT_ASSERT( R == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( B == 223 );

  ct1.Set( Size-1, R, G, B );
  ct1.Get( Size-1, R, G, B );
  CPPUNIT_ASSERT( R == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( B == 223 );
  
  // copy ctor
  GifColorTable ct2 = ct1;
  CPPUNIT_ASSERT( ct2.Size() == Size);

  ct2.Get( 0, R, G, B );
  CPPUNIT_ASSERT( R == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( B == 223 );

  ct2.Get( Size-1, R, G, B );
  CPPUNIT_ASSERT( R == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( B == 223 );

  // assignment operator
  GifColorTable ct3(4);
  CPPUNIT_ASSERT( ct3.Size() == 4 );
  ct3 = ct1;
  CPPUNIT_ASSERT( ct3.Size() == Size );

  ct3.Get( 0, R, G, B );
  CPPUNIT_ASSERT( R == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( B == 223 );

  ct3.Get( Size-1, R, G, B );
  CPPUNIT_ASSERT( R == 23 );
  CPPUNIT_ASSERT( G == 123 );
  CPPUNIT_ASSERT( B == 223 );
}

void GifColorTableTest::testInput()
{
  // set stream
  std::stringstream stream;
  for( uint8_t i = 0; i < 8; ++i )
  {
    stream << static_cast<uint8_t>(i+2);
    stream << static_cast<uint8_t>(i+4);
    stream << static_cast<uint8_t>(i+6);
  }

  // input color table
  GifColorTable ct( 8 );
  stream >> ct;
  for( uint8_t i = 0; i < 8; ++i )
  {
    uint8_t R, G, B;
    ct.Get( i, R, G, B );
    CPPUNIT_ASSERT( R == i+2 );
    CPPUNIT_ASSERT( G == i+4 );
    CPPUNIT_ASSERT( B == i+6 );
  }
}

void GifColorTableTest::testOutput()
{
  // set color table
  GifColorTable ct( 8 );
  for( uint8_t i = 0; i < 8; ++i )
    ct.Set( i, i+1, i+3, i+5 );
  CPPUNIT_ASSERT( ct.Size() == 8 );

  // output color table
  std::stringstream stream;
  stream << ct;
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 24 );
  for( uint8_t i = 0; i < 8; ++i )
  {
    CPPUNIT_ASSERT( str[3*i]   == i+1 );
    CPPUNIT_ASSERT( str[3*i+1] == i+3 );
    CPPUNIT_ASSERT( str[3*i+2] == i+5 );
  }
}

void GifColorTableTest::testRoundup()
{
  CPPUNIT_ASSERT( GifColorTable::Roundup( 1 ) == 0 );
  CPPUNIT_ASSERT( GifColorTable::Roundup( 2 ) == 0 );

  CPPUNIT_ASSERT( GifColorTable::Roundup( 3 ) == 1 );
  CPPUNIT_ASSERT( GifColorTable::Roundup( 4 ) == 1 );

  uint16_t s;
  for( s = 5; s <= 8; ++s )
    CPPUNIT_ASSERT( GifColorTable::Roundup( s ) == 2 );

  for( s = 9; s <= 16; ++s )
    CPPUNIT_ASSERT( GifColorTable::Roundup( s ) == 3 );

  for( s = 17; s <= 32; ++s )
    CPPUNIT_ASSERT( GifColorTable::Roundup( s ) == 4 );

  for( s = 33; s <= 64; ++s )
    CPPUNIT_ASSERT( GifColorTable::Roundup( s ) == 5 );

  for( s = 65; s <= 128; ++s )
    CPPUNIT_ASSERT( GifColorTable::Roundup( s ) == 6 );

  for( s = 129; s <= 256; ++s )
    CPPUNIT_ASSERT( GifColorTable::Roundup( s ) == 7 );
}

void GifColorTableTest::testSize()
{
  // initial size = 2^4 = 16
  uint8_t PackeByte = 0xB3;  // 10110011
  GifColorTable ct( 16 );
  CPPUNIT_ASSERT( ct.Size() == 16 );
  uint8_t R, G, B;
  uint8_t i;
  for( i = 0; i < 16; ++i )
  {
    ct.Get( i, R, G, B );
    CPPUNIT_ASSERT( R == 0xFF );
    CPPUNIT_ASSERT( G == 0xFF );
    CPPUNIT_ASSERT( B == 0xFF );
  }
  
  // change size to 3, actually round up to 4 = 2^2
  CPPUNIT_ASSERT( ct.Size( 3, PackeByte ) );
  CPPUNIT_ASSERT( ct.Size() == 4 );
  CPPUNIT_ASSERT( PackeByte == 0xB1 );  // 10110001
  for( i = 0; i < 4; ++i )
    ct.Set( i, 0xF1, 0xF2, 0xF3 );

  // change size to 5, actually round up to 8 = 2^3
  CPPUNIT_ASSERT( ct.Size( 5, PackeByte ) );
  CPPUNIT_ASSERT( ct.Size() == 8 );
  CPPUNIT_ASSERT( PackeByte == 0xB2 );  // 101100010
  for( i = 0; i < 8; ++i )
  {
    ct.Get( i, R, G, B );
    if( i < 4)  // these entries kept unchanged
    {
      CPPUNIT_ASSERT( R == 0xF1 );
      CPPUNIT_ASSERT( G == 0xF2 );
      CPPUNIT_ASSERT( B == 0xF3 );
    }
    else
    {
      CPPUNIT_ASSERT( R == 0xFF );
      CPPUNIT_ASSERT( G == 0xFF );
      CPPUNIT_ASSERT( B == 0xFF );
    }
  }

  // change size to 6, no effect
  CPPUNIT_ASSERT( ct.Size( 6, PackeByte ) == false );
  CPPUNIT_ASSERT( ct.Size() == 8 );
  CPPUNIT_ASSERT( PackeByte == 0xB2 );  // 101100010
  for( i = 0; i < 8; ++i )
  {
    ct.Get( i, R, G, B );
    if( i < 4)
    {
      CPPUNIT_ASSERT( R == 0xF1 );
      CPPUNIT_ASSERT( G == 0xF2 );
      CPPUNIT_ASSERT( B == 0xF3 );
    }
    else
    {
      CPPUNIT_ASSERT( R == 0xFF );
      CPPUNIT_ASSERT( G == 0xFF );
      CPPUNIT_ASSERT( B == 0xFF );
    }
  }

  // change size to 0
  CPPUNIT_ASSERT( ct.Size( 0, PackeByte ) );
  CPPUNIT_ASSERT( ct.Size() == 0 );
  CPPUNIT_ASSERT( PackeByte == 0x30 );  // 00110000
  CPPUNIT_ASSERT_THROW( ct.Set( 0, 0xF1, 0xF2, 0xF3 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( ct.Get( 0, R, G, B ), vp::Exception );

  // change size to 0, no effect
  CPPUNIT_ASSERT( ct.Size( 0, PackeByte ) == false );
  CPPUNIT_ASSERT( ct.Size() == 0 );
  CPPUNIT_ASSERT( PackeByte == 0x30 );  // 00110000

  // change size to 2
  CPPUNIT_ASSERT( ct.Size( 2, PackeByte ) );
  CPPUNIT_ASSERT( PackeByte == 0xB0 );  // 101100000
  CPPUNIT_ASSERT( ct.Size() == 2 );
  for( i = 0; i < 2; ++i )
  {
    ct.Get( i, R, G, B );
    CPPUNIT_ASSERT( R == 0xFF );
    CPPUNIT_ASSERT( G == 0xFF );
    CPPUNIT_ASSERT( B == 0xFF );
  }

  // this function reset entries to default value and doesn't verify
  // whether or not the passed-in size is power of 2, 
  ct.Size( 3 );
  CPPUNIT_ASSERT( ct.Size() == 3 );
  ct.Get( 2, R, G, B ); 
  CPPUNIT_ASSERT( R == 0xFF );
  CPPUNIT_ASSERT( G == 0xFF );
  CPPUNIT_ASSERT( B == 0xFF );
  ct.Set( 2, 0xF1, 0xF2, 0xF3 );
  ct.Get( 2, R, G, B );
  CPPUNIT_ASSERT( R == 0xF1 );
  CPPUNIT_ASSERT( G == 0xF2 );
  CPPUNIT_ASSERT( B == 0xF3 );

  CPPUNIT_ASSERT_THROW( ct.Set( 3, 0xF1, 0xF2, 0xF3 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( ct.Get( 3, R, G, B ), vp::Exception );
}
