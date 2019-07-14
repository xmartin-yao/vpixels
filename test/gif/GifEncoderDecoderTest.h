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
// Test Encoder and GifDecoder together using randomly generated pixels.

#ifndef GifEncoderDecoderTest_h
#define GifEncoderDecoderTest_h

#include <cppunit/extensions/HelperMacros.h>


/////////////////////
class GifEncoderDecoderTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( GifEncoderDecoderTest );

  CPPUNIT_TEST( testBpp1 );
  CPPUNIT_TEST( testBpp2 );
  CPPUNIT_TEST( testBpp3 );
  CPPUNIT_TEST( testBpp4 );
  CPPUNIT_TEST( testBpp5 );
  CPPUNIT_TEST( testBpp6 );
  CPPUNIT_TEST( testBpp7 );
  CPPUNIT_TEST( testBpp8 );

  CPPUNIT_TEST_SUITE_END();

protected:
  void testBpp1();
  void testBpp2();
  void testBpp3();
  void testBpp4();
  void testBpp5();
  void testBpp6();
  void testBpp7();
  void testBpp8();
};

#endif //GifEncoderDecoderTest_h
