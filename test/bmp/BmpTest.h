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
// Unit test for Bmp

#ifndef BmpTest_h
#define BmpTest_h

#include <cppunit/extensions/HelperMacros.h>

/////////////////////
class BmpTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( BmpTest );

  CPPUNIT_TEST( testCtors );

  CPPUNIT_TEST( test1Bit );
  CPPUNIT_TEST( test4Bits );
  CPPUNIT_TEST( test8Bits );
  CPPUNIT_TEST( test24Bits );

  CPPUNIT_TEST( testImport );
  CPPUNIT_TEST( testExport );

  CPPUNIT_TEST_SUITE_END();

protected:
  void testCtors();
  void test1Bit();
  void test4Bits();
  void test8Bits();
  void test24Bits();
  void testImport();
  void testExport();
};

#endif //BmpTest_h
