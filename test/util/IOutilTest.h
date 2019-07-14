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
// Unit test for IOutil

#ifndef IOutilTest_h
#define IOutilTest_h

#include <cppunit/extensions/HelperMacros.h>


/////////////////////
class IOutilTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( IOutilTest );

  CPPUNIT_TEST( testRead16 );
  CPPUNIT_TEST( testRead16SignBit );

  CPPUNIT_TEST( testWrite16Unsiged );
  CPPUNIT_TEST( testWrite16 );
  CPPUNIT_TEST( testWrite16Negative );

  CPPUNIT_TEST( testRead32 );
  CPPUNIT_TEST( testRead32SignBit );

  CPPUNIT_TEST( testWrite32Unsiged );
  CPPUNIT_TEST( testWrite32 );
  CPPUNIT_TEST( testWrite32Negative );

  CPPUNIT_TEST_SUITE_END();

public:
  virtual void setUp();

protected:
  void testRead16();
  void testRead16SignBit();

  void testWrite16Unsiged();
  void testWrite16();
  void testWrite16Negative();

  void testRead32();
  void testRead32SignBit();

  void testWrite32Unsiged();
  void testWrite32();
  void testWrite32Negative();

private:
  std::stringstream strstream;
};

#endif //IOutilTest_h
