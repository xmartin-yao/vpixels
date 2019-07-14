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

#include "GifHeaderTest.h"
#include "GifHeader.h"
#include "U8String.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifHeaderTest );

void GifHeaderTest::testCtor()
{
  // ctor
  GifHeader h2( GifVersion::V87a );
  CPPUNIT_ASSERT( h2 );
  CPPUNIT_ASSERT( h2.Version() == GifVersion::V87a );

  // ctor
  GifHeader h3( GifVersion::V89a );
  CPPUNIT_ASSERT( h3 );
  CPPUNIT_ASSERT( h3.Version() == GifVersion::V89a );

  // copy ctor
  GifHeader h4( h3 );
  CPPUNIT_ASSERT( h4 );
  CPPUNIT_ASSERT( h4.Version() == GifVersion::V89a );
}

void GifHeaderTest::testIO()
{
  std::string GIF89a("GIF89a");
  std::string GIF87a("GIF87a");;

  GifHeader header( GifVersion::V89a ); 
  std::stringstream stream;

  // output
  stream << header;
  std::string actual = stream.str();

  CPPUNIT_ASSERT( stream.str() == GIF89a );

  // prepare input stream
  stream.clear();
  stream.str(std::string());
  stream << GIF87a;
  CPPUNIT_ASSERT( stream.str() == GIF87a );

  // input
  stream >> header;
  CPPUNIT_ASSERT( header );
  CPPUNIT_ASSERT( header.Version() == GifVersion::V87a );
}
