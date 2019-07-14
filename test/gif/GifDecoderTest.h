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
// Unit test for GifDecoder

#ifndef GifDecoderTest_h
#define GifDecoderTest_h

#include <cppunit/extensions/HelperMacros.h>


/////////////////////
class GifDecoderTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( GifDecoderTest );

  CPPUNIT_TEST( testZeroPixel );
  CPPUNIT_TEST( testOnePixel );
  CPPUNIT_TEST( testNoClearCode );
  CPPUNIT_TEST( testNoEOICode );

  CPPUNIT_TEST( testBpp1 );
  CPPUNIT_TEST( testBpp2 );
  CPPUNIT_TEST( testBpp3 );
  CPPUNIT_TEST( testBpp4 );
  CPPUNIT_TEST( testBpp5 );
  CPPUNIT_TEST( testBpp6 );
  CPPUNIT_TEST( testBpp7 );
  CPPUNIT_TEST( testBpp8 );

  CPPUNIT_TEST( testBpp8MaxCode );
  CPPUNIT_TEST( testBpp8ExceedMaxCode );
  CPPUNIT_TEST( testBpp8AllCodes );

  CPPUNIT_TEST_SUITE_END();

protected:
  void testZeroPixel();
  void testOnePixel();
  void testNoClearCode();
  void testNoEOICode();

  void testBpp1();
  void testBpp2();
  void testBpp3();
  void testBpp4();
  void testBpp5();
  void testBpp6();
  void testBpp7();
  void testBpp8();

  void testBpp8MaxCode();
  void testBpp8ExceedMaxCode();
  void testBpp8AllCodes();
};

#endif //GifDecoderTest_h
