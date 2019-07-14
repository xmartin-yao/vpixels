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

#include "GifCodeWriterTest.h"
#include "GifCodeWriter.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifCodeWriterTest );

void GifCodeWriterTest::test1Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(1, 1);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x01 );
}

void GifCodeWriterTest::test2Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(3, 2);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x03 );
}

void GifCodeWriterTest::test3Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(7, 3);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x07 );
}

void GifCodeWriterTest::test4Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(15, 4);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x0F );
}

void GifCodeWriterTest::test5Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(31, 5);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x1F );
}

void GifCodeWriterTest::test6Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(63, 6);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x3F );
}

void GifCodeWriterTest::test7Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(127, 7);

  CPPUNIT_ASSERT( Codes.size() == 0 );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x7F );
}

void GifCodeWriterTest::test8Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(255, 8);

  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );

  // End() does not write out anything
  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
}

void GifCodeWriterTest::test9Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(511, 9);

  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
  CPPUNIT_ASSERT( Codes[1] == 0x01 );
}

void GifCodeWriterTest::test10Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(1023, 10);

  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
  CPPUNIT_ASSERT( Codes[1] == 0x03 );
}

void GifCodeWriterTest::test11Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(2047, 11);

  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
  CPPUNIT_ASSERT( Codes[1] == 0x07 );
}

void GifCodeWriterTest::test12Bit()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );
  writeCode(4095, 12);

  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
  CPPUNIT_ASSERT( Codes[1] == 0x0F );
}

void GifCodeWriterTest::testMixed()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );

  // codes: 1,3,5,7,9,2,4,6,8,10,12
  writeCode(1, 2);   // 01
  writeCode(3, 3);   // 011
  writeCode(5, 4);   // 0101
  writeCode(7, 5);   // 00111
  writeCode(9, 6);   // 001001
  writeCode(2, 7);   // 0000010
  writeCode(4, 8);   // 00000100
  writeCode(6, 9);   // 000000110
  writeCode(8, 10);  // 0000001000
  writeCode(10, 11); // 00000001010
  writeCode(12, 12); // 000000001100

  CPPUNIT_ASSERT( Codes.size() == 9 );
  CPPUNIT_ASSERT( Codes[0] == 0xAD );  // 101,011,01
  CPPUNIT_ASSERT( Codes[1] == 0x4E );  // 01,00111,0
  CPPUNIT_ASSERT( Codes[2] == 0x22 );  // 0010,0010
  CPPUNIT_ASSERT( Codes[3] == 0x20 );  // 00100,000
  CPPUNIT_ASSERT( Codes[4] == 0x30 );  // 00110,000
  CPPUNIT_ASSERT( Codes[5] == 0x80 );  // 1000,0000
  CPPUNIT_ASSERT( Codes[6] == 0x80 );  // 10,000000
  CPPUNIT_ASSERT( Codes[7] == 0x02 );  // 00000010
  CPPUNIT_ASSERT( Codes[8] == 0x18 );  // 0001100,0

  writeCode.End();
  
  CPPUNIT_ASSERT( Codes.size() == 10 );
  CPPUNIT_ASSERT( Codes[0] == 0xAD );  // 101,011,01
  CPPUNIT_ASSERT( Codes[1] == 0x4E );  // 01,00111,0
  CPPUNIT_ASSERT( Codes[2] == 0x22 );  // 0010,0010
  CPPUNIT_ASSERT( Codes[3] == 0x20 );  // 00100,000
  CPPUNIT_ASSERT( Codes[4] == 0x30 );  // 00110,000
  CPPUNIT_ASSERT( Codes[5] == 0x80 );  // 1000,0000
  CPPUNIT_ASSERT( Codes[6] == 0x80 );  // 10,000000
  CPPUNIT_ASSERT( Codes[7] == 0x02 );  // 00000010
  CPPUNIT_ASSERT( Codes[8] == 0x18 );  // 0001100,0
  CPPUNIT_ASSERT( Codes[9] == 0x00 );  // 000,00000
}

// test data member m_Buffer
void GifCodeWriterTest::testBuffer()
{
  U8String Codes;
  GifCodeWriter writeCode( Codes );

  // extreme case
  // 12-bit code is added, while 7 bits of m_Buffer is occupied
  writeCode(127, 7);
  writeCode(4095, 12);

  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
  CPPUNIT_ASSERT( Codes[1] == 0xFF );

  writeCode.End();
  CPPUNIT_ASSERT( Codes.size() == 3 );
  CPPUNIT_ASSERT( Codes[0] == 0xFF );
  CPPUNIT_ASSERT( Codes[1] == 0xFF );
  CPPUNIT_ASSERT( Codes[2] == 0x07 );
}
