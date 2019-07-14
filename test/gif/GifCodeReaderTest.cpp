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

#include "GifCodeReaderTest.h"
#include "GifCodeReader.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifCodeReaderTest );

void GifCodeReaderTest::test1Bit()
{
  U8String Codes;
  Codes += 0x89;  // 10001001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(1) == 1 );
  CPPUNIT_ASSERT( ReadCode(1) == 0 );
  CPPUNIT_ASSERT( ReadCode(1) == 0 );
  CPPUNIT_ASSERT( ReadCode(1) == 1 );
  CPPUNIT_ASSERT( ReadCode(1) == 0 );
  CPPUNIT_ASSERT( ReadCode(1) == 0 );
  CPPUNIT_ASSERT( ReadCode(1) == 0 );
  CPPUNIT_ASSERT( ReadCode(1) == 1 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test2Bit()
{
  U8String Codes;
  Codes += 0xC9;  // 11,00,10,01
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(2) == 1 );
  CPPUNIT_ASSERT( ReadCode(2) == 2 );
  CPPUNIT_ASSERT( ReadCode(2) == 0 );
  CPPUNIT_ASSERT( ReadCode(2) == 3 );
  CPPUNIT_ASSERT_THROW( ReadCode(2), vp::Exception );
}

void GifCodeReaderTest::test3Bit()
{
  U8String Codes;
  Codes += 0xF9;  // 11,111,001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(3) == 1 );
  CPPUNIT_ASSERT( ReadCode(3) == 7 );
  CPPUNIT_ASSERT( ReadCode(2) == 3 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test4Bit()
{
  U8String Codes;
  Codes += 0xF9;  // 1111,1001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(4) == 9 );
  CPPUNIT_ASSERT( ReadCode(4) == 15 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test5Bit()
{
  U8String Codes;
  Codes += 0xF9;  // 111,11001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(5) == 25 );
  CPPUNIT_ASSERT( ReadCode(3) == 7 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test6Bit()
{
  U8String Codes;
  Codes += 0xF9;  // 11,111001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(6) == 57 );
  CPPUNIT_ASSERT( ReadCode(2) == 3 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test7Bit()
{
  U8String Codes;
  Codes += 0xF9;  // 1,1111001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(7) == 121 );
  CPPUNIT_ASSERT( ReadCode(1) == 1 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test8Bit()
{
  U8String Codes;
  Codes += 0xF9;  // 11111001
  CPPUNIT_ASSERT( Codes.size() == 1 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(8) == 0xF9 );
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test9Bit()
{
  U8String Codes;
  Codes += 0x99;  // 10011001
  Codes += 0xF9;  // 1111100,1
  CPPUNIT_ASSERT( Codes.size() == 2 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(9) == 409 );  // 110011001
  CPPUNIT_ASSERT( ReadCode(7) == 124 );  // 1111100
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test10Bit()
{
  U8String Codes;
  Codes += 0x99;  // 10011001
  Codes += 0xF9;  // 111110,01
  CPPUNIT_ASSERT( Codes.size() == 2 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(10) == 409 );  // 0110011001
  CPPUNIT_ASSERT( ReadCode(6) == 62 );    // 111110
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test11Bit()
{
  U8String Codes;
  Codes += 0x99;  // 10011001
  Codes += 0xF9;  // 11111,001
  CPPUNIT_ASSERT( Codes.size() == 2 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(11) == 409 );  // 00110011001
  CPPUNIT_ASSERT( ReadCode(5) == 31 );    // 11111
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::test12Bit()
{
  U8String Codes;
  Codes += 0x99;  // 10011001
  Codes += 0xF9;  // 1111,1001
  CPPUNIT_ASSERT( Codes.size() == 2 );

  GifCodeReader ReadCode( Codes );

  CPPUNIT_ASSERT( ReadCode(12) == 2457 );  // 100110011001
  CPPUNIT_ASSERT( ReadCode(4) == 15 );     // 1111
  CPPUNIT_ASSERT_THROW( ReadCode(1), vp::Exception );
}

void GifCodeReaderTest::testMixed()
{
  U8String Codes;

  // codes: 1,3,5,7,9,2,4,6,8,10,12,7
  Codes += 0xAD;  // 101,011,01
  Codes += 0x4E;  // 01,00111,0
  Codes += 0x22;  // 0010,0010
  Codes += 0x20;  // 00100,000
  Codes += 0x30;  // 00110,000
  Codes += 0x80;  // 1000,0000
  Codes += 0x80;  // 10,000000
  Codes += 0x02;  // 00000010
  Codes += 0x18;  // 0001100,0
  Codes += 0x80;  // 100,00000
  CPPUNIT_ASSERT( Codes.size() == 10 );

  GifCodeReader ReadCode( Codes );
  
  CPPUNIT_ASSERT( ReadCode(2) == 1 );    // 01
  CPPUNIT_ASSERT( ReadCode(3) == 3 );    // 011
  CPPUNIT_ASSERT( ReadCode(4) == 5 );    // 0101
  CPPUNIT_ASSERT( ReadCode(5) == 7 );    // 00111
  CPPUNIT_ASSERT( ReadCode(6) == 9 );    // 001001
  CPPUNIT_ASSERT( ReadCode(7) == 2 );    // 0000010
  CPPUNIT_ASSERT( ReadCode(8) == 4 );    // 00000100
  CPPUNIT_ASSERT( ReadCode(9) == 6 );    // 000000110
  CPPUNIT_ASSERT( ReadCode(10) == 8 );   // 0000001000
  CPPUNIT_ASSERT( ReadCode(11) == 10 );  // 00000001010
  CPPUNIT_ASSERT( ReadCode(12) == 12 );  // 000000001100
  CPPUNIT_ASSERT_THROW( ReadCode(4), vp::Exception );  // 100, 3 bits left
}

// test data member m_Buffer
void GifCodeReaderTest::testBuffer()
{
  U8String Codes;

  Codes += 0xF9;  // 111,11001
  Codes += 0x99;  // 10011001111
  Codes += 0xC8;  // 1100100,0
  CPPUNIT_ASSERT( Codes.size() == 3 );

  GifCodeReader ReadCode( Codes );

  // 3 bits are left in buffer after 1st reading,
  // 2nd reading causes 2 bytes being added to buffer,
  // which results in 19 bits in buffer.
  CPPUNIT_ASSERT( ReadCode(5) == 25 );      // 11001
  CPPUNIT_ASSERT( ReadCode(12) == 1231 );   // 10011001111
  CPPUNIT_ASSERT_THROW( ReadCode(8), vp::Exception );  // 1100100, 7 bits left
}
