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

// Unit tests for BmpInfo

#include "BmpInfoTest.h"
#include "BmpInfo.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpInfoTest );


/////////////////////////
void BmpInfoTest::testSupported()
{
  CPPUNIT_ASSERT( BmpInfo::Supported(1) );
  CPPUNIT_ASSERT( BmpInfo::Supported(4) );
  CPPUNIT_ASSERT( BmpInfo::Supported(8) );
  CPPUNIT_ASSERT( BmpInfo::Supported(24) );

  CPPUNIT_ASSERT( !BmpInfo::Supported(0) );
  CPPUNIT_ASSERT( !BmpInfo::Supported(2) );
  CPPUNIT_ASSERT( !BmpInfo::Supported(16) );
  CPPUNIT_ASSERT( !BmpInfo::Supported(32) );
}

/////////////////////////
void BmpInfoTest::testPaddingBytes()
{
  CPPUNIT_ASSERT( 3 == BmpInfo::PaddingBytes(1) );
  CPPUNIT_ASSERT( 2 == BmpInfo::PaddingBytes(2) );
  CPPUNIT_ASSERT( 1 == BmpInfo::PaddingBytes(3) );
  CPPUNIT_ASSERT( 0 == BmpInfo::PaddingBytes(4) );

  CPPUNIT_ASSERT( 2 == BmpInfo::PaddingBytes(10) );
  CPPUNIT_ASSERT( 0 == BmpInfo::PaddingBytes(20) );
  CPPUNIT_ASSERT( 2 == BmpInfo::PaddingBytes(30) );
  CPPUNIT_ASSERT( 0 == BmpInfo::PaddingBytes(40) );
}

/////////////////////////
void BmpInfoTest::testCreate()
{
  std::unique_ptr<BmpInfo> pInfo;

  // 1-bit
  pInfo = BmpInfo::Create( 1, 2, 3 );
  CPPUNIT_ASSERT( pInfo );
  CPPUNIT_ASSERT( pInfo->BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( pInfo->Width() == 2 );
  CPPUNIT_ASSERT( pInfo->Height() == 3 );

  // 4-bit
  pInfo = BmpInfo::Create( 4, 5, 6 );
  CPPUNIT_ASSERT( pInfo );
  CPPUNIT_ASSERT( pInfo->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( pInfo->Width() == 5 );
  CPPUNIT_ASSERT( pInfo->Height() == 6 );

  // 8-bit
  pInfo = BmpInfo::Create( 8, 7, 9 );
  CPPUNIT_ASSERT( pInfo );
  CPPUNIT_ASSERT( pInfo->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( pInfo->Width() == 7 );
  CPPUNIT_ASSERT( pInfo->Height() == 9 );

  // 24-bit
  pInfo = BmpInfo::Create( 24, 8, 9 );
  CPPUNIT_ASSERT( pInfo );
  CPPUNIT_ASSERT( pInfo->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( pInfo->Width() == 8 );
  CPPUNIT_ASSERT( pInfo->Height() == 9 );

  // no supported
  CPPUNIT_ASSERT_THROW( BmpInfo::Create( 2, 1, 1 ), vp::Exception);
}
