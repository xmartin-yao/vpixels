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

#include <ctime>
#include "GifEncoderDecoderTest.h"
#include "GifEncoder.h"
#include "GifDecoder.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifEncoderDecoderTest );

void GifEncoderDecoderTest::testBpp1()
{
  srand(time(nullptr));

  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 2;

  GifEncoder encoder;
  encoder( 1, Pixels, Codes );

  GifDecoder decoder;
  decoder( 1, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp2()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 4;

  GifEncoder encoder;
  encoder( 2, Pixels, Codes );

  GifDecoder decoder;
  decoder( 2, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp3()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 8;

  GifEncoder encoder;
  encoder( 3, Pixels, Codes );

  GifDecoder decoder;
  decoder( 3, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp4()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 16;

  GifEncoder encoder;
  encoder( 4, Pixels, Codes );

  GifDecoder decoder;
  decoder( 4, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp5()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 32;

  GifEncoder encoder;
  encoder( 5, Pixels, Codes );

  GifDecoder decoder;
  decoder( 5, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp6()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 64;

  GifEncoder encoder;
  encoder( 6, Pixels, Codes );

  GifDecoder decoder;
  decoder( 6, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp7()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 128;

  GifEncoder encoder;
  encoder( 7, Pixels, Codes );

  GifDecoder decoder;
  decoder( 7, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}

void GifEncoderDecoderTest::testBpp8()
{
  U8String Pixels, Codes, DecodedPixels;
  uint16_t nPixels = 5000;
  uint16_t n;
  for( n = 0; n < nPixels; ++n ) 
    Pixels += rand() % 256;

  GifEncoder encoder;
  encoder( 8, Pixels, Codes );

  GifDecoder decoder;
  decoder( 8, Codes, DecodedPixels );

  for( n = 0; n < nPixels; ++n ) 
    CPPUNIT_ASSERT( Pixels[n] == DecodedPixels[n] );
}
