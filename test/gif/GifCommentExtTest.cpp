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

#include <iostream>
#include "GifCommentExtTest.h"
#include "GifCommentExt.h"


CPPUNIT_TEST_SUITE_REGISTRATION( GifCommentExtTest );

void GifCommentExtTest::testCtors()
{
  // ctor
  GifCommentExt ce1;
  CPPUNIT_ASSERT( ce1.ID() == 0x21 );
  CPPUNIT_ASSERT( ce1.Label() == 0xFE );
  CPPUNIT_ASSERT( ce1.Comment() == "" );
  ce1.Comment( "Test GifCommentExt." );
  CPPUNIT_ASSERT( ce1.Comment() == "Test GifCommentExt." );

  // copy ctor
  GifCommentExt ce2 = ce1;
  CPPUNIT_ASSERT( ce2.ID() == 0x21 );
  CPPUNIT_ASSERT( ce2.Label() == 0xFE );
  CPPUNIT_ASSERT( ce2.Comment() == "Test GifCommentExt." );
}

void GifCommentExtTest::testOutput()
{
  GifCommentExt ce;
  ce.Comment( "Test GifCommentExt." );

  std::stringstream stream;
  ce.Write( stream );
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 23 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x21 );      // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xFE );      // label
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 19 );        // sub-block size
  CPPUNIT_ASSERT( str.substr( 3, 19 ) == "Test GifCommentExt." ); // comment
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[22]) == 0x00 );     // terminator
}

void GifCommentExtTest::testInput()
{
  std::stringstream stream;
  stream << static_cast<uint8_t>(0x2C);  // ID
  stream << static_cast<uint8_t>(0xFE);  // label
  stream << static_cast<uint8_t>(0x13);  // app block size
  stream << "GifCommentExt Test.";          // comment
  stream << static_cast<uint8_t>(0x00);  // terminator

  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 23 );

  GifCommentExt ce;
  CPPUNIT_ASSERT( ce.Comment() == "" );
  CPPUNIT_ASSERT( stream.get() == 0x2C );  // ID
  CPPUNIT_ASSERT( stream.get() == 0xFE );  // label
  ce.Read( stream );
  CPPUNIT_ASSERT( ce.Comment() == "GifCommentExt Test." );
}
