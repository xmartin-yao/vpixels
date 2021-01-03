////////////////////////////////////////////////////////////////////////
// Copyright (C) 2021 Xueyi Yao
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
// along with VPixels. If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#include "UtilTest.h"
#include "Util.h"

CPPUNIT_TEST_SUITE_REGISTRATION( UtilTest );

void UtilTest::testCheckVersion()
{
  const char V[] = {"0.7.2"};
  const char ReqV[] = {"0.7.1"};
  // check if V is a later version
  CPPUNIT_ASSERT( Util::CheckVersion( ReqV, V ) );
  // check V is exactly as ReqV
  CPPUNIT_ASSERT( !Util::CheckVersion( ReqV, V, true ) );

  // compare V to itself
  CPPUNIT_ASSERT( Util::CheckVersion( V, V ) );
  CPPUNIT_ASSERT( Util::CheckVersion( V, V, true ) );

  // leading 0 doesn't count
  const char V2[] = {"0.07.02"};
  CPPUNIT_ASSERT( Util::CheckVersion( V, V2, true ) );

  // if one of argument is not exact 3 fields, return false.
  const char V3[] = {"0.7.2.1"};
  const char V4[] = {"7.2"};
  CPPUNIT_ASSERT( !Util::CheckVersion( V, V3 ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V, V3, true ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V3, V ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V3, V, true ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V, V4 ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V, V4, true ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V4, V ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V4, V, true ) );

  // any field contains non-digit char, return false.
  const char V5[] = {"0.7.2a"};
  CPPUNIT_ASSERT( !Util::CheckVersion( V, V5 ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V, V5, true ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V5, V ) );
  CPPUNIT_ASSERT( !Util::CheckVersion( V5, V, true ) );
}
