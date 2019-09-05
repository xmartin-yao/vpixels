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

#include "GifImageVecBuilderTest.h"
#include "GifImageVecBuilder.h"
#include "GifImage.h"
#include "GifImpl.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifImageVecBuilderTest );

void GifImageVecBuilderTest::testOneImage()
{
  GifImpl gifimpl( 2, 3, 3 );
  GifImageVec iv = GifImageVecBuilder()( gifimpl, gifimpl.m_ComponentVec );
  CPPUNIT_ASSERT( iv.size() == 1 );

  CPPUNIT_ASSERT( iv[0]->GetPixel( 1, 1 ) == 0 );
  iv[0]->SetAllPixels( 1 );
  CPPUNIT_ASSERT( iv[0]->GetPixel( 1, 1 ) == 1 );
  CPPUNIT_ASSERT( iv[0]->Interlaced() == false );
  CPPUNIT_ASSERT( iv[0]->Delay() == 0 );
  CPPUNIT_ASSERT_THROW( iv[0]->Delay( 100 ), vp::Exception );
  CPPUNIT_ASSERT( iv[0]->ColorTable() == false );
}

void GifImageVecBuilderTest::testTwoImages()
{
  GifImpl gifimpl( 2, 3, 3, 2 );
  GifImageVec iv = GifImageVecBuilder()( gifimpl, gifimpl.m_ComponentVec );
  CPPUNIT_ASSERT( iv.size() == 2 );

  // 1st image
  CPPUNIT_ASSERT( iv[0]->GetPixel( 1, 1 ) == 0 );
  iv[0]->SetAllPixels( 1 );
  CPPUNIT_ASSERT( iv[0]->GetPixel( 1, 1 ) == 1 );
  CPPUNIT_ASSERT( iv[0]->Interlaced() == false );
  CPPUNIT_ASSERT( iv[0]->Delay() == 0 );
  iv[0]->Delay( 100 );
  CPPUNIT_ASSERT( iv[0]->Delay() == 100 );
  CPPUNIT_ASSERT( iv[0]->ColorTable() == false );

  // 2nd image
  CPPUNIT_ASSERT( iv[1]->GetPixel( 1, 1 ) == 0 );
  iv[1]->SetAllPixels( 1 );
  CPPUNIT_ASSERT( iv[1]->GetPixel( 1, 1 ) == 1 );
  CPPUNIT_ASSERT( iv[1]->Interlaced() == false );
  CPPUNIT_ASSERT( iv[1]->Delay() == 0 );
  iv[1]->Delay( 100 );
  CPPUNIT_ASSERT( iv[1]->Delay() == 100 );
  CPPUNIT_ASSERT( iv[1]->ColorTable() == false );
}
