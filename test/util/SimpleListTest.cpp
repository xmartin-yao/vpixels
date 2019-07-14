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

#include "SimpleListTest.h"
#include "SimpleList.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SimpleListTest );

// node: pointer to int
void SimpleListTest::testInt()
{
  int x0 = 0;
  int x1 = 1;
  int x2 = 2;
  int x3 = 3;

  SimpleList<int> list;
  list.Add( &x0 );
  list.Add( &x1 );
  list.Add( &x2 );

  // iterate
  int i = 2;
  list.Rewind();
  int* px = list.Next();
  while( px != nullptr )
  {
    CPPUNIT_ASSERT( *px == i );
    px = list.Next();
    --i;
  }

  // remove
  px = list.Remove( &x1 );
  CPPUNIT_ASSERT( *px == 1 );
  px = list.Remove( &x0 );
  CPPUNIT_ASSERT( *px == 0 );
  px = list.Remove( &x2 );
  CPPUNIT_ASSERT( *px == 2 );

  // not in list
  CPPUNIT_ASSERT( list.Remove( &x3 ) == nullptr );

  // already removed
  CPPUNIT_ASSERT( list.Remove( &x0 ) == nullptr );
  CPPUNIT_ASSERT( list.Remove( &x1 ) == nullptr );
  CPPUNIT_ASSERT( list.Remove( &x2 ) == nullptr );

  // add again, for code coverage of dtor
  list.Add( &x0 );
  list.Add( &x1 );
  list.Add( &x2 );
}

// node: pointer to struct
void SimpleListTest::testStruct()
{
  struct Data
  {
    Data( int i, float f ) : m_i(i), m_f(f) {}
    int   m_i;
    float m_f;
  };

  Data* pD0 = new Data( 0, 0.0 );
  Data* pD1 = new Data( 1, 1.0 );
  Data* pD2 = new Data( 2, 2.0 );
  Data* pD3 = new Data( 3, 3.0 );
  
  SimpleList<Data> list;
  list.Add( pD0 );
  list.Add( pD1 );
  list.Add( pD2 );

  // iterate
  int   i = 2;
  float f = 2.0;
  list.Rewind();
  Data* pD = list.Next();
  while( pD != nullptr )
  {
    CPPUNIT_ASSERT( pD->m_i == i && pD->m_f == f );
    pD = list.Next();
    --i;
    --f;
  }

  // remove
  pD = list.Remove( pD2 );
  CPPUNIT_ASSERT( pD->m_i == 2 && pD->m_f == 2.0 );
  pD = list.Remove( pD0 );
  CPPUNIT_ASSERT( pD->m_i == 0 && pD->m_f == 0.0 );
  pD = list.Remove( pD1 );
  CPPUNIT_ASSERT( pD->m_i == 1 && pD->m_f == 1.0 );

  // not in list
  CPPUNIT_ASSERT( list.Remove( pD3 ) == nullptr );

  // already removed
  CPPUNIT_ASSERT( list.Remove( pD0 ) == nullptr );
  CPPUNIT_ASSERT( list.Remove( pD1 ) == nullptr );
  CPPUNIT_ASSERT( list.Remove( pD2 ) == nullptr );

  // add again, for code coverage of dtor
  list.Add( pD0 );
  list.Add( pD1 );
  list.Add( pD2 );

  delete pD0;
  delete pD1;
  delete pD2;
  delete pD3;
}

