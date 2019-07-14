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

#include "GifStringTableTest.h"
#include "GifStringTable.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifStringTableTest );

// test Search() and Add() that have no index as argument
void GifStringTableTest::testNoIndex()
{
  GifStringTable st;

  CPPUNIT_ASSERT( st.Search( 4, 1 ) == 0 );
  CPPUNIT_ASSERT( st.Search( 34, 2 ) == 0 );
  CPPUNIT_ASSERT( st.Search( 258, 9 ) == 0 );

  st.Add( 4, 1, 5 );
  st.Add( 34, 2, 36 );
  st.Add( 258, 9, 267 );
  CPPUNIT_ASSERT( st.Search( 4, 1 ) == 5 );
  CPPUNIT_ASSERT( st.Search( 34, 2 ) == 36 );
  CPPUNIT_ASSERT( st.Search( 258, 9 ) == 267 );

  st.Reset();
  CPPUNIT_ASSERT( st.Search( 4, 1 ) == 0 );
  CPPUNIT_ASSERT( st.Search( 34, 2 ) == 0 );
  CPPUNIT_ASSERT( st.Search( 258, 9 ) == 0 );
}

// test Search() and Add() that have index as argument
void GifStringTableTest::testWithIndex()
{
  GifStringTable st;
  uint16_t index1, index2;

  CPPUNIT_ASSERT( st.Search( 4, 1, index1 ) == 0 );
  st.Add( index1, 4, 1, 5 );
  CPPUNIT_ASSERT( st.Search( 4, 1, index2 ) == 5 );
  CPPUNIT_ASSERT( index1 == index2 );

  CPPUNIT_ASSERT( st.Search( 34, 2, index1 ) == 0 );
  st.Add( index1, 34, 2, 36 );
  CPPUNIT_ASSERT( st.Search( 34, 2, index2 ) == 36 );
  CPPUNIT_ASSERT( index1 == index2 );

  CPPUNIT_ASSERT( st.Search( 258, 9, index1 ) == 0 );
  st.Add( index1, 258, 9, 267 );
  CPPUNIT_ASSERT( st.Search( 258, 9, index2 ) == 267 );
  CPPUNIT_ASSERT( index1 == index2 );

  st.Reset();
  CPPUNIT_ASSERT( st.Search( 4, 1, index1 ) == 0 );
  CPPUNIT_ASSERT( st.Search( 34, 2, index1 ) == 0 );
  CPPUNIT_ASSERT( st.Search( 258, 9, index1 ) == 0 );
}
