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

#include "GifDecoderTest.h"
#include "GifDecoder.h"
#include "GifCodeWriter.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifDecoderTest );

// Input: clear code, EOI code
// Output: empty
void GifDecoderTest::testZeroPixel()
{
  U8String Codes, Pixels;
  GifDecoder decoder;

  //
  // bpp = 1
  // input: 2,3 (10,11)
  Codes += 0x0E;  // 0000,11,10
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 1, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 2
  // input: 4,5 (100,101)
  Codes.clear();
  Codes += 0x2C;  // 00,101,100
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 2, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 3
  // input: 8,9 (1000,1001)
  Codes.clear();
  Codes += 0x98;  // 1000,1001
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 3, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 4
  // input: 16,17 (10000,10001)
  Codes.clear();
  Codes += 0x30;  // 001,10000
  Codes += 0x02;  // 000000,10
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 4, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 5
  // input: 32,33 (100000,100001)
  Codes.clear();
  Codes += 0x60;  // 01,100000
  Codes += 0x08;  // 0000,1000
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 5, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 6
  // input: 64,65 (1000000,1000001)
  Codes.clear();
  Codes += 0xC0;  // 1,1000000
  Codes += 0x20;  // 00,100000
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 6, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 7
  // input: 128,129 (10000000,10000001)
  Codes.clear();
  Codes += 0x80;  // 10000000
  Codes += 0x81;  // 10000001
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 7, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );

  //
  // bpp = 8
  // input: 256,257 (100000000,100000001)
  Codes.clear();
  Codes += static_cast<uint8_t>(0);  // 00000000
  Codes += 0x03;                     // 0000001,1
  Codes += 0x02;                     // 000000,10
  CPPUNIT_ASSERT( Codes.size() == 3 );
  decoder( 8, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 0 );
}

// Input: clear code, 1, EOI code
// Output: 1
void GifDecoderTest::testOnePixel()
{
  U8String Codes, Pixels;
  GifDecoder decoder;

  //
  // bpp = 1
  // input: 2,1,3 (10,01,11)
  Codes += 0x36;  // 00,11,01,10
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 1, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );

  //
  // bpp = 2
  // input: 4,1,5 (100,001,101)
  Codes.clear();
  Pixels.clear();
  Codes += 0x4C;  // 01,001,100
  Codes += 0x01;  // 0000000,1
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 2, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );

  //
  // bpp = 3
  // input: 8,1,9 (1000,0001,1001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x18;  // 0001,1000
  Codes += 0x09;  // 0000,1001
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 3, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );

  //
  // bpp = 4
  // input: 16,1,17 (10000,00001,10001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x30;  // 001,10000
  Codes += 0x44;  // 0,10001,00
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 4, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );

  //
  // bpp = 5
  // input: 32,1,33 (100000,000001,100001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x60;  // 01,100000
  Codes += 0x10;  // 0001,0000
  Codes += 0x02;  // 000000,10
  CPPUNIT_ASSERT( Codes.size() == 3 );
  decoder( 5, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  //
  // bpp = 6
  // input: 64,1,65 (1000000,0000001,1000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0xC0;  // 1,1000000
  Codes += 0x40;  // 01,000000
  Codes += 0x10;  // 000,10000
  CPPUNIT_ASSERT( Codes.size() == 3 );
  decoder( 6, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );

  //
  // bpp = 7
  // input: 128,1,129 (10000000,00000001,10000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x80;  // 10000000
  Codes += 0x01;  // 00000001
  Codes += 0x81;  // 10000001
  CPPUNIT_ASSERT( Codes.size() == 3 );
  decoder( 7, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );

  //
  // bpp = 8
  // input: 256,1,257 (100000000,000000001,100000001)
  Codes.clear();
  Pixels.clear();
  Codes += static_cast<uint8_t>(0);  // 00000000
  Codes += 0x03;                     // 0000001,1
  Codes += 0x04;                     // 000001,00
  Codes += 0x04;                     // 00000,100
  CPPUNIT_ASSERT( Codes.size() == 4 );
  decoder( 8, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
  CPPUNIT_ASSERT( Pixels[0] == 1 );
}

// input string does not start with clear code
void GifDecoderTest::testNoClearCode()
{
  U8String Codes, Pixels;
  GifDecoder decoder;

  //
  // bpp = 1
  // input: 1,3 (01,11)
  Codes += 0x0D;  // 0000,11,01
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT_THROW( decoder( 1, Codes, Pixels ), vp::Exception );

  //
  // bpp = 2
  // input: 1,5 (001,101)
  Codes.clear();
  Pixels.clear();
  Codes += 0x29;  // 00,101,001
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT_THROW( decoder( 2, Codes, Pixels ), vp::Exception );

  //
  // bpp = 3
  // input: 1,9 (0001,1001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x91;  // 1001,0001
  CPPUNIT_ASSERT( Codes.size() == 1 );
  CPPUNIT_ASSERT_THROW( decoder( 3, Codes, Pixels ), vp::Exception );

  //
  // bpp = 4
  // input: 1,17 (00001,10001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x21;  // 001,00001
  Codes += 0x02;  // 000000,10
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT_THROW( decoder( 4, Codes, Pixels ), vp::Exception );

  //
  // bpp = 5
  // input: 1,33 (000001,100001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x41;  // 01,000001
  Codes += 0x08;  // 0000,1000
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT_THROW( decoder( 5, Codes, Pixels ), vp::Exception );

  //
  // bpp = 6
  // input: 1,65 (0000001,1000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x81;  // 1,0000001
  Codes += 0x20;  // 00,100000
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT_THROW( decoder( 6, Codes, Pixels ), vp::Exception );

  //
  // bpp = 7
  // input: 1,129 (00000001,10000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x01;  // 00000001
  Codes += 0x81;  // 10000001
  CPPUNIT_ASSERT( Codes.size() == 2 );
  CPPUNIT_ASSERT_THROW( decoder( 7, Codes, Pixels ), vp::Exception );

  //
  // bpp = 8
  // input: 1,257 (000000001,100000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x01;  // 00000001
  Codes += 0x02;  // 0000001,0
  Codes += 0x02;  // 000000,10
  CPPUNIT_ASSERT( Codes.size() == 3 );
  CPPUNIT_ASSERT_THROW( decoder( 8, Codes, Pixels ), vp::Exception );
}

// absent of EOI Code is tolerable
void GifDecoderTest::testNoEOICode()
{
  U8String Codes, Pixels;
  GifDecoder decoder;

  //
  // bpp = 1
  // input: 2,1,0 (10,01,00)
  Codes += 0x06;  // 00,00,01,10
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 1, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 2 );

  //
  // bpp = 2
  // input: 4,1 (100,001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x0C;  // 00,001,100
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 2, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  //
  // bpp = 3
  // input: 8,1 (1000,0001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x18;  // 0001,1000
  CPPUNIT_ASSERT( Codes.size() == 1 );
  decoder( 3, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  //
  // bpp = 4
  // input: 16,1,0 (10000,00001,00000)
  Codes.clear();
  Pixels.clear();
  Codes += 0x30;                     // 001,10000
  Codes += static_cast<uint8_t>(0);  // 0,00000,00
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 4, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 2 );

  //
  // bpp = 5
  // input: 32,1 (100000,000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x60;                     // 01,100000
  Codes += static_cast<uint8_t>(0);  // 0000,0000
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 5, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  //
  // bpp = 6
  // input: 64,1 (1000000,0000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0xC0;                     // 1,1000000
  Codes += static_cast<uint8_t>(0);  // 00,000000
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 6, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  //
  // bpp = 7
  // input: 128,1 (10000000,00000001)
  Codes.clear();
  Pixels.clear();
  Codes += 0x80;  // 10000000
  Codes += 0x01;  // 00000001
  CPPUNIT_ASSERT( Codes.size() == 2 );
  decoder( 7, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );

  //
  // bpp = 8
  // input: 256,1 (100000000,000000001)
  Codes.clear();
  Pixels.clear();
  Codes += static_cast<uint8_t>(0);  // 00000000
  Codes += 0x03;                     // 0000001,1
  Codes += static_cast<uint8_t>(0);  // 000000,00
  CPPUNIT_ASSERT( Codes.size() == 3 );
  decoder( 8, Codes, Pixels );
  CPPUNIT_ASSERT( Pixels.size() == 1 );
}

// bpp = 1
// Inverse of EncoderTest::testBpp1Codes()
void GifDecoderTest::testBpp1()
{
  U8String Pixels, Codes;

  // input: 2,0,1,4,5,3
  //       (10,00,001,100,101,011)
  Codes += 0x12;  // 0,001,00,10
  Codes += 0x76;  //  011,101,10
  CPPUNIT_ASSERT( Codes.size() == 2 );

  GifDecoder decoder;
  decoder( 1, Codes, Pixels );

  // output: 0,1,0,1,1,0
  U8String strOuput = { 0,1,0,1,1,0 };
  CPPUNIT_ASSERT( Pixels.size() == 6 );
  CPPUNIT_ASSERT( Pixels == strOuput );
}

// bpp = 2
// Inverse of EncoderTest::testBpp2Codes()
void GifDecoderTest::testBpp2()
{
  U8String Pixels, Codes;

  // input: 4,1,2,1,3,2,3,6,8,10,7,9,11,5 
  GifCodeWriter WriteCode( Codes ); 
  WriteCode( 4, 3 );  // clear code
  WriteCode( 1, 3 );
  WriteCode( 2, 3 );
  WriteCode( 1, 3 );
  WriteCode( 3, 4 );
  WriteCode( 2, 4 );
  WriteCode( 3, 4 );
  WriteCode( 6, 4 );
  WriteCode( 8, 4 );
  WriteCode( 10, 4 );
  WriteCode( 7, 4 );
  WriteCode( 9, 4 );
  WriteCode( 11, 5 );
  WriteCode( 5, 5 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 7 );  

  GifDecoder decoder;
  decoder( 2, Codes, Pixels );

  // output: 1,2,1,3,2,3,1,2,1,3,2,3,2,1,3,2,3,1
  U8String strExpected = { 1,2,1,3,2,3,1,2,1,3,2,3,2,1,3,2,3,1 };
  CPPUNIT_ASSERT( Pixels.size() == 18 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 3
// Inverse of EncoderTest::testBpp3Codes()
void GifDecoderTest::testBpp3()
{
  U8String Pixels, Codes;

  // input: 8,1,2,1,3,1,4,2,3,2,4,3,4,10,12,14,16,18,20,11,13,15,17,19,21,9
  GifCodeWriter WriteCode( Codes ); 

  // 4-bit: 8,1,2,1,3,1,4,2
  WriteCode( 8, 4 );  // clear code
  uint8_t i;
  for( i = 2; i <= 4; ++i )
  {
    WriteCode( 1, 4 );
    WriteCode( i, 4 );
  }
  WriteCode( 2, 4 );

  // 5-bit: 3,2,4,3,4,10,12,14,16,18,20,11,13,15,17,19
  WriteCode( 3, 5 );
  for( i = 2; i <= 3; ++i )
  {
    WriteCode( i, 5 );
    WriteCode( 4, 5 );
  }
  for( i = 10; i <= 20; i += 2 )
    WriteCode( i, 5 );
  for( i = 11; i <= 19; i += 2 )
    WriteCode( i, 5 );

  // 6-bit: 21,9
  WriteCode( 21, 6 );
  WriteCode( 9, 9 );  // EOI code
  WriteCode.End();

  CPPUNIT_ASSERT( Codes.size() == 16 );

  GifDecoder decoder;
  decoder( 3, Codes, Pixels );

  // output: 1,2,1,3,1,4,2,3,2,4,3,4,
  //         1,2,1,3,1,4,2,3,2,4,3,4,
  //           2,1,3,1,4,2,3,2,4,3,4,1
  U8String strExpected = {1,2,1,3,1,4,2,3,2,4,3,4,1,2,1,3,1,4,2,3,2,4,3,4,2,1,3,1,4,2,3,2,4,3,4,1};
  CPPUNIT_ASSERT( Pixels.size() == 36 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 4
// Inverse of EncoderTest::testBpp4Codes()
void GifDecoderTest::testBpp4()
{
  U8String Pixels, Codes;

  // input: 16,1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,
  //        18,20,22,24,26,28,30,32,34,36,
  //        19,21,23,25,27,29,31,33,35,37,17
  GifCodeWriter WriteCode( Codes ); 

  // 5-bit: 16,1,2,1,3,1,4,1,5,2,3,2,4,2,5,3
  WriteCode( 16, 5 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 5; ++j )
    {
      WriteCode( i, 5 );
      WriteCode( j, 5 );
    }
  }
  WriteCode( 3, 5 );

  // 6-bit: 4,3,5,4,5,18,20,22,24,26,28,30,32,34,36,19,21,23,25,27,29,31,33,35,37,17
  WriteCode( 4, 6 );
  for( i = 3; i <= 4; ++i )
  {
    WriteCode( i, 6 );
    WriteCode( 5, 6 );
  }
  for( i = 18; i <= 36; i += 2 )
    WriteCode( i, 6 );
  for( i = 19; i <= 37; i += 2 )
    WriteCode( i, 6 );
  WriteCode( 17, 6 );  // EOI code
  WriteCode.End();

  CPPUNIT_ASSERT( Codes.size() == 30 );

  GifDecoder decoder;
  decoder( 4, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,
  //         1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,
  //           2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1
  U8String strBase = {1,2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5};
  U8String strExpected = strBase + strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,2,3,2,4,2,5,3,4,3,5,4,5,1
  strExpected += strBase;
  strExpected += 1;
  CPPUNIT_ASSERT( Pixels.size() == 60 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 5
// Inverse of EncoderTest::testBpp5Codes()
void GifDecoderTest::testBpp5()
{
  U8String Pixels, Codes;

  // input: 32,1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  //        34,36,38,40,...72,74,
  //        35,37,39,41,...73,75,33
  GifCodeWriter WriteCode( Codes ); 

  // 6-bit: 32,1,2,1,3,1,4,1,5,...3,6,3,7,4
  WriteCode( 32, 6 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 3; ++i )
  {
    for( j = i + 1; j <= 7; ++j )
    {
      WriteCode( i, 6 ); 
      WriteCode( j, 6 ); 
    }
  }
  WriteCode( 4, 6 ); 

  // 7-bit: 5,4,6,4,7,5,6,5,7,6,7,34,36,38,40,...72,74,1,33
  WriteCode( 5, 7 ); 
  for( i = 4; i <= 5; ++i )
  {
    for( j = 6; j <= 7; ++j )
    {
      WriteCode( i, 7 ); 
      WriteCode( j, 7 ); 
    }
  }
  WriteCode( 6, 7 ); 
  WriteCode( 7, 7 ); 
  for( i = 34; i <= 74; i +=2 )
    WriteCode( i, 7 ); 
  for( i = 35; i <= 75; i +=2 )
    WriteCode( i, 7 ); 
  WriteCode( 33, 7 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 72 );

  GifDecoder decoder;
  decoder( 5, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  //         1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  //           2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,1
  U8String strBase;  // 1,2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7,
  for( i = 1; i <= 6; ++i )
  {
    for( j = i + 1; j <= 7; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  U8String strExpected = strBase + strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,1,6,1,7,...5,6,5,7,6,7
  strExpected += strBase;
  strExpected += 1;
  CPPUNIT_ASSERT( Pixels.size() == 126 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 6
// Inverse of EncoderTest::testBpp6Codes()
void GifDecoderTest::testBpp6()
{
  U8String Pixels, Codes;

  // input: 64,1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,
  //        66,68,70,...134,136,
  //        67,69,71,...135,137,65
  GifCodeWriter WriteCode( Codes ); 

  // 7-bit: 64,1,2,1,3,1,4,1,5,...6,7,6 
  WriteCode( 64, 7 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 5; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      WriteCode( i, 7 ); 
      WriteCode( j, 7 ); 
    }
  }
  WriteCode( 6, 7 ); 
  WriteCode( 7, 7 ); 
  WriteCode( 6, 7 ); 

  // 8-bit: 8,6,9,7,8,7,9,8,9,66,68,70,...134,136,1,65
  WriteCode( 8, 8 ); 
  WriteCode( 6, 8 ); 
  WriteCode( 9, 8 ); 
  for( i = 7; i <= 8; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      WriteCode( i, 8 ); 
      WriteCode( j, 8 ); 
    }
  }
  for( i = 66; i <= 136; i += 2 )
    WriteCode( i, 8 ); 
  for( i = 67; i <= 137; i += 2 )
    WriteCode( i, 8 ); 
  WriteCode( 65, 8 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 138 );

  GifDecoder decoder;
  decoder( 6, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,
  //         1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,
  //           2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9,1
  U8String strBase;  // 1,2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9
  for( i = 1; i <= 8; ++i )
  {
    for( j = i + 1; j <= 9; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  U8String strExpected = strBase + strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,1,6,1,7,...7,8,7,9,8,9
  strExpected += strBase;
  strExpected += 1;
  CPPUNIT_ASSERT( Pixels.size() == 216 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 7
// Inverse of EncoderTest::testBpp7Codes()
void GifDecoderTest::testBpp7()
{
  U8String Pixels, Codes;

  // input: 128,1,2,1,3,1,4,1,5,...10,11,10,12,11,12,
  //        130,132,134,...258,260
  //        131,133,135,...259,261,129
  GifCodeWriter WriteCode( Codes ); 

  // 8-bit: 128,1,2,1,3,1,4,1,5,...9,10,9,11,9,12,10
  WriteCode( 128, 8 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 9; ++i )
  {
    for( j = i + 1; j <= 12; ++j )
    {
      WriteCode( i, 8 ); 
      WriteCode( j, 8 ); 
    }
  }
  WriteCode( 10, 8 ); 

  // 9-bit: 11,10,12,11,12,130,132,134,...258,260,1,129
  WriteCode( 11, 9 ); 
  for( i = 10; i <= 11; ++i )
  {
    WriteCode( i, 9 ); 
    WriteCode( 12, 9 ); 
  }
  for( uint16_t k = 130; k <= 260; k += 2 )
    WriteCode( k, 9 ); 
  for( uint16_t k = 131; k <= 261; k += 2 )
    WriteCode( k, 9 ); 
  WriteCode( 129, 9 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 284 );

  GifDecoder decoder;
  decoder( 7, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,...10,11,10,12,11,12,
  //         1,2,1,3,1,4,1,5,...10,11,10,12,11,12,
  //           2,1,3,1,4,1,5,...10,11,10,12,11,12,1
  U8String strBase;  // 1,2,1,3,1,4,1,5,...10,11,10,12,11,12
  for( i = 1; i <= 11; ++i )
  {
    for( j = i + 1; j <= 12; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  U8String strExpected = strBase + strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,...10,11,10,12,11,12
  strExpected += strBase;
  strExpected += 1;
  CPPUNIT_ASSERT( Pixels.size() == 396 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 8
// Inverse of EncoderTest::testBpp8Codes()
void GifDecoderTest::testBpp8()
{
  U8String Pixels, Codes;

  // input: 256,1,2,1,3,1,4,1,5,...15,16,15,17,16,17,
  //          258,260,262,...526,528,
  //          259,261,263,...527,529,257
  GifCodeWriter WriteCode( Codes ); 

  // 9-bit: 256,1,2,1,3,1,4,1,5,...13,14,13
  WriteCode( 256, 9 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 12; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      WriteCode( i, 9 ); 
      WriteCode( j, 9 ); 
    }
  }
  WriteCode( 13, 9 ); 
  WriteCode( 14, 9 ); 
  WriteCode( 13, 9 ); 

  // 10-bit:  15,13,16,13,17,14,15,14,16,14,17,15,16,15,17,16,17,
  //          258,260,262,...526,528,
  //          259,261,263,...527,529,257
  WriteCode( 15, 10 ); 
  for( i = 16; i <= 17; ++i )
  {
    WriteCode( 13, 10 ); 
    WriteCode( i, 10 ); 
  }
  for( i = 14; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      WriteCode( i, 10 ); 
      WriteCode( j, 10 ); 
    }
  }
  for( uint16_t k = 258; k <= 528; k += 2 )
    WriteCode( k, 10 ); 
  for( uint16_t k = 259; k <= 529; k += 2 )
    WriteCode( k, 10 ); 
  WriteCode( 257, 10 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 651 );

  GifDecoder decoder;
  decoder( 8, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,...15,16,15,17,16,17,
  //         1,2,1,3,1,4,1,5,...15,16,15,17,16,17,
  //           2,1,3,1,4,1,5,...15,16,15,17,16,17,1
  U8String strBase;  // 1,2,1,3,1,4,1,5,...10,11,10,12,11,12
  for( i = 1; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      strBase += i;
      strBase += j;
    }
  }
  U8String strExpected = strBase + strBase;
  strBase.erase( 0, 1 );  // 2,1,3,1,4,1,5,...10,11,10,12,11,12
  strExpected += strBase;
  strExpected += 1;
  CPPUNIT_ASSERT( Pixels.size() == 816 );
  CPPUNIT_ASSERT( Pixels == strExpected );
}

// bpp = 8
// Inverse of EncoderTest::testBpp8MaxCode()
void GifDecoderTest::testBpp8MaxCode()
{
  U8String Pixels, Codes;

  // input: 256,1,2,1,3,1,4,1,5,...55,56,55,57,55,257
  GifCodeWriter WriteCode( Codes ); 

  // 9-bit: 256,1,2,1,3,1,4,1,5,...3,4,3,5,3,6,3,7,3
  WriteCode( 256, 9 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 9 ); 
      WriteCode( j, 9 ); 
    }
  }
  for( j = 4; j <= 7; ++j )
  {
    WriteCode( 3, 9 ); 
    WriteCode( j, 9 ); 
  }
  WriteCode( 3, 9 ); 

  // 10-bit:  8,3,9,3,10,3,11,...7,8,7,9,...7,33,7
  WriteCode( 8, 10 ); 
  for( j = 9; j <= 63; ++j )  // 3,9,3,10,...3,63 (110 numbers)
  {
    WriteCode( 3, 10 ); 
    WriteCode( j, 10 ); 
  }
  for( i = 4; i <= 6; ++i )  // 4,5,4,6,4,7,...6,62,6,63 (348 numbers)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 10 ); 
      WriteCode( j, 10 ); 
    }
  }
  for( j = 8; j <= 33; ++j )  // 7,8,7,9,...7,33 (52 numbers)
  {
    WriteCode( 7, 10 ); 
    WriteCode( j, 10 ); 
  }
  WriteCode( 7, 10 ); 

  // 11-bit: 34,7,35,7,36,...17,39,17,40,17 (1024*11 = 11264 bits)
  WriteCode( 34, 11 ); 
  for( j = 35 ; j <= 63; ++j )  // 7,35,7,36,...7,63 (58 numbers)
  {
    WriteCode( 7, 11 ); 
    WriteCode( j, 11 ); 
  }
  for( i = 8; i <= 16; ++i )  // 8,9,8,10,...16,62,16,63 (918 numbers)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 11 ); 
      WriteCode( j, 11 ); 
    }
  }
  for( j = 18; j <= 40; ++j )  // 17,18,17,19,...17,40 (46 numbers)
  {
    WriteCode( 17, 11 ); 
    WriteCode( j, 11 ); 
  }
  WriteCode( 17, 11 ); 

  // 12-bit: 41,17,42,17,43,17,44,...55,56,55,57,55 (2049*12 = 24588 bits)
  WriteCode( 41, 12 ); 
  for( j = 42; j <= 63; ++j )  // 17,42,17,43,...17,63 (44 numbers)
  {
    WriteCode( 17, 12 ); 
    WriteCode( j, 12 ); 
  }
  for( i = 18; i <= 54; ++i )  // 18,19,18,20,...54,62,54,63 (1998 numbers)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 12 ); 
      WriteCode( j, 12 ); 
    }
  }
  for( j = 56; j <= 57; ++j ) // 55,56,55,57 (4 numbers)
  {
    WriteCode( 55, 12 ); 
    WriteCode( j, 12 ); 
  }
  WriteCode( 55, 12 ); 
  WriteCode( 257, 12 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 5410 );

  GifDecoder decoder;
  decoder( 8, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,...55,56,55,57,55
  CPPUNIT_ASSERT( Pixels.size() == 3839 );
  uint16_t k = 0;
  for( i = 1; i <= 54; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( Pixels[k] == i ); ++k;
      CPPUNIT_ASSERT( Pixels[k] == j ); ++k;
    }
  }
  for( j = 56; j <= 57; ++j )  // 5 numbers
  {
    CPPUNIT_ASSERT( Pixels[k] == 55 ); ++k;
    CPPUNIT_ASSERT( Pixels[k] == j ); ++k;
  }
  CPPUNIT_ASSERT( Pixels[k] == 55 );
  CPPUNIT_ASSERT( k == 3838 );
}

// bpp = 8
// Inverse of EncoderTest::testBpp8ExceedMaxCode()
void GifDecoderTest::testBpp8ExceedMaxCode()
{
  U8String Pixels, Codes;

  // input: 256,1,2,1,3,1,4,1,5,...55,56,55,57,55,
  //        256,58,55,59,55,60,...61,62,61,63,62,63,1,257
  GifCodeWriter WriteCode( Codes ); 

  // 9-bit: 256,1,2,1,3,1,4,1,5,...3,4,3,5,3,6,3,7,3
  WriteCode( 256, 9 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 2; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 9 ); 
      WriteCode( j, 9 ); 
    }
  }
  for( j = 4; j <= 7; ++j )
  {
    WriteCode( 3, 9 ); 
    WriteCode( j, 9 ); 
  }
  WriteCode( 3, 9 ); 

  // 10-bit:  8,3,9,3,10,3,11,...7,8,7,9,...7,33,7
  WriteCode( 8, 10 ); 
  for( j = 9; j <= 63; ++j )  // 3,9,3,10,...3,63 (110 numbers)
  {
    WriteCode( 3, 10 ); 
    WriteCode( j, 10 ); 
  }
  for( i = 4; i <= 6; ++i )  // 4,5,4,6,4,7,...6,62,6,63 (348 numbers)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 10 ); 
      WriteCode( j, 10 ); 
    }
  }
  for( j = 8; j <= 33; ++j )  // 7,8,7,9,...7,33 (52 numbers)
  {
    WriteCode( 7, 10 ); 
    WriteCode( j, 10 ); 
  }
  WriteCode( 7, 10 ); 

  // 11-bit: 34,7,35,7,36,...17,39,17,40,17 (1024*11 = 11264 bits)
  WriteCode( 34, 11 ); 
  for( j = 35 ; j <= 63; ++j )  // 7,35,7,36,...7,63 (58 numbers)
  {
    WriteCode( 7, 11 ); 
    WriteCode( j, 11 ); 
  }
  for( i = 8; i <= 16; ++i )  // 8,9,8,10,...16,62,16,63 (918 numbers)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 11 ); 
      WriteCode( j, 11 ); 
    }
  }
  for( j = 18; j <= 40; ++j )  // 17,18,17,19,...17,40 (46 numbers)
  {
    WriteCode( 17, 11 ); 
    WriteCode( j, 11 ); 
  }
  WriteCode( 17, 11 ); 

  // 12-bit: 41,17,42,17,43,17,44,...55,56,55,57,55 (2049*12 = 24588 bits)
  WriteCode( 41, 12 ); 
  for( j = 42; j <= 63; ++j )  // 17,42,17,43,...17,63 (44 numbers)
  {
    WriteCode( 17, 12 ); 
    WriteCode( j, 12 ); 
  }
  for( i = 18; i <= 54; ++i )  // 18,19,18,20,...54,62,54,63 (1998 numbers)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 12 ); 
      WriteCode( j, 12 ); 
    }
  }
  for( j = 56; j <= 57; ++j ) // 55,56,55,57 (4 numbers)
  {
    WriteCode( 55, 12 ); 
    WriteCode( j, 12 ); 
  }
  WriteCode( 55, 12 ); 
  WriteCode( 256, 12 );  // clear code 

  // 9-bit again: 58,55,59,55,60,55,61,...62,63,1,257 (69*9 = 621 bits)
  WriteCode( 58, 9 ); 
  for( j = 59; j <= 63; ++j )  // 55,59,55,60,...55,63 (10 numbers)
  {
    WriteCode( 55, 9 ); 
    WriteCode( j, 9 ); 
  }
  for( i = 56; i <= 62; ++i )  // 56,57,56,58,...62,63 (56 number)
  {
    for( j = i + 1; j <= 63; ++j )
    {
      WriteCode( i, 9 ); 
      WriteCode( j, 9 ); 
    }
  }
  WriteCode( 1, 9 );
  WriteCode( 257, 9 );  // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 5488 );

  GifDecoder decoder;
  decoder( 8, Codes, Pixels );

  // output: 1,2,1,3,1,4,1,5,...55,56,55,57,55,
  //         58,55,59,55,60,...61,62,61,63,62,63,1
  CPPUNIT_ASSERT( Pixels.size() == 3907 );
  uint16_t k = 0;
  for( i = 1; i <= 62; ++i )
  {
    for( j = i + 1; j <= 63; ++j )
    {
      CPPUNIT_ASSERT( Pixels[k] == i ); ++k;
      CPPUNIT_ASSERT( Pixels[k] == j ); ++k;
    }
  }
  CPPUNIT_ASSERT( Pixels[k] == 1 );
  CPPUNIT_ASSERT( k == 3906 );
}

// bpp = 8
// Inverse of EncoderTest::testBpp8AllCodes()
void GifDecoderTest::testBpp8AllCodes()
{
  U8String Pixels, Codes;

  // input: 256,1,258,259,...,4094,4095,257
  GifCodeWriter WriteCode( Codes ); 

  // 9-bit: 256,1,258,259,...511 (clear code + 1 + 254 codes = 256 codes)
  WriteCode( 256, 9 );  // clear code
  WriteCode( 1, 9 );
  uint16_t i;
  for( i = 258; i <= 511; ++i )
    WriteCode( i, 9 );

  // 10-bit: 512,513,...,1023 (512 codes)
  for( i = 512; i <= 1023; ++i )
    WriteCode( i, 10 );

  // 11-bit: 1024,1025,...2047 (1024 codes)
  for( i = 1024; i <= 2047; ++i )
    WriteCode( i, 11 );

  // 12-bit: 2048,2049,...4095 (2048 codes + EOI = 2049 codes)
  for( i = 2048; i <= 4095; ++i )
    WriteCode( i, 12 );
  WriteCode( 257, 12 ); // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 5410 );

  GifDecoder decoder;
  decoder( 8, Codes, Pixels );

  // output: 1,1,1,1,.... (7370880 1's)
  CPPUNIT_ASSERT( Pixels.size() == 7370880 );
  // verifying sum is faster than doing each pixel
  size_t sum = 0;
  for( size_t i = 0; i < 7370880; ++i )
    sum += Pixels[i];
  CPPUNIT_ASSERT( sum == 7370880 );
}
