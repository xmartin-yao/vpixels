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
// Unit test for GifHeader

#ifndef GifHeaderTest_h
#define GifHeaderTest_h

#include <cppunit/extensions/HelperMacros.h>

/////////////////////
class GifHeaderTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE( GifHeaderTest );
  CPPUNIT_TEST( testCtor );
  CPPUNIT_TEST( testIO );
  CPPUNIT_TEST_SUITE_END();

protected:
  void testCtor();
  void testIO();
};

#endif //GifHeaderTest_h

