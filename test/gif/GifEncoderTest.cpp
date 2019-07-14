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

#include "GifEncoderTest.h"
#include "GifEncoder.h"
#include "GifCodeReader.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifEncoderTest );

// Input: empty
// Output: clear code, EOI code
void GifEncoderTest::testZeroPixel()
{
  U8String Pixels, Codes;

  // input: empty
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  GifEncoder encoder;

  //
  // bpp = 1
  encoder( 1, Pixels, Codes );

  // output: 2,3 (Clear Code and EOI, 2 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x0E );  // 11,10

  //
  // bpp = 2
  Codes.resize(0);
  encoder( 2, Pixels, Codes );

  // output: 4,5 (Clear Code and EOI, 3 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x2C );  // 101,100

  //
  // bpp = 3
  Codes.resize(0);
  encoder( 3, Pixels, Codes );

  // output: 8,9 (Clear Code and EOI, 4 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x98 );  // 1001,1000

  //
  // bpp = 4
  Codes.resize(0);
  encoder( 4, Pixels, Codes );

  // output: 16,17 (Clear Code and EOI, 5 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x30 );  // 001,10000
  CPPUNIT_ASSERT( Codes[1] == 0x02 );  // 10

  //
  // bpp = 5
  Codes.resize(0);
  encoder( 5, Pixels, Codes );

  // output: 32,33 (Clear Code and EOI, 6 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x60 );  // 01,100000
  CPPUNIT_ASSERT( Codes[1] == 0x08 );  // 1000

  //
  // bpp = 6
  Codes.resize(0);
  encoder( 6, Pixels, Codes );

  // output: 64,65 (Clear Code and EOI, 7 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0xC0 );  // 1,1000000
  CPPUNIT_ASSERT( Codes[1] == 0x20 );  // 100000

  //
  // bpp = 7
  Codes.resize(0);
  encoder( 7, Pixels, Codes );

  // output: 128,129 (Clear Code and EOI, 8 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x80 );  // 10000000
  CPPUNIT_ASSERT( Codes[1] == 0x81 );  // 10000001

  //
  // bpp = 8
  Codes.resize(0);
  encoder( 8, Pixels, Codes );

  // output: 256,257 (Clear Code and EOI, 9 bits/each)
  CPPUNIT_ASSERT( Codes.size() == 3 );
  CPPUNIT_ASSERT( Codes[0] == 0x00 );  // 00000000
  CPPUNIT_ASSERT( Codes[1] == 0x03 );  // 0000001,1
  CPPUNIT_ASSERT( Codes[2] == 0x02 );  // 10
}

// Input: 1
// Output: clear code, 1, EOI code
void GifEncoderTest::testOnePixel()
{
  U8String Pixels, Codes;

  // input: 1
  Pixels += 0x01; 
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  GifEncoder encoder;

  //
  // bpp = 1
  encoder( 1, Pixels, Codes );

  // output: 2,1,3 (10,01,11)
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT( Codes[0] == 0x36 );  // 11,01,10

  //
  // bpp = 2
  Codes.resize(0);
  encoder( 2, Pixels, Codes );

  // output: 4,1,5 (100,001,101)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x4C );  // 01,001,100
  CPPUNIT_ASSERT( Codes[1] == 0x01 );  // 1

  //
  // bpp = 3
  Codes.resize(0);
  encoder( 3, Pixels, Codes );

  // output: 8,1,9 (1000,0001,1001)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x18 );  // 0001,1000
  CPPUNIT_ASSERT( Codes[1] == 0x09 );  // 1001

  //
  // bpp = 4
  Codes.resize(0);
  encoder( 4, Pixels, Codes );

  // output: 16,1,17 (10000,00001,10001)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x30 );  // 001,10000
  CPPUNIT_ASSERT( Codes[1] == 0x44 );  // 10001,00

  //
  // bpp = 5
  Codes.resize(0);
  encoder( 5, Pixels, Codes );

  // output: 32,1,33 (100000,000001,100001)
  CPPUNIT_ASSERT( Codes.size() == 3 );
  CPPUNIT_ASSERT( Codes[0] == 0x60 );  // 01,100000
  CPPUNIT_ASSERT( Codes[1] == 0x10 );  // 0001,0000
  CPPUNIT_ASSERT( Codes[2] == 0x02 );  // 10

  //
  // bpp = 6
  Codes.resize(0);
  encoder( 6, Pixels, Codes );

  // output: 64,1,65 (1000000,0000001,1000001)
  CPPUNIT_ASSERT( Codes.size() == 3 );
  CPPUNIT_ASSERT( Codes[0] == 0xC0 );  // 1,1000000
  CPPUNIT_ASSERT( Codes[1] == 0x40 );  // 01,000000
  CPPUNIT_ASSERT( Codes[2] == 0x10 );  // 10000

  //
  // bpp = 7
  Codes.resize(0);
  encoder( 7, Pixels, Codes );

  // output: 128,1,129 (10000000,00000001,10000001)
  CPPUNIT_ASSERT( Codes.size() == 3 );
  CPPUNIT_ASSERT( Codes[0] == 0x80 );  // 10000000
  CPPUNIT_ASSERT( Codes[1] == 0x01 );  // 00000001
  CPPUNIT_ASSERT( Codes[2] == 0x81 );  // 10000001

  //
  // bpp = 8
  Codes.resize(0);
  encoder( 8, Pixels, Codes );

  // output: 256,1,257 (100000000,000000001,100000001)
  CPPUNIT_ASSERT( Codes.size() == 4 );
  CPPUNIT_ASSERT( Codes[0] == 0x00 );  // 00000000
  CPPUNIT_ASSERT( Codes[1] == 0x03 );  // 0000001,1
  CPPUNIT_ASSERT( Codes[2] == 0x04 );  // 000001,00
  CPPUNIT_ASSERT( Codes[3] == 0x04 );  // 100
}

// bpp = 1
// Verify code size correctly changed from 2-bit to 3-bit.
// Input: no repetition, forms permutations P(2,2) = 2!/(2-2)! = 2
// Output: same as input.
// 2 codes: 4,5 added to string table:
//   4 -> (0,1), 5 -> (1,0)
// Code size changes from 2- to 3-bit, when code 4 is added.
// therefor 0 in outputs is 2-bit, while 1,0 are 2-bit.
void GifEncoderTest::testBpp1()
{
  // input: 0,1,0
  U8String Pixels = {0,1,0};
  CPPUNIT_ASSERT( Pixels.size() == 3 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 1, Pixels, Codes );

  // output: 2,0,1,0,3
  //        (10,00,001,000,011)
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x12 );  // 0,001,00,10
  CPPUNIT_ASSERT( Codes[1] == 0x0C );  //      011,00
}

// bpp = 1
// Verify the 2 codes in testBpp1()
void GifEncoderTest::testBpp1Codes()
{
  // input: 0,1,0,1,1,0
  //        (inputs of testBpp1() + 1,1,0)
  U8String Pixels = {0,1,0,1,1,0};
  CPPUNIT_ASSERT( Pixels.size() == 6 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 1, Pixels, Codes );

  // output: 2,0,1,4,5,3 (10,00,001,100,101,011)
  //   0,1,0,1,1,0 -> 0,1,(0,1),(1,0) -> 0,1,4,5   
  // 1 more code added to string table:
  //   6->(0,1,1)
  // code 6 is still 3-bit
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT( Codes[0] == 0x12 );  // 0,001,00,10
  CPPUNIT_ASSERT( Codes[1] == 0x76 );  //  011,101,10
}

// bpp = 2
// Verify code size correctly changed from 3-bit to 4-bit.
// Input: no repetition, forms permutations P(3,2) = 3!/(3-2)! = 6
// Output: same as input.
// 6 codes added to string table:
//   string | 1,2 | 2,1 | 1,3 | 3,2 | 2,3 | 3,1
//  --------+-----+-----+-----+-----+-----+-----
//   code   |  6  |  7  |  8  |  9  | 10  | 11
// Code size changes from 3-bit to 4-bit, when code 8 is added,
// 1,2,1 in the output are 3-bit, while 3,2,3,1, 4-bit
void GifEncoderTest::testBpp2()
{
  // input: 1,2,1,3,2,3,1
  U8String Pixels = { 1,2,1,3,2,3,1 };
  CPPUNIT_ASSERT( Pixels.size() == 7 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 2, Pixels, Codes );

  // output: 4,1,2,1,3,2,3,1,5
  CPPUNIT_ASSERT( Codes.size() == 4 );

  GifCodeReader ReadCode(Codes);

  // 3-bit: 4,1,2,1
  CPPUNIT_ASSERT( ReadCode(3) == 4 );  // clear code
  CPPUNIT_ASSERT( ReadCode(3) == 1 );
  CPPUNIT_ASSERT( ReadCode(3) == 2 );
  CPPUNIT_ASSERT( ReadCode(3) == 1 );

  // 4-bit: 3,2,3,1,5
  CPPUNIT_ASSERT( ReadCode(4) == 3 );
  CPPUNIT_ASSERT( ReadCode(4) == 2 );
  CPPUNIT_ASSERT( ReadCode(4) == 3 );
  CPPUNIT_ASSERT( ReadCode(4) == 1 );
  CPPUNIT_ASSERT( ReadCode(4) == 5 );  // EOI code
}

// bpp = 2
// Verify the 6 codes in testBpp2()
void GifEncoderTest::testBpp2Codes()
{
  // input: 1,2,1,3,2,3,
  //        1,2,1,3,2,3,
  //          2,1,3,2,3,1
  U8String Pixels = { 1,2,1,3,2,3,1,2,1,3,2,3,2,1,3,2,3,1 };
  CPPUNIT_ASSERT( Pixels.size() == 18 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 2, Pixels, Codes );

  // onput: 4,1,2,1,3,2,3,6,8,10,7,9,11,5
  //   1,2,1,3,2,3,1,2,1,3,2,3,2,1,3,2,3,1
  //     -> 1,2,1,3,2,3,(1,2),(1,3),(2,3),(2,1),(3,2),(3,1)
  //     -> 1,2,1,3,2,3,6,8,10,7,9,11
  // 5 more codes added to string table
  //   12->(1,2,1) 13->(1,3,2) 14->(2,3,2) 15->(2,1,3) 16->(3,2,3)
  // when code 16 is added, code size changes to 5-bit, 11 in output is 5-bit
  CPPUNIT_ASSERT( Codes.size() == 7 );

  GifCodeReader ReadCode(Codes);

  // 3-bit: 4,1,2,1
  CPPUNIT_ASSERT( ReadCode(3) == 4 );  // clear code
  CPPUNIT_ASSERT( ReadCode(3) == 1 );
  CPPUNIT_ASSERT( ReadCode(3) == 2 );
  CPPUNIT_ASSERT( ReadCode(3) == 1 );

  // 4-bit: 3,2,3,6,8,10,7,9
  CPPUNIT_ASSERT( ReadCode(4) == 3 );
  CPPUNIT_ASSERT( ReadCode(4) == 2 );
  CPPUNIT_ASSERT( ReadCode(4) == 3 );
  CPPUNIT_ASSERT( ReadCode(4) == 6 );
  CPPUNIT_ASSERT( ReadCode(4) == 8 );
  CPPUNIT_ASSERT( ReadCode(4) == 10 );
  CPPUNIT_ASSERT( ReadCode(4) == 7 );
  CPPUNIT_ASSERT( ReadCode(4) == 9 );

  // 4-bit: 11,5
  CPPUNIT_ASSERT( ReadCode(5) == 11 );
  CPPUNIT_ASSERT( ReadCode(5) == 5 );  // EOI code
}

// bpp = 3
// Verify code size correctly changed from 4-bit to 5-bit.
// Input: no repetition, forms permutations P(4,2) = 4!/(4-2)! = 12
// Output: same as input.
// 12 codes added to string table:
//  string| 1,2 | 2,1 | 1,3 | 3,1 | 1,4 | 4,2 | 2,3 | 3,2 | 2,4 | 4,3 | 3,4 | 4,1
// -------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+----
//  code  | 10  | 11  | 12  | 13  | 14  | 15  | 16  | 17  | 18  | 19  | 20  | 21
// Code size changes from 4-bit to 5-bit, when code 16 is added,
// therefor 1,2,1,3,1,4,2 in the outputs are 4-bit, while 3,2,4,3,4,1, 5-bit
void GifEncoderTest::testBpp3()
{
  // input: 1,2,1,3,1,4,2,3,2,4,3,4,1
  U8String Pixels = { 1,2,1,3,1,4,2,3,2,4,3,4,1 };
  CPPUNIT_ASSERT( Pixels.size() == 13 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 3, Pixels, Codes );

  // output: 8,1,2,1,3,1,4,2,3,2,4,3,4,1,9
  CPPUNIT_ASSERT( Codes.size() == 9 );

  GifCodeReader ReadCode(Codes);

  // 4-bit: 8,1,2,1,3,1,4,2
  CPPUNIT_ASSERT( ReadCode(4) == 8 );  // clear code
  uint8_t i, j;
  for( j = 2; j <= 4; ++j )
  {
    CPPUNIT_ASSERT( ReadCode(4) == 1 );
    CPPUNIT_ASSERT( ReadCode(4) == j );
  }
  CPPUNIT_ASSERT( ReadCode(4) == 2 );

  // 5-bit: 3,2,4,3,4,1,9
  CPPUNIT_ASSERT( ReadCode(5) == 3 );
  for( i = 2; i <= 3; ++i )
  {
    CPPUNIT_ASSERT( ReadCode(5) == i );
    CPPUNIT_ASSERT( ReadCode(5) == 4 );
  }
  CPPUNIT_ASSERT( ReadCode(5) == 1 );
  CPPUNIT_ASSERT( ReadCode(5) == 9 );  // EOI code
}

// bpp = 3
// Verify the 12 codes in testBpp3()
void GifEncoderTest::testBpp3Codes()
{
  // input: 1,2,1,3,1,4,2,3,2,4,3,4,
  //        1,2,1,3,1,4,2,3,2,4,3,4,
  //          2,1,3,1,4,2,3,2,4,3,4,1
  U8String Pixels = { 1,2,1,3,1,4,2,3,2,4,3,4,1,2,1,3,1,4,2,3,2,4,3,4,2,1,3,1,4,2,3,2,4,3,4,1 };
  CPPUNIT_ASSERT( Pixels.size() == 36 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 3, Pixels, Codes );

  // output: 8,1,2,1,3,1,4,2,3,2,4,3,4,10,12,14,16,18,20,11,13,15,17,19,21,9
  // 1,2,1,3,1,4,2,3,2,4,3,4,1,2,1,3,1,4,2,3,2,4,3,4,2,1,3,1,4,2,3,2,4,3,4,1
  //   -> 1,2,1,3,1,4,2,3,2,4,3,4,
  //      (1,2),(1,3),(1,4),(2,3),(2,4),(3,4),
  //      (2,1),(3,1),(4,2),(3,2),(4,3),(4,1)
  //   -> 1,2,1,3,1,4,2,3,2,4,3,4,10,12,14,16,18,20,11,13,15,17,19,21
  // 11 more codes added to string table:
  //   22->(1,2,1) 23->(1,3,1) 24->(1,4,2) ... 31->(3,2,4) 32->(4,3,4)
  // Code size changes to 6-bit, when code 32 is added, 
  // therefor 21 in output is 6-bit.
  CPPUNIT_ASSERT( Codes.size() == 16 );

  GifCodeReader ReadCode(Codes);

  // 4-bit: 8,1,2,1,3,1,4,2
  CPPUNIT_ASSERT( ReadCode(4) == 8 );  // clear code
  uint8_t i, j;
  for( j = 2; j <= 4; ++j )
  {
    CPPUNIT_ASSERT( ReadCode(4) == 1 );
    CPPUNIT_ASSERT( ReadCode(4) == j );
  }
  CPPUNIT_ASSERT( ReadCode(4) == 2 );

  // 5-bit: 3,2,4,3,4,10,12,14,16,18,20,11,13,15,17,19
  CPPUNIT_ASSERT( ReadCode(5) == 3 );
  for( i = 2; i <= 3; ++i )
  {
    CPPUNIT_ASSERT( ReadCode(5) == i );
    CPPUNIT_ASSERT( ReadCode(5) == 4 );
  }
  for( i = 10; i <= 20; i += 2 )
    CPPUNIT_ASSERT( ReadCode(5) == i );
  for( i = 11; i <= 19; i += 2 )
    CPPUNIT_ASSERT( ReadCode(5) == i );

  // 6-bit: 21,9
  CPPUNIT_ASSERT( ReadCode(6) == 21 );
  CPPUNIT_ASSERT( ReadCode(6) == 9 );  // EOI code
}

// bpp = 4
// Verify code size correctly changed from 5- to 6-bit.
// Input: no repetition, forms permutations P(5,2) = 5!/(5-2)! = 20
// Output: same as input.
// 20 codes add to string table:
//  string| 1,2 | 2,1 | 1,3 | 3,1 | ... | 3,4 | 4,3 | 3,5 | 5,4 | 4,5 | 5,1 
// -------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
//  code  | 18  | 19  | 20  | 21  | ... | 32  | 33  | 34  | 35  | 36  | 37
// Code size changes from 5-bit to 6-bit, when code 32 is added,
// therefor 1,2,1,3,1,4,1,5,2,3,2,4,2,5,3 in the outputs are 5-bit, while
// 4,3,5,4,5,1, 6-bit
void GifEncoderTest::testBpp4()
{
  // input: 1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1
  U8String Pixels = { 1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1 };
  CPPUNIT_ASSERT( Pixels.size() == 21 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 4, Pixels, Codes );

  // output: 16,1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1,17
  CPPUNIT_ASSERT( Codes.size() == 16 );

  GifCodeReader ReadCode(Codes);

  // 5-bit: 16,1,2,1,3,1,4,1,5,2,3,2,4,2,5,3
  CPPUNIT_ASSERT( ReadCode(5) == 16 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 5; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(5) == i );
      CPPUNIT_ASSERT( ReadCode(5) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(5) == 3 );

  // 6-bit: 4,3,5,4,5,1,17
  CPPUNIT_ASSERT( ReadCode(6) == 4 );
  for( i = 3; i <= 4; ++i )
  {
    CPPUNIT_ASSERT( ReadCode(6) == i );
    CPPUNIT_ASSERT( ReadCode(6) == 5 );
  }
  CPPUNIT_ASSERT( ReadCode(6) == 1 );
  CPPUNIT_ASSERT( ReadCode(6) == 17 );  // EOI code
}

// bpp = 4
// Verify the 20 codes in testBpp4()
void GifEncoderTest::testBpp4Codes()
{
  // input: 1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,
  //        1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,
  //          2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1
  U8String strBase = { 1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5 };
  U8String Pixels;
  Pixels += strBase;
  Pixels += strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1
  Pixels += strBase;
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 60 );

  U8String Codes;
  GifEncoder encoder;
  encoder( 4, Pixels, Codes );

  // output: 16,1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,
  //         18,20,22,24,26,28,30,32,34,36,
  //         19,21,23,25,27,29,31,33,35,37,17
  // 19 more codes added to string table: 38 ~ 56
  // all are 6-bit
  CPPUNIT_ASSERT( Codes.size() == 30 );

  GifCodeReader ReadCode(Codes);

  // 5-bit: 16,1,2,1,3,1,4,1,5,2,3,2,4,2,5,3
  CPPUNIT_ASSERT( ReadCode(5) == 16 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 5; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(5) == i );
      CPPUNIT_ASSERT( ReadCode(5) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(5) == 3 );

  // 6-bit: 4,3,5,4,5,18,20,22,24,26,28,30,32,34,36,19,21,23,25,27,29,31,33,35,37,17
  CPPUNIT_ASSERT( ReadCode(6) == 4 );
  for( i = 3; i <= 4; ++i )
  {
    CPPUNIT_ASSERT( ReadCode(6) == i );
    CPPUNIT_ASSERT( ReadCode(6) == 5 );
  }
  for( i = 18; i <= 36; i += 2 )
    CPPUNIT_ASSERT( ReadCode(6) == i );
  for( i = 19; i <= 37; i += 2 )
    CPPUNIT_ASSERT( ReadCode(6) == i );
  CPPUNIT_ASSERT( ReadCode(6) == 17 );  // EOI code
}

// bpp = 5
// Verify code size correctly changed from 6-bit to 7-bit.
// Input: no repetition, forms permutations P(7,2) = 7!/(7-2)! = 42
// Output: same as input.
// 42 codes added to string table:
//    string| 1,2 | 2,1 | 1,3 | 3,1 | ... | 4,5 | 5,4 | 4,6 | ... | 6,7 | 7,1 
//   -------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
//    code  | 34  | 35  | 36  | 37  | ... | 64  | 65  | 66  | ... | 74  | 75
// Code size changes from 6-bit to 7-bit, when code 64 is added,
// therefor 1,2,1,3,1,4,1,5,...3,6,3,7,4 in the outputs are 6-bit, while
// 5,4,6,4,7,5,6,5,7,6,7,1, 7-bit
void GifEncoderTest::testBpp5()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,1
  uint8_t i, j;
  for( i = 1; i <= 6; ++i )
  {
    for( j = i + 1; j <= 7; ++j )
    {
      Pixels += i;
      Pixels += j;
    }
  }
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 43 );

  GifEncoder encoder;
  encoder( 5, Pixels, Codes );

  // output: 32,1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,1,33
  CPPUNIT_ASSERT( Codes.size() == 36 );  // (192 + 91 = 283 bits)

  GifCodeReader ReadCode(Codes);

  // 6-bit: 32,1,2,1,3,1,4,1,5,...3,6,3,7,4  (32*6 = 192 bits)
  CPPUNIT_ASSERT( ReadCode(6) == 32 );  // clear code
  for( i = 1; i <= 3; ++i )
  {
    for( j = i + 1; j <= 7; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(6) == i );
      CPPUNIT_ASSERT( ReadCode(6) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(6) == 4 );

  // 7-bit: 5,4,6,4,7,5,6,5,7,6,7,1,33 (13*7 = 91 bits)
  CPPUNIT_ASSERT( ReadCode(7) == 5 );
  for( i = 4; i <= 5; ++i )
  {
    for( j = 6; j <= 7; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(7) == i );
      CPPUNIT_ASSERT( ReadCode(7) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(7) == 6 );
  CPPUNIT_ASSERT( ReadCode(7) == 7 );
  CPPUNIT_ASSERT( ReadCode(7) == 1 );
  CPPUNIT_ASSERT( ReadCode(7) == 33 );  // EOI code
}

// bpp = 5
// Verify the 42 codes in testBpp5()
void GifEncoderTest::testBpp5Codes()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  //        1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  //          2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,1
  uint8_t i, j;
  U8String strBase;  // 1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7
  for( i = 1; i <= 6; ++i )
  {
    for( j = i + 1; j <= 7; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  Pixels += strBase;
  Pixels += strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7
  Pixels += strBase;
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 126 );

  GifEncoder encoder;
  encoder( 5, Pixels, Codes );

  // output: 32,1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  //         34,36,38,40,...72,74,
  //         35,37,39,41,...73,75,33
  // 41 more codes added to string table: 76 ~ 116
  // all are 7-bit
  CPPUNIT_ASSERT( Codes.size() == 72 );  // 192 + 378 = 570 bits

  GifCodeReader ReadCode(Codes);

  // 6-bit: 32,1,2,1,3,1,4,1,5,...3,6,3,7,4  (32*6 = 192 bits)
  CPPUNIT_ASSERT( ReadCode(6) == 32 );  // clear code
  for( i = 1; i <= 3; ++i )
  {
    for( j = i + 1; j <= 7; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(6) == i );
      CPPUNIT_ASSERT( ReadCode(6) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(6) == 4 );

  // 7-bit: 5,4,6,4,7,5,6,5,7,6,7,
  //        34,36,38,40,...72,74,
  //        35,37,39,41,...73,75,33 (54*7 = 378 bits)
  CPPUNIT_ASSERT( ReadCode(7) == 5 );
  for( i = 4; i <= 5; ++i )
  {
    for( j = 6; j <= 7; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(7) == i );
      CPPUNIT_ASSERT( ReadCode(7) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(7) == 6 );
  CPPUNIT_ASSERT( ReadCode(7) == 7 );
  for( i = 34; i <= 74; i +=2 )
    CPPUNIT_ASSERT( ReadCode(7) == i );
  for( i = 35; i <= 75; i +=2 )
    CPPUNIT_ASSERT( ReadCode(7) == i );
  CPPUNIT_ASSERT( ReadCode(7) == 33 );  // EOI code
}

// bpp = 6
// Verify code size correctly changed from 7- to 8-bit.
// Input: no repetition, forms permutations P(9,2) = 9!/(9-2)! = 72
// Output: same as input.
// 72 codes added to string table:
//  string| 1,2 | 2,1 | 1,3 | 3,1 | ... | 6,8 | 8,6 | 6,9 | ... | 8,9 | 9,1 
// -------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
//  code  | 66  | 67  | 68  | 69  | ... | 128 | 129 | 130 | ... | 136 | 137
// Code size changes from 7- to 8-bit, when code 128 is added,
// therefor 1,2,1,3,1,4,1,5,...6,7,6 in the outputs are 7-bit, while
// 8,6,9,7,8,7,9,8,9,1, 8-bit
void GifEncoderTest::testBpp6()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,1
  uint8_t i, j;
  for( i = 1; i <= 8; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      Pixels += i;
      Pixels += j;
    }
  }
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 73 );

  GifEncoder encoder;
  encoder( 6, Pixels, Codes );

  // output: 64,1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,1,65
  CPPUNIT_ASSERT( Codes.size() == 67 );  // 448 + 88 = 536 bits

  GifCodeReader ReadCode(Codes);

  // 7-bit: 64,1,2,1,3,1,4,1,5,...6,7,6 (64*7 = 448 bits)
  CPPUNIT_ASSERT( ReadCode(7) == 64 );  // clear code
  for( i = 1; i <= 5; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(7) == i );
      CPPUNIT_ASSERT( ReadCode(7) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(7) == 6 );
  CPPUNIT_ASSERT( ReadCode(7) == 7 );
  CPPUNIT_ASSERT( ReadCode(7) == 6 );

  // 8-bit: 8,6,9,7,8,7,9,8,9,1,65 (11*8 = 88)
  CPPUNIT_ASSERT( ReadCode(8) == 8 );
  CPPUNIT_ASSERT( ReadCode(8) == 6 );
  CPPUNIT_ASSERT( ReadCode(8) == 9 );
  for( i = 7; i <= 8; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(8) == i );
      CPPUNIT_ASSERT( ReadCode(8) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(8) == 1 );
  CPPUNIT_ASSERT( ReadCode(8) == 65 );  // EOI code
}

// bpp = 6
// Verify the 72 codes in testBpp6()
void GifEncoderTest::testBpp6Codes()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,
  //        1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,
  //          2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,1
  uint8_t i, j;
  U8String strBase;  // 1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9
  for( i = 1; i <= 8; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  Pixels += strBase;
  Pixels += strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9
  Pixels += strBase;
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 216 );

  GifEncoder encoder;
  encoder( 6, Pixels, Codes );

  // output: 64,1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,
  //         66,68,70,...134,136,
  //         67,69,71,...135,137,65
  // 71 more codes added to string table: 138 ~ 208
  // all are 8-bit
  CPPUNIT_ASSERT( Codes.size() == 138 );  // 448 + 656 = 1104 bits

  GifCodeReader ReadCode(Codes);

  // 7-bit: 64,1,2,1,3,1,4,1,5,...6,7,6 (64*7 = 448 bits)
  CPPUNIT_ASSERT( ReadCode(7) == 64 );  // clear code
  for( i = 1; i <= 5; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(7) == i );
      CPPUNIT_ASSERT( ReadCode(7) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(7) == 6 );
  CPPUNIT_ASSERT( ReadCode(7) == 7 );
  CPPUNIT_ASSERT( ReadCode(7) == 6 );

  // 8-bit: 8,6,9,7,8,7,9,8,9,
  //        66,68,70,...134,136,
  //        67,69,71,...135,137,65 (82*8 = 656 bits)
  CPPUNIT_ASSERT( ReadCode(8) == 8 );
  CPPUNIT_ASSERT( ReadCode(8) == 6 );
  CPPUNIT_ASSERT( ReadCode(8) == 9 );
  for( i = 7; i <= 8; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(8) == i );
      CPPUNIT_ASSERT( ReadCode(8) == j );
    }
  }
  for( i = 66; i <= 136; i += 2 )
    CPPUNIT_ASSERT( ReadCode(8) == i );
  for( i = 67; i <= 137; i += 2 )
    CPPUNIT_ASSERT( ReadCode(8) == i );
  CPPUNIT_ASSERT( ReadCode(8) == 65 );  // EOI code
}

// bpp = 7
// Verify code size correctly changed from 8-bit to 9-bit.
// Input: no repetition, forms permutations P(12,2) = 12!/(12-2)! = 132
// Output: same as input.
// 132 codes added to string table:
//  string| 1,2 | 2,1 | 1,3 | 3,1 | ... |10,11|11,10|10,12|12,11|11,12| 12,1 
// -------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
//  code  | 130 | 131 | 132 | 133 | ... | 256 | 257 | 258 | 259 | 260 | 261
// Code size change from 8 bits to 9 bits, when code 256 is added.
// therefor 1,2,1,3,1,4,1,5,...9,10,9,11,9,12,10 in the outputs are 8-bit, while
// 11,10,12,11,12,1, 9-bit
void GifEncoderTest::testBpp7()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,...10,11,10,12,11,12,1
  uint8_t i, j;
  for( i = 1; i <= 11; ++i )
  {
    for( j = i + 1; j <= 12; ++j )
    {
      Pixels += i;
      Pixels += j;
    }
  }
  Pixels += 1;

  CPPUNIT_ASSERT( Pixels.size() == 133 );

  GifEncoder encoder;
  encoder( 7, Pixels, Codes );

  // output: 128,1,2,1,3,1,4,1,5,...10,11,10,12,11,12,1,129
  CPPUNIT_ASSERT( Codes.size() == 136 );  // 1024 + 63 = 1087 bits

  GifCodeReader ReadCode(Codes);

  // 8-bit: 128,1,2,1,3,1,4,1,5,...9,10,9,11,9,12,10 (128*8 = 1024 bits)
  CPPUNIT_ASSERT( ReadCode(8) == 128 );  // clear code
  for( i = 1; i <= 9; ++i )
  {
    for( j = i + 1; j <= 12; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(8) == i );
      CPPUNIT_ASSERT( ReadCode(8) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(8) == 10 );

  // 9-bit: 11,10,12,11,12,1,129 (7*9 = 63 bits)
  CPPUNIT_ASSERT( ReadCode(9) == 11 );
  for( i = 10; i <= 11; ++i )
  {
    CPPUNIT_ASSERT( ReadCode(9) == i );
    CPPUNIT_ASSERT( ReadCode(9) == 12 );
  }
  CPPUNIT_ASSERT( ReadCode(9) == 1 );
  CPPUNIT_ASSERT( ReadCode(9) == 129 );  // EOI code
}

// bpp = 7
// Verify the 132 codes used in testBpp6()
void GifEncoderTest::testBpp7Codes()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,...10,11,10,12,11,12,
  //        1,2,1,3,1,4,1,5,...10,11,10,12,11,12,
  //          2,1,3,1,4,1,5,...10,11,10,12,11,12,1
  uint8_t i, j;
  U8String strBase;  // 1,2,1,3,1,4,1,5,...10,11,10,12,11,12
  for( i = 1; i <= 11; ++i )
  {
    for( j = i + 1; j <= 12; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  Pixels += strBase;
  Pixels += strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,...10,11,10,12,11,12
  Pixels += strBase;
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 396 );

  GifEncoder encoder;
  encoder( 7, Pixels, Codes );

  // output: 128,1,2,1,3,1,4,1,5,...10,11,10,12,11,12,
  //         130,132,134,...258,260
  //         131,133,135,...259,261,129
  // 131 more codes added to string table: 262 ~ 392
  // all are 9-bit
  CPPUNIT_ASSERT( Codes.size() == 284 );  // 1024 + 1242 = 2266 bits

  GifCodeReader ReadCode(Codes);

  // 8-bit: 128,1,2,1,3,1,4,1,5,...9,10,9,11,9,12,10 (128*8 = 1024 bits)
  CPPUNIT_ASSERT( ReadCode(8) == 128 );  // clear code
  for( i = 1; i <= 9; ++i )
  {
    for( j = i + 1; j <= 12; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(8) == i );
      CPPUNIT_ASSERT( ReadCode(8) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(8) == 10 );

  // 9-bit: 11,10,12,11,12,
  //        130,132,134,...258,260,
  //        131,133,135,...259,261,129 (138*9 = 1242 bits)
  CPPUNIT_ASSERT( ReadCode(9) == 11 );
  for( i = 10; i <= 11; ++i )
  {
    CPPUNIT_ASSERT( ReadCode(9) == i );
    CPPUNIT_ASSERT( ReadCode(9) == 12 );
  }
  for( uint16_t k = 130; k <= 260; k += 2 )
    CPPUNIT_ASSERT( ReadCode(9) == k );
  for( uint16_t k = 131; k <= 261; k += 2 )
    CPPUNIT_ASSERT( ReadCode(9) == k );
  CPPUNIT_ASSERT( ReadCode(9) == 129 );  // EOI code
}

// bpp = 8
// Verify code size correctly changed from 9- to 10-bit.
// Input: no repetition, forms permutations P(17,2) = 17!/(17-2)! = 272
// Output: same as input.
// 272 codes added to string table:
//   string| 1,2 | 2,1 | 1,3 | 3,1 | ... |13,15|15,13|13,16| ... |16,17| 17,1 
//  -------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----
//   code  | 258 | 259 | 260 | 262 | ... | 512 | 513 | 514 | ... | 528 | 529
// Code size changes from 9 bits to 10 bits, when code 512 is added.
// therefor 1,2,1,3,1,4,1,5,...12,16,12,17,13,14,13 in the outputs are 8-bit, while
// 15,13,16,13,17,14,15,14,16,14,17,15,16,15,17,16,17,1 are 9-bit
void GifEncoderTest::testBpp8()
{
  U8String Pixels, Codes;

  // input: 1,2,1,3,1,4,1,5,...15,16,15,17,16,17,1
  uint8_t i, j;
  for( i = 1; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      Pixels += i;
      Pixels += j;
    }
  }
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 273 );

  GifEncoder encoder;
  encoder( 8, Pixels, Codes );

  // outputs: 256,1,2,1,3,1,4,1,5,...15,16,15,17,16,17,1,257
  CPPUNIT_ASSERT( Codes.size() == 312 );  // 2304 + 190 = 2494 bits
  
  GifCodeReader ReadCode(Codes);

  // 9-bit: 256,1,2,1,3,1,4,1,5,...12,16,12,17,13,14,13 (256*9 = 2304 bits)
  CPPUNIT_ASSERT( ReadCode(9) == 256 );  // clear code
  for( i = 1; i <= 12; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(9) == i );
      CPPUNIT_ASSERT( ReadCode(9) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(9) == 13 );
  CPPUNIT_ASSERT( ReadCode(9) == 14 );
  CPPUNIT_ASSERT( ReadCode(9) == 13 );

  // 10-bit:  15,13,16,13,17,14,15,14,16,14,17,15,16,15,17,16,17,1,257
  //  (19*10 = 190 bits)
  CPPUNIT_ASSERT( ReadCode(10) == 15 );
  for( j = 16; j <= 17; ++j )
  {
    CPPUNIT_ASSERT( ReadCode(10) == 13 );
    CPPUNIT_ASSERT( ReadCode(10) == j );
  }
  for( i = 14; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(10) == i );
      CPPUNIT_ASSERT( ReadCode(10) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(10) == 1 );
  CPPUNIT_ASSERT( ReadCode(10) == 257 );
}

// bpp = 8
// Verify the 272 codes used in testBpp8()
void GifEncoderTest::testBpp8Codes()
{
  U8String Pixels, Codes;

  // inputs: 1,2,1,3,1,4,1,5,...15,16,15,17,16,17,
  //         1,2,1,3,1,4,1,5,...15,16,15,17,16,17,
  //           2,1,3,1,4,1,5,...15,16,15,17,16,17,1
  uint8_t i, j;
  U8String strBase;   // 1,2,1,3,1,4,1,5,...15,16,15,17,16,17
  for( i = 1; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  Pixels += strBase;
  Pixels += strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,...15,16,15,17,16,17
  Pixels += strBase;
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 816 );

  GifEncoder encoder;
  encoder( 8, Pixels, Codes );

  // outputs: 256,1,2,1,3,1,4,1,5,...15,16,15,17,16,17,
  //          258,260,262,...526,528,
  //          259,261,263,...527,529,257
  // 271 more codes added to string table: 530 ~ 800
  // all are 10-bit
  CPPUNIT_ASSERT( Codes.size() == 651 );  // 2304 + 2900 = 5204 bits
  
  GifCodeReader ReadCode(Codes);

  // 9-bit: 256,1,2,1,3,1,4,1,5,...13,14,13 (256*9 = 2304 bits)
  CPPUNIT_ASSERT( ReadCode(9) == 256 );  // clear code
  for( i = 1; i <= 12; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(9) == i );
      CPPUNIT_ASSERT( ReadCode(9) == j );
    }
  }
  CPPUNIT_ASSERT( ReadCode(9) == 13 );
  CPPUNIT_ASSERT( ReadCode(9) == 14 );
  CPPUNIT_ASSERT( ReadCode(9) == 13 );

  // 10-bit:  15,13,16,13,17,14,15,14,16,14,17,15,16,15,17,16,17,
  //          258,260,262,...526,528
  //          259,261,263,...527,529,257
  //  (290*10 = 2900 bits)
  CPPUNIT_ASSERT( ReadCode(10) == 15 );
  for( j = 16; j <= 17; ++j )
  {
    CPPUNIT_ASSERT( ReadCode(10) == 13 );
    CPPUNIT_ASSERT( ReadCode(10) == j );
  }
  for( i = 14; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(10) == i );
      CPPUNIT_ASSERT( ReadCode(10) == j );
    }
  }
  for( uint16_t k = 258; k <= 528; k += 2 )
    CPPUNIT_ASSERT( ReadCode(10) == k );
  for( uint16_t k = 259; k <= 529; k += 2 )
    CPPUNIT_ASSERT( ReadCode(10) == k );
  CPPUNIT_ASSERT( ReadCode(10) == 257 );  // EOI code
}

// bpp = 8
// Reach maximum code.
// All 3838 codes: 258~4095
//   9-bit:  258~511   (254  codes)
//   10-bit: 512~1023  (512  codes)
//   11-bit: 1024~2047 (1024 codes)
//   12-bit: 2048~4095 (2048 codes)
// String that forms P(63,2) = 63!/(63-2)! = 3906 permutations contains 3907 numbers. 
// Use the first 3839 of the 3907 numbers to form the 3838 codes.
void GifEncoderTest::testBpp8MaxCode()
{
  U8String Pixels, Codes;

  // inputs: (total 3839 numbers)
  //   1,2,1,3,1,4,1,5 ... 1,63 (2*62 = 124 numbers)
  //   2,3,2,4,2,5 ... 2,63     (2*61 = 122 numbers)
  //   3,4,3,5 ... 3,63         (2*60 = 120 numbers)
  //   ......
  //   54,55,54,56,...54,63     (2*9 = 18 numbers)
  //   55,56,55,57,55           (5 numbers)

  uint8_t i, j;
  for( i = 1; i <= 54; ++i )  // 3834 numbers
  {
    for( j = i + 1; j <= 63; ++j )
    {
      Pixels += i;
      Pixels += j;
    }
  }
  for( j = 56; j <= 57; ++j )  // 5 numbers
  {
    Pixels += 55;
    Pixels += j;
  }
  Pixels += 55;
  CPPUNIT_ASSERT( Pixels.size() == 3839 );

  GifEncoder encoder;
  encoder( 8, Pixels, Codes );
  
  // output: same as input,
  //         prefixed with clear code and suffixed with EOI code
  //    9-bit:   1st  ~ 255th number  (255 codes)
  //   10-bit: 256th  ~ 767th number  (512 codes)
  //   11-bit: 768th  ~ 1791st number (1024 codes)
  //   12-bit: 1792nd ~ 3839th number (2048 codes)

  // 256*9 + 512*10 + 1024*11 + 2049*12 = 43276 bits = 5410 bytes
  CPPUNIT_ASSERT( Codes.size() == 5410 );

  GifCodeReader ReadCode(Codes);

  // 9-bit: 256,1,2,1,3,1,4,1,5,...3,4,3,5,3,6,3,7,3 (256 codes)
  uint16_t Counter = 0;
  CPPUNIT_ASSERT( ReadCode(9) == 256 );  // clear code
  Counter++;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(9) == i );
      CPPUNIT_ASSERT( ReadCode(9) == j );
      Counter += 2;
    }
  }
  for( j = 4; j <= 7; ++j )
  {
    CPPUNIT_ASSERT( ReadCode(9) == 3 );
    CPPUNIT_ASSERT( ReadCode(9) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(9) == 3 );
  Counter++;
  CPPUNIT_ASSERT( Counter == 256 );  // clear code + 255 codes

  // 10-bit:  8,3,9,3,10,3,11,...7,8,7,9,...7,33,7 (512 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(10) == 8 );
  Counter++;
  for( j = 9; j <= 63; ++j )  // 3,9,3,10,...3,63 (110 codes)
  {
    CPPUNIT_ASSERT( ReadCode(10) == 3 );
    CPPUNIT_ASSERT( ReadCode(10) == j );
    Counter += 2;
  }
  for( i = 4; i <= 6; ++i )  // 4,5,4,6,4,7,...6,62,6,63 (348 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(10) == i );
      CPPUNIT_ASSERT( ReadCode(10) == j );
      Counter += 2;
    }
  }
  for( j = 8; j <= 33; ++j )  // 7,8,7,9,...7,33 (52 codes)
  {
    CPPUNIT_ASSERT( ReadCode(10) == 7 );
    CPPUNIT_ASSERT( ReadCode(10) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(10) == 7 );
  Counter++;
  CPPUNIT_ASSERT( Counter == 512 );  // 512 codes

  // 11-bit: 34,7,35,7,36,...17,39,17,40,17 (1024 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(11) == 34 );
  Counter++;
  for( j = 35 ; j <= 63; ++j )  // 7,35,7,36,...7,63 (58 codes)
  {
    CPPUNIT_ASSERT( ReadCode(11) == 7 );
    CPPUNIT_ASSERT( ReadCode(11) == j );
    Counter += 2;
  }
  for( i = 8; i <= 16; ++i )  // 8,9,8,10,...16,62,16,63 (918 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(11) == i );
      CPPUNIT_ASSERT( ReadCode(11) == j );
      Counter += 2;
    }
  }
  for( j = 18; j <= 40; ++j )  // 17,18,17,19,...17,40 (46 codes)
  {
    CPPUNIT_ASSERT( ReadCode(11) == 17 );
    CPPUNIT_ASSERT( ReadCode(11) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(11) == 17 );
  Counter++;
  CPPUNIT_ASSERT( Counter == 1024 );  // 1024 codes

  // 12-bit: 41,17,42,17,43,17,44,...55,56,55,57,55,257 (2049 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(12) == 41 );
  Counter++;
  for( j = 42; j <= 63; ++j )  // 17,42,17,43,...17,63 (44 codes)
  {
    CPPUNIT_ASSERT( ReadCode(12) == 17 );
    CPPUNIT_ASSERT( ReadCode(12) == j );
    Counter += 2;
  }
  for( i = 18; i <= 54; ++i )  // 18,19,18,20,...54,62,54,63 (1998 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(12) == i );
      CPPUNIT_ASSERT( ReadCode(12) == j );
      Counter += 2;
    }
  }
  for( j = 56; j <= 57; ++j ) // 55,56,55,57 (4 codes)
  {
    CPPUNIT_ASSERT( ReadCode(12) == 55 );
    CPPUNIT_ASSERT( ReadCode(12) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(12) == 55 );
  CPPUNIT_ASSERT( ReadCode(12) == 257 );  // EOI code
  Counter += 2;
  CPPUNIT_ASSERT( Counter == 2049 );  // 2048 codes + EOI code
}


// bpp = 8
// Verify that encoder will write out clear code when maximum code is reached.
// Input string that forms P(63,2) = 63!/(63-2)! = 3906 permutations contains 3907
// numbers. First 3839 of the 3907 numbers fill out all 3838 codes from 258 to 4905.
//   All 3838 codes: 258~4095
//     9-bit:  258~511   (254  codes)
//     10-bit: 512~1023  (512  codes)
//     11-bit: 1024~2047 (1024 codes)
//     12-bit: 2048~4095 (2048 codes)
// The remaining 86 numbers will cause string table to start over again, clear code
// of size 9-bit is written out.
// Output contains all 3907 numbers + 2 clear codes + 1 EOI codes, totally 3910 codes.
void GifEncoderTest::testBpp8ExceedMaxCode()
{
  U8String Pixels, Codes;

  // inputs: 1,2,1,3,1,4,1,5,...61,62,61,63,62,63,1 (total 3907 numbers)
  //   first 3839 numbers
  //     1,2,1,3,1,4,1,5 ... 1,63 (2*62 = 124 numbers)
  //     2,3,2,4,2,5 ... 2,63     (2*61 = 122 numbers)
  //     3,4,3,5 ... 3,63         (2*60 = 120 numbers)
  //     ......
  //     54,55,54,56,...54,63     (2*9 = 18 numbers)
  //     55,56,55,57,55           (5 numbers)
  //   remaining 86 numbers:
  //     58,55,59,55,60,55,61,55,62,55,63 
  //     56,57,56,58,...56,63
  //     ......
  //     61,62,61,63              (2*2 = 4)
  //     62,63                    (2*1 = 2)
  //     1                        (1*1 = 1)
  uint8_t i, j;
  for( i = 1; i <= 62; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      Pixels += i;
      Pixels += j;
    }
  }
  Pixels += 1;
  CPPUNIT_ASSERT( Pixels.size() == 3907 );

  GifEncoder encoder;
  encoder( 8, Pixels, Codes );

  // outputs: 256,1,2,1,3,1,4,1,5,...55,56,55,57,55,
  //          256,58,55,59,55,60,...61,62,61,63,62,63,1,257
  //          (3910 codes = 3907 + 2 clear codes + EOI code)

  // 256*9 + 512*10 + 1024*11 + 2049*12 + 69*9 = 43897 bits = 5488 bytes
  CPPUNIT_ASSERT( Codes.size() == 5488 );

  GifCodeReader ReadCode(Codes);

  // 9-bit: 256,1,2,1,3,1,4,1,5,...3,4,3,5,3,6,3,7,3 (256 codes)
  uint16_t Counter = 0;
  CPPUNIT_ASSERT( ReadCode(9) == 256 );  // clear code
  Counter++;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(9) == i );
      CPPUNIT_ASSERT( ReadCode(9) == j );
      Counter += 2;
    }
  }
  for( j = 4; j <= 7; ++j )
  {
    CPPUNIT_ASSERT( ReadCode(9) == 3 );
    CPPUNIT_ASSERT( ReadCode(9) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(9) == 3 );
  Counter++;
  CPPUNIT_ASSERT( Counter == 256 );  // 256 codes

  // 10-bit:  8,3,9,3,10,3,11,...7,8,7,9,...7,33,7 (512 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(10) == 8 );
  Counter++;
  for( j = 9; j <= 63; ++j )  // 3,9,3,10,...3,63 (110 codes)
  {
    CPPUNIT_ASSERT( ReadCode(10) == 3 );
    CPPUNIT_ASSERT( ReadCode(10) == j );
    Counter += 2;
  }
  for( i = 4; i <= 6; ++i )  // 4,5,4,6,4,7,...6,62,6,63 (348 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(10) == i );
      CPPUNIT_ASSERT( ReadCode(10) == j );
      Counter += 2;
    }
  }
  for( j = 8; j <= 33; ++j )  // 7,8,7,9,...7,33 (52 codes)
  {
    CPPUNIT_ASSERT( ReadCode(10) == 7 );
    CPPUNIT_ASSERT( ReadCode(10) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(10) == 7 );
  Counter++;
  CPPUNIT_ASSERT( Counter == 512 );  // 512 codes

  // 11-bit: 34,7,35,7,36,...17,39,17,40,17 (1024 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(11) == 34 );
  Counter++;
  for( j = 35 ; j <= 63; ++j )  // 7,35,7,36,...7,63 (58 codes)
  {
    CPPUNIT_ASSERT( ReadCode(11) == 7 );
    CPPUNIT_ASSERT( ReadCode(11) == j );
    Counter += 2;
  }
  for( i = 8; i <= 16; ++i )  // 8,9,8,10,...16,62,16,63 (918 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(11) == i );
      CPPUNIT_ASSERT( ReadCode(11) == j );
      Counter += 2;
    }
  }
  for( j = 18; j <= 40; ++j )  // 17,18,17,19,...17,40 (46 codes)
  {
    CPPUNIT_ASSERT( ReadCode(11) == 17 );
    CPPUNIT_ASSERT( ReadCode(11) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(11) == 17 );
  Counter++;
  CPPUNIT_ASSERT( Counter == 1024 );  // 1024 codes

  // 12-bit: 41,17,42,17,43,17,44,...55,56,55,57,55 (2049 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(12) == 41 );
  Counter++;
  for( j = 42; j <= 63; ++j )  // 17,42,17,43,...17,63 (44 codes)
  {
    CPPUNIT_ASSERT( ReadCode(12) == 17 );
    CPPUNIT_ASSERT( ReadCode(12) == j );
    Counter += 2;
  }
  for( i = 18; i <= 54; ++i )  // 18,19,18,20,...54,62,54,63 (1998 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(12) == i );
      CPPUNIT_ASSERT( ReadCode(12) == j );
      Counter += 2;
    }
  }
  for( j = 56; j <= 57; ++j ) // 55,56,55,57 (4 codes)
  {
    CPPUNIT_ASSERT( ReadCode(12) == 55 );
    CPPUNIT_ASSERT( ReadCode(12) == j );
    Counter += 2;
  }
  CPPUNIT_ASSERT( ReadCode(12) == 55 );
  CPPUNIT_ASSERT( ReadCode(12) == 256 );  // clear code
  Counter += 2;
  CPPUNIT_ASSERT( Counter == 2049 );  // 2049 codes

  // 9-bit again: 58,55,59,55,60,55,61,...62,63,1,257 (69 codes)
  Counter = 0;
  CPPUNIT_ASSERT( ReadCode(9) == 58 );
  Counter++;
  for( j = 59; j <= 63; ++j )  // 55,59,55,60,...55,63 (10 codes)
  {
    CPPUNIT_ASSERT( ReadCode(9) == 55 );
    CPPUNIT_ASSERT( ReadCode(9) == j );
    Counter += 2;
  }
  for( i = 56; i <= 62; ++i )  // 56,57,56,58,...62,63 (56 codes)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( ReadCode(9) == i );
      CPPUNIT_ASSERT( ReadCode(9) == j );
      Counter += 2;
    }
  }
  CPPUNIT_ASSERT( ReadCode(9) == 1 ); 
  CPPUNIT_ASSERT( ReadCode(9) == 257 );  // EOI code
  Counter += 2;
  CPPUNIT_ASSERT( Counter == 69 );  // 68 codes + EOI code
}

// bpp = 8
// Directly verify all 3838 codes
void GifEncoderTest::testBpp8AllCodes()
{
  U8String Pixels, Codes;

  // input: 1,1,1,1,... (7370880 1's)
  //  1,(1,1),(1,1,1),(1,1,1,1),(1,1,1,1,1),...(3838 1's),(3839 1's)
  //    -> 1,258,259,260,261,...4094,4095
  //  32640 1's  (1+2+...+255=32640)          -->  9-bit codes: 1,258,259,...511
  //  261888 1's (256+257+...+767=261888)     --> 10-bit codes: 512,513,...,1023
  //  1310208 1's(768+769+...+1791=1310208)   --> 11-bit codes: 1024,1025,...2047
  //  5766144 1's(1792+1793+...+3839=5766144) --> 12-bit codes: 2048,2049,...4095
  uint8_t uChar = 1;
  for( int i = 0; i < 7370880; ++i )
    Pixels += uChar;

  GifEncoder encoder;
  encoder( 8, Pixels, Codes );
  
  // output: 256,1,258,259,...,4094,4095,257
  //   clear code + 1 + 3838 codes + EOI codes = 3841 codes
  //   256*9 + 512*10 + 1024*11 + 2049*12 = 43276 bits
  //   43276/8 = 5409 bytes + 4 bit = 5410 bytes
  CPPUNIT_ASSERT( Codes.size() == 5410 );

  GifCodeReader ReadCode(Codes);

  // 9-bit: 256,1,258,259,...511 (256 codes)
  CPPUNIT_ASSERT( ReadCode(9) == 256 ); // clear code
  CPPUNIT_ASSERT( ReadCode(9) == 1 );
  uint16_t i;
  for( i = 258; i <= 511; ++i )
    CPPUNIT_ASSERT( ReadCode(9) == i );

  // 10-bit: 512,513,...,1023 (512 codes)
  for( i = 512; i <= 1023; ++i )
    CPPUNIT_ASSERT( ReadCode(10) == i );

  // 11-bit: 1024,1025,...2047 (1024 codes)
  for( i = 1024; i <= 2047; ++i )
    CPPUNIT_ASSERT( ReadCode(11) == i );

  // 12-bit: 2048,2049,...4095 (2049 codes)
  for( i = 2048; i <= 4095; ++i )
    CPPUNIT_ASSERT( ReadCode(12) == i );
  CPPUNIT_ASSERT( ReadCode(12) == 257 ); // EOI code
}
