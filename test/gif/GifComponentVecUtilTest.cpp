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

#include "GifComponentVecUtilTest.h"
#include "GifComponentVecUtil.h"
#include "GifApplicationExt.h"
#include "GifGraphicsControlExt.h"
#include "GifImageDescriptor.h"
#include "GifCommentExt.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifComponentVecUtilTest );

void GifComponentVecUtilTest::testRead()
{
  // prepare stream
  std::stringstream stream;
  GifApplicationExt app;
  app.Write( stream );
  GifGraphicsControlExt gc;
  gc.Write( stream );
  GifImageDescriptor id;
  id.Write( stream );
  GifCommentExt ce;
  ce.Write( stream );
  stream << static_cast<uint8_t>(0x3B);  // Gif Trailer

  // read from stream
  GifComponentVec cv;
  CPPUNIT_ASSERT( GifComponentVecUtil::Read( stream, cv ) == 0x3B );
  CPPUNIT_ASSERT( cv.size() == 4 );

  CPPUNIT_ASSERT( typeid( *cv[0] ) == typeid( GifApplicationExt ) );
  CPPUNIT_ASSERT( cv[0]->ID() == 0x21 );
  CPPUNIT_ASSERT( cv[0]->Label() == 0xFF );

  CPPUNIT_ASSERT( typeid( *cv[1] ) == typeid( GifGraphicsControlExt ) );
  CPPUNIT_ASSERT( cv[1]->ID() == 0x21 );
  CPPUNIT_ASSERT( cv[1]->Label() == 0xF9 );

  CPPUNIT_ASSERT( typeid( *cv[2] ) == typeid( GifImageDescriptor ) );
  CPPUNIT_ASSERT( cv[2]->ID() == 0x2C );
  CPPUNIT_ASSERT( cv[2]->Label() == 0x00 );

  CPPUNIT_ASSERT( typeid( *cv[3] ) == typeid( GifCommentExt ) );
  CPPUNIT_ASSERT( cv[3]->ID() == 0x21 );
  CPPUNIT_ASSERT( cv[3]->Label() == 0xFE );
}

void GifComponentVecUtilTest::testWrite()
{
  // prepare vec
  GifComponentVec cv;
  cv.push_back( std::shared_ptr<GifComponent>(new GifGraphicsControlExt()) );
  cv.push_back( std::shared_ptr<GifComponent>(new GifImageDescriptor()) );

  // write vec into stream
  std::stringstream stream;
  GifComponentVecUtil::Write( stream, cv );
  std::string str = stream.str();

  // GifGraphicsControlExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x21 );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xF9 );  // label

  // GifImageDescriptor
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0x2C );  // ID

  // GifImageData: 4,5 (Clear Code and EOI, 2 bits/each)
  // default ctor of GifImageData instantiates an object with bpp = 2 and no pixel.
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[18]) == 0x02 );  // bpp
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[19]) == 0x01 );  // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[20]) == 0x2C );  // codes: 4,5
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[21]) == 0x00 );  // sub-block ends

  // GifCommentExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[22]) == 0x21 );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[23]) == 0xFE );  // label
  auto size = static_cast<uint8_t>(str[24]);  // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[24+size+1]) == 0x00 );  // sub-block ends
}

void GifComponentVecUtilTest::testCreateOne()
{
  GifComponentVec cv = GifComponentVecUtil::Create( 2, 3, 3 );
  CPPUNIT_ASSERT( cv.size() == 1 );
  CPPUNIT_ASSERT( typeid( *cv[0] ) == typeid( GifImageDescriptor ) );
  CPPUNIT_ASSERT( cv[0]->ID() == 0x2C );
  CPPUNIT_ASSERT( cv[0]->Label() == 0x00 );
}

void GifComponentVecUtilTest::testCreateTwo()
{
  GifComponentVec cv = GifComponentVecUtil::Create( 2, 3, 3, 2 );
  CPPUNIT_ASSERT( cv.size() == 5 );

  CPPUNIT_ASSERT( typeid( *cv[0] ) == typeid( GifApplicationExt ) );
  CPPUNIT_ASSERT( cv[0]->ID() == 0x21 );
  CPPUNIT_ASSERT( cv[0]->Label() == 0xFF );

  CPPUNIT_ASSERT( typeid( *cv[1] ) == typeid( GifGraphicsControlExt ) );
  CPPUNIT_ASSERT( cv[1]->ID() == 0x21 );
  CPPUNIT_ASSERT( cv[1]->Label() == 0xF9 );

  CPPUNIT_ASSERT( typeid( *cv[2] ) == typeid( GifImageDescriptor ) );
  CPPUNIT_ASSERT( cv[2]->ID() == 0x2C );
  CPPUNIT_ASSERT( cv[2]->Label() == 0x00 );

  CPPUNIT_ASSERT( typeid( *cv[3] ) == typeid( GifGraphicsControlExt ) );
  CPPUNIT_ASSERT( cv[3]->ID() == 0x21 );
  CPPUNIT_ASSERT( cv[3]->Label() == 0xF9 );

  CPPUNIT_ASSERT( typeid( *cv[4] ) == typeid( GifImageDescriptor ) );
  CPPUNIT_ASSERT( cv[4]->ID() == 0x2C );
  CPPUNIT_ASSERT( cv[4]->Label() == 0x00 );
}
