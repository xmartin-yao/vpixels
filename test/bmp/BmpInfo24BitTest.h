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
// Unit tests for BmpInfo24Bit

#ifndef BmpInfo24BitTest_h
#define BmpInfo24BitTest_h

#include <cppunit/extensions/HelperMacros.h>

class BmpInfo24Bit;


/////////////////////
class BmpInfo24BitTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( BmpInfo24BitTest );
  CPPUNIT_TEST( testBitsPerPixel );
  CPPUNIT_TEST( testRowLength );
  CPPUNIT_TEST( testByteArraySize );
  CPPUNIT_TEST( testImageDataSize );
  CPPUNIT_TEST( testByteArrayIndices );
  CPPUNIT_TEST( testColorTableSize );
  CPPUNIT_TEST( testGetColorIndex );
  CPPUNIT_TEST( testSetColorIndex );
  CPPUNIT_TEST_SUITE_END();

public:
  virtual void setUp();
  virtual void tearDown();

protected:
  void testBitsPerPixel();
  void testRowLength();
  void testByteArraySize();
  void testImageDataSize();
  void testByteArrayIndices();
  void testColorTableSize();
  void testGetColorIndex();
  void testSetColorIndex();

private:
  BmpInfo24Bit *m_p1x1, *m_p1x2, *m_p2x2, *m_p3x2, *m_p4x2,
               *m_p10x10, *m_p10x20, *m_p20x20, *m_p30x20, *m_p40x20;
};

#endif  // BmpInfo24BitTest_h
