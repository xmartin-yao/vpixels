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
// Unit test for GifCodeWriter

#ifndef  GifCodeWriterTest_h
#define  GifCodeWriterTest_h

#include <cppunit/extensions/HelperMacros.h>


/////////////////////
class GifCodeWriterTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( GifCodeWriterTest );

  CPPUNIT_TEST( test1Bit );
  CPPUNIT_TEST( test2Bit );
  CPPUNIT_TEST( test3Bit );
  CPPUNIT_TEST( test4Bit );
  CPPUNIT_TEST( test5Bit );
  CPPUNIT_TEST( test6Bit );
  CPPUNIT_TEST( test7Bit );
  CPPUNIT_TEST( test8Bit );
  CPPUNIT_TEST( test9Bit );
  CPPUNIT_TEST( test10Bit );
  CPPUNIT_TEST( test11Bit );
  CPPUNIT_TEST( test12Bit );
  CPPUNIT_TEST( testMixed );
  CPPUNIT_TEST( testBuffer );

  CPPUNIT_TEST_SUITE_END();

protected:
  void test1Bit();
  void test2Bit();
  void test3Bit();
  void test4Bit();
  void test5Bit();
  void test6Bit();
  void test7Bit();
  void test8Bit();
  void test9Bit();
  void test10Bit();
  void test11Bit();
  void test12Bit();
  void testMixed();
  void testBuffer();
};

#endif //GifCodeWriterTest_h
