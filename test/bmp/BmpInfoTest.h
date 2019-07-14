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

// Unit test for BmpInfo

#ifndef BmpInfoTest_h
#define BmpInfoTest_h

#include <cppunit/extensions/HelperMacros.h>


/////////////////////
class BmpInfoTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( BmpInfoTest );
  CPPUNIT_TEST( testSupported );
  CPPUNIT_TEST( testPaddingBytes );
  CPPUNIT_TEST( testCreate );
  CPPUNIT_TEST_SUITE_END();

protected:
  void testSupported();
  void testPaddingBytes();
  void testCreate();
};

#endif  // BmpInfoTest_h
