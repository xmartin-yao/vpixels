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

// Unit tests for BmpFileHeader

#include "BmpFileHeaderTest.h"
#include "BmpFileHeader.h"
#include "BmpInfo.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpFileHeaderTest );

void BmpFileHeaderTest::setUp()
{
  m_pInfo1Bit  = BmpInfo::Create( 1, 0, 0 ).release();
  m_pInfo4Bit  = BmpInfo::Create( 4, 10, 10 ).release();
  m_pInfo8Bit  = BmpInfo::Create( 8, 10, 10 ).release();
  m_pInfo24Bit = BmpInfo::Create( 24, 10, 10 ).release();
}

void BmpFileHeaderTest::tearDown()
{
  delete m_pInfo1Bit;
  delete m_pInfo4Bit;
  delete m_pInfo8Bit;
  delete m_pInfo24Bit;
}

void BmpFileHeaderTest::testCtors()
{
  // ctor
  BmpFileHeader header1( *m_pInfo1Bit );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo1Bit ) );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo4Bit )  == false );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo8Bit )  == false );
  CPPUNIT_ASSERT( header1.Check( *m_pInfo24Bit ) == false );

  BmpFileHeader header2( *m_pInfo4Bit );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo1Bit )  == false );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo4Bit ) );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo8Bit )  == false );
  CPPUNIT_ASSERT( header2.Check( *m_pInfo24Bit ) == false );

  BmpFileHeader header3( *m_pInfo8Bit );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo1Bit )  == false );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo4Bit )  == false );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo8Bit ) );
  CPPUNIT_ASSERT( header3.Check( *m_pInfo24Bit ) == false );

  BmpFileHeader header4( *m_pInfo24Bit );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo1Bit ) == false );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo4Bit ) == false );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo8Bit ) == false );
  CPPUNIT_ASSERT( header4.Check( *m_pInfo24Bit ) );

  // copy ctor
  BmpFileHeader header5 = header2;
  CPPUNIT_ASSERT( header5.Check( *m_pInfo1Bit )  == false );
  CPPUNIT_ASSERT( header5.Check( *m_pInfo4Bit ) );
  CPPUNIT_ASSERT( header5.Check( *m_pInfo8Bit )  == false );
  CPPUNIT_ASSERT( header5.Check( *m_pInfo24Bit ) == false );
}

// file size = file header + info header + color table + image data
// image data offset = file header + info header + color table
// file header + info header = 14 + 40 = 54 bytes
void BmpFileHeaderTest::testAttributes()
{
  BmpFileHeader header1( *m_pInfo1Bit );
  CPPUNIT_ASSERT( header1.FileSize() == 54 + 4*2 + 0 );
  CPPUNIT_ASSERT( header1.ImageOffset() == 54 + 4*2 );

  BmpFileHeader header2( *m_pInfo4Bit );
  CPPUNIT_ASSERT( header2.FileSize() == 54 + 4*16 + 8*10 );
  CPPUNIT_ASSERT( header2.ImageOffset() == 54 + 4*16 );

  BmpFileHeader header3( *m_pInfo8Bit );
  CPPUNIT_ASSERT( header3.FileSize() == 54 + 4*256 + 12*10 );
  CPPUNIT_ASSERT( header3.ImageOffset() == 54 + 4*256 );

  BmpFileHeader header4( *m_pInfo24Bit );
  CPPUNIT_ASSERT( header4.FileSize() == 54 + 0 + 32*10 );
  CPPUNIT_ASSERT( header4.ImageOffset() == 54 );
}

void BmpFileHeaderTest::testIO()
{
  std::stringstream strstream;
  BmpFileHeader header1( *m_pInfo8Bit );
  strstream << header1;

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 14 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x42 );  // Id: 'BM'
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x4D );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0xAE );  // file size: 1198
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0x04 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0x00 );  // reserved 1
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0x00 );  // reserved 2
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[9]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[10]) == 0x36 ); // Offset to image data
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[11]) == 0x04 ); //  1078
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[12]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[13]) == 0x00 );

  BmpFileHeader header2( *m_pInfo1Bit );
  strstream >> header2;
  CPPUNIT_ASSERT( header2.Check( *m_pInfo8Bit ) );
  CPPUNIT_ASSERT( header2.FileSize() == 54 + 4*256 + 12*10 );
  CPPUNIT_ASSERT( header2.ImageOffset() == 54 + 4*256 );
}
