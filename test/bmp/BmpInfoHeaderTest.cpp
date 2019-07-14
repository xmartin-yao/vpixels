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

// Unit tests for BmpInfoHeader

#include "BmpInfoHeaderTest.h"
#include "BmpInfoHeader.h"
#include "BmpInfo.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpInfoHeaderTest );

void BmpInfoHeaderTest::setUp()
{
  m_pInfo1Bit  = BmpInfo::Create( 1, 0, 0 ).release();
  m_pInfo4Bit  = BmpInfo::Create( 4, 10, 10 ).release();
  m_pInfo8Bit  = BmpInfo::Create( 8, 10, 10 ).release();
  m_pInfo24Bit = BmpInfo::Create( 24, 10, 10 ).release();
}

void BmpInfoHeaderTest::tearDown()
{
  delete m_pInfo1Bit;
  delete m_pInfo4Bit;
  delete m_pInfo8Bit;
  delete m_pInfo24Bit;
}

void BmpInfoHeaderTest::testCtors()
{
  // ctor
  BmpInfoHeader header1( *m_pInfo1Bit );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo1Bit ) );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo4Bit )  == false );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo8Bit )  == false );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo24Bit ) == false );

  BmpInfoHeader header2( *m_pInfo4Bit );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo1Bit )  == false );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo4Bit ) );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo8Bit )  == false );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo24Bit ) == false );

  BmpInfoHeader header3( *m_pInfo8Bit );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo1Bit )  == false );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo4Bit )  == false );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo8Bit ) );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo24Bit ) == false );

  BmpInfoHeader header4( *m_pInfo24Bit );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo1Bit ) == false );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo4Bit ) == false );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo8Bit ) == false );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo24Bit ) );

  // copy ctor
  BmpInfoHeader header5 = header2;
  CPPUNIT_ASSERT( header5.Check( *m_pInfo1Bit )  == false );
  CPPUNIT_ASSERT( header5.Check( *m_pInfo4Bit ) );
  CPPUNIT_ASSERT( header5.Check( *m_pInfo8Bit )  == false );
  CPPUNIT_ASSERT( header5.Check( *m_pInfo24Bit ) == false );
}

void BmpInfoHeaderTest::testAttributes()
{
  BmpInfoHeader header1( *m_pInfo1Bit );
  CPPUNIT_ASSERT( header1.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( header1.Width() == 0 );
  CPPUNIT_ASSERT( header1.Height() == 0 );
  CPPUNIT_ASSERT( header1.ImageDataSize() == 0 );

  BmpInfoHeader header2( *m_pInfo4Bit );
  CPPUNIT_ASSERT( header2.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( header2.Width() == 10 );
  CPPUNIT_ASSERT( header2.Height() == 10 );
  CPPUNIT_ASSERT( header2.ImageDataSize() == 8*10 );

  BmpInfoHeader header3( *m_pInfo8Bit );
  CPPUNIT_ASSERT( header3.BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( header3.Width() == 10 );
  CPPUNIT_ASSERT( header3.Height() == 10 );
  CPPUNIT_ASSERT( header3.ImageDataSize() == 12*10 );

  BmpInfoHeader header4( *m_pInfo24Bit );
  CPPUNIT_ASSERT( header4.BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( header4.Width() == 10 );
  CPPUNIT_ASSERT( header4.Height() == 10 );
  CPPUNIT_ASSERT( header4.ImageDataSize() == 32*10 );
}

void BmpInfoHeaderTest::testIO()
{
  std::stringstream strstream;
  BmpInfoHeader header1( *m_pInfo4Bit );
  strstream << header1;

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 40 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x28 );  // size: 40
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0x00 );  
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0x0A );  // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0x0A );  // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[9]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[10]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[11]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[12]) == 0x01 ); // planes: 1
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[13]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[14]) == 0x04 ); // bits/pixel
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[15]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[16]) == 0x00 ); // compression: 0
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[17]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[18]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[19]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[20]) == 0x50 ); // image data size: 80
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[21]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[22]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[23]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[24]) == 0xC4 ); // x resolution: 3780
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[25]) == 0x0E );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[26]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[27]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[28]) == 0xC4 ); // y resolution: 3780
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[29]) == 0x0E );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[30]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[31]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[32]) == 0x00 ); // used colors: 0 
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[33]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[34]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[35]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[36]) == 0x00 ); // important colors: 0
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[37]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[38]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[39]) == 0x00 );

  BmpInfoHeader header2( *m_pInfo4Bit );
  strstream >> header2;
  CPPUNIT_ASSERT( header2.Check( *m_pInfo4Bit ) );
  CPPUNIT_ASSERT( header2.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( header2.Width() == 10 );
  CPPUNIT_ASSERT( header2.Height() == 10 );
  CPPUNIT_ASSERT( header2.ImageDataSize() == 80 );
}
