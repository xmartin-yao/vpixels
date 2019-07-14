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
// Unit tests for GifEncoder

#ifndef GifEncoderTest_h
#define GifEncoderTest_h

#include <cppunit/extensions/HelperMacros.h>

/////////////////////
class GifEncoderTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( GifEncoderTest );

  CPPUNIT_TEST( testZeroPixel );
  CPPUNIT_TEST( testOnePixel );

  CPPUNIT_TEST( testBpp1 );
  CPPUNIT_TEST( testBpp1Codes );

  CPPUNIT_TEST( testBpp2 );
  CPPUNIT_TEST( testBpp2Codes );

  CPPUNIT_TEST( testBpp3 );
  CPPUNIT_TEST( testBpp3Codes );

  CPPUNIT_TEST( testBpp4 );
  CPPUNIT_TEST( testBpp4Codes );

  CPPUNIT_TEST( testBpp5 );
  CPPUNIT_TEST( testBpp5Codes );

  CPPUNIT_TEST( testBpp6 );
  CPPUNIT_TEST( testBpp6Codes );

  CPPUNIT_TEST( testBpp7 );
  CPPUNIT_TEST( testBpp7Codes );

  CPPUNIT_TEST( testBpp8 );
  CPPUNIT_TEST( testBpp8Codes );

  CPPUNIT_TEST( testBpp8AllCodes );
  CPPUNIT_TEST( testBpp8MaxCode );
  CPPUNIT_TEST( testBpp8ExceedMaxCode );

  CPPUNIT_TEST_SUITE_END();

protected:
  void testZeroPixel();
  void testOnePixel();

  void testBpp1();
  void testBpp1Codes();

  void testBpp2();
  void testBpp2Codes();

  void testBpp3();
  void testBpp3Codes();

  void testBpp4();
  void testBpp4Codes();

  void testBpp5();
  void testBpp5Codes();

  void testBpp6();
  void testBpp6Codes();

  void testBpp7();
  void testBpp7Codes();

  void testBpp8();
  void testBpp8Codes();

  void testBpp8MaxCode();
  void testBpp8ExceedMaxCode();
  void testBpp8AllCodes();
};

#endif //GifEncoderTest_h
