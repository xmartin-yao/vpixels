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
#include "GifApplicationExtTest.h"
#include "GifApplicationExt.h"


CPPUNIT_TEST_SUITE_REGISTRATION( GifApplicationExtTest );

void GifApplicationExtTest::testCtors()
{
  // ctor
  GifApplicationExt ae1;
  CPPUNIT_ASSERT( ae1.ID() == 0x21 );
  CPPUNIT_ASSERT( ae1.Label() == 0xFF );
  CPPUNIT_ASSERT( ae1.AppID() == "NETSCAPE" );
  CPPUNIT_ASSERT( ae1.AuthCode() == "2.0" );
  CPPUNIT_ASSERT( ae1.LoopTimes() == 0 );
  CPPUNIT_ASSERT( ae1.LoopTimes( 5 ) );
  CPPUNIT_ASSERT( ae1.LoopTimes() == 5 );

  // copy ctor
  GifApplicationExt ae2 = ae1;
  CPPUNIT_ASSERT( ae2.ID() == 0x21 );
  CPPUNIT_ASSERT( ae2.Label() == 0xFF );
  CPPUNIT_ASSERT( ae2.AppID() == "NETSCAPE" );
  CPPUNIT_ASSERT( ae2.AuthCode() == "2.0" );
  CPPUNIT_ASSERT( ae2.LoopTimes() == 5 );
}

void GifApplicationExtTest::testOutput()
{
  GifApplicationExt ae;
  ae.LoopTimes( 500 );

  std::stringstream stream;
  ae.Write( stream );
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 19 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x21 );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xFF );  // label
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0x0B );  // app block size
  CPPUNIT_ASSERT( str.substr( 3, 8 ) == "NETSCAPE" );      // app ID
  CPPUNIT_ASSERT( str.substr( 11, 3 ) == "2.0" );          // auth code
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[14]) == 0x03 ); // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[15]) == 0x01 ); // 1st byte
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[16]) == 0xF4 ); // loop times
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[17]) == 0x01 ); 
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[18]) == 0x00 ); // terminator
}

void GifApplicationExtTest::testInput()
{
  std::stringstream stream;
  stream << static_cast<uint8_t>(0x2C);  // ID
  stream << static_cast<uint8_t>(0xFF);  // label
  stream << static_cast<uint8_t>(0x0B);  // app block size 11
  stream << "NETSCAPE";                  // app ID
  stream << "2.0";                       // auth code
  stream << static_cast<uint8_t>(0x03);  // sub-block size
  stream << static_cast<uint8_t>(0x01);  // 1st byte
  stream << static_cast<uint8_t>(0x0A);  // loop times 
  stream << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00);  // terminator
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 19 );

  GifApplicationExt ae;
  CPPUNIT_ASSERT( stream.get() == 0x2C );  // ID
  CPPUNIT_ASSERT( stream.get() == 0xFF );  // label
  ae.Read( stream );
  CPPUNIT_ASSERT( ae.AppID() == "NETSCAPE" );
  CPPUNIT_ASSERT( ae.AuthCode() == "2.0" );
  CPPUNIT_ASSERT( ae.LoopTimes() == 10 );
}
