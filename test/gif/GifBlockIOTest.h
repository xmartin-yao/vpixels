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
// Unit test for GifBlockIO

#ifndef GifBlockIOTest_h
#define GifBlockIOTest_h

#include <cppunit/extensions/HelperMacros.h>


/////////////////////
class GifBlockIOTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( GifBlockIOTest );

  CPPUNIT_TEST( testReadSubBlocks );
  CPPUNIT_TEST( testWriteSubBlocks );

  CPPUNIT_TEST( testReadSubBlocksChars );
  CPPUNIT_TEST( testWriteSubBlocksChars );

  CPPUNIT_TEST( testReadBlock );
  CPPUNIT_TEST( testWriteBlock );

  CPPUNIT_TEST( testReadBlockChars );
  CPPUNIT_TEST( testWriteBlockChars );

  CPPUNIT_TEST_SUITE_END();

public:
  virtual void setUp();

protected:
  void testReadSubBlocks();
  void testWriteSubBlocks();

  void testReadSubBlocksChars();
  void testWriteSubBlocksChars();

  void testReadBlock();
  void testWriteBlock();

  void testReadBlockChars();
  void testWriteBlockChars();

private:
  std::stringstream strstream;
};

#endif //GifBlockIOTest_h
