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

// Unit test for BmpInfoHeader

#ifndef BmpInfoHeaderTest_h
#define BmpInfoHeaderTest_h

#include <cppunit/extensions/HelperMacros.h>

class BmpInfo;

/////////////////////
class BmpInfoHeaderTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( BmpInfoHeaderTest );
  CPPUNIT_TEST( testCtors );
  CPPUNIT_TEST( testAttributes );
  CPPUNIT_TEST( testIO );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

protected:
  void testCtors();
  void testAttributes();
  void testIO();

private:
  BmpInfo *m_pInfo1Bit, *m_pInfo4Bit, *m_pInfo8Bit, *m_pInfo24Bit;
};

#endif //BmpInfoHeaderTest_h
