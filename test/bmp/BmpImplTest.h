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
class BmpImplTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( BmpImplTest );

  CPPUNIT_TEST( testCtors );

  CPPUNIT_TEST( testRead1Bit );
  CPPUNIT_TEST( testRead4Bits );
  CPPUNIT_TEST( testRead8Bits );
  CPPUNIT_TEST( testRead24Bits );

  CPPUNIT_TEST( testReadWrongID );
  CPPUNIT_TEST( testReadWrongFileHeaderSize );
  CPPUNIT_TEST( testReadWrongBpp );
  CPPUNIT_TEST( testReadWrongFileHeader );
  CPPUNIT_TEST( testReadWrongInfoHeader );

  CPPUNIT_TEST_SUITE_END();

protected:
  void testCtors();
  void testRead1Bit();
  void testRead4Bits();
  void testRead8Bits();
  void testRead24Bits();
  void testReadWrongID();
  void testReadWrongFileHeaderSize();
  void testReadWrongBpp();
  void testReadWrongFileHeader();
  void testReadWrongInfoHeader();
};

#endif //BmpTest_h

