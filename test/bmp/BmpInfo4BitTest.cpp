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
// Unit tests for BmpInfo4Bit

#include "BmpInfo4BitTest.h"
#include "BmpInfo4Bit.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpInfo4BitTest );


/////////////////////////
void BmpInfo4BitTest::setUp()
{
  m_p1x1 = new BmpInfo4Bit( 1, 1 );
  m_p1x2 = new BmpInfo4Bit( 1, 2 );
  m_p2x2 = new BmpInfo4Bit( 2, 2 );
  m_p3x2 = new BmpInfo4Bit( 3, 2 );
  m_p4x2 = new BmpInfo4Bit( 4, 2 );

  m_p10x10 = new BmpInfo4Bit( 10, 10 );
  m_p10x20 = new BmpInfo4Bit( 10, 20 );
  m_p20x20 = new BmpInfo4Bit( 20, 20 );
  m_p30x20 = new BmpInfo4Bit( 30, 20 );
  m_p40x20 = new BmpInfo4Bit( 40, 20 );
}

/////////////////////////
void BmpInfo4BitTest::tearDown()
{
  delete m_p1x1;
  delete m_p1x2;
  delete m_p2x2;
  delete m_p3x2;
  delete m_p4x2;
  delete m_p10x10;
  delete m_p10x20;
  delete m_p20x20;
  delete m_p30x20;
  delete m_p40x20;
}

//////////////////////////////////////
void BmpInfo4BitTest::testBitsPerPixel()
{
  CPPUNIT_ASSERT( m_p1x1->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p1x2->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p2x2->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p3x2->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p4x2->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p10x10->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p10x20->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p20x20->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p30x20->BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( m_p40x20->BitsPerPixel() == 4 );
}

////////////////////////////////
void BmpInfo4BitTest::testRowLength()
{
  CPPUNIT_ASSERT( m_p1x1->RowLength() == 1 );
  CPPUNIT_ASSERT( m_p1x2->RowLength() == 1 );
  CPPUNIT_ASSERT( m_p2x2->RowLength() == 1 );
  CPPUNIT_ASSERT( m_p3x2->RowLength() == 2 );
  CPPUNIT_ASSERT( m_p4x2->RowLength() == 2 );
  CPPUNIT_ASSERT( m_p10x10->RowLength() == 5 );
  CPPUNIT_ASSERT( m_p10x20->RowLength() == 5 );
  CPPUNIT_ASSERT( m_p20x20->RowLength() == 10 );
  CPPUNIT_ASSERT( m_p30x20->RowLength() == 15 );
  CPPUNIT_ASSERT( m_p40x20->RowLength() == 20 );
}

////////////////////////////////
void BmpInfo4BitTest::testByteArraySize()
{
  CPPUNIT_ASSERT( m_p1x1->ByteArraySize() == 1 );
  CPPUNIT_ASSERT( m_p1x2->ByteArraySize() == 2 );
  CPPUNIT_ASSERT( m_p2x2->ByteArraySize() == 2 );
  CPPUNIT_ASSERT( m_p3x2->ByteArraySize() == 4 );
  CPPUNIT_ASSERT( m_p4x2->ByteArraySize() == 4 );
  CPPUNIT_ASSERT( m_p10x10->ByteArraySize() == 50 );
  CPPUNIT_ASSERT( m_p10x20->ByteArraySize() == 100 );
  CPPUNIT_ASSERT( m_p20x20->ByteArraySize() == 200 );
  CPPUNIT_ASSERT( m_p30x20->ByteArraySize() == 300 );
  CPPUNIT_ASSERT( m_p40x20->ByteArraySize() == 400 );
}

////////////////////////////////
void BmpInfo4BitTest::testImageDataSize()
{
  CPPUNIT_ASSERT( m_p1x1->ImageDataSize() == 4 );
  CPPUNIT_ASSERT( m_p1x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p2x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p3x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p4x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p10x10->ImageDataSize() == 80 );
  CPPUNIT_ASSERT( m_p10x20->ImageDataSize() == 160 );
  CPPUNIT_ASSERT( m_p20x20->ImageDataSize() == 240 );
  CPPUNIT_ASSERT( m_p30x20->ImageDataSize() == 320 );
  CPPUNIT_ASSERT( m_p40x20->ImageDataSize() == 400 );
}

////////////////////////////////
void BmpInfo4BitTest::testByteArrayIndices()
{
  uint32_t ByteIndex;
  uint8_t BitIndex;

  // 1x1 (1 byte)
  m_p1x1->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  CPPUNIT_ASSERT_THROW( m_p1x1->ByteArrayIndices( 1, 0, ByteIndex, BitIndex ),
                        vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->ByteArrayIndices( -1, 0, ByteIndex, BitIndex ),
                        vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->ByteArrayIndices( 0, 1, ByteIndex, BitIndex ),
                        vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->ByteArrayIndices( 0, -1, ByteIndex, BitIndex ),
                        vp::Exception );
  // 1x2 (1 byte x 2)
  m_p1x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p1x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 2x2 (1 byte x 2)
  m_p2x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 1, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p2x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 1, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );

  // 3x2 (2 bytes x 2)
  m_p3x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 2, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 2 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 2, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 4x2 (2 bytes x 2)
  m_p4x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 3, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p4x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 2 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 3, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );

  // 10x10 (5 bytes x 10)
  m_p10x10->ByteArrayIndices( 0, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 9, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 4 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p10x10->ByteArrayIndices( 4, 4, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 27 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 45 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 9, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 49 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );

  // 10x20 (5 bytes x 20)
  m_p10x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 9, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 4 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p10x20->ByteArrayIndices( 4, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 52 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 95 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 9, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 99 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );

  // 20x20 (10 bytes x 20)
  m_p20x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 19, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 9 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p20x20->ByteArrayIndices( 9, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 104 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p20x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 190 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 19, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 199 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );

  // 30x20 (15 bytes x 20)
  m_p30x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 29, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 14 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p30x20->ByteArrayIndices( 14, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 157 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 285 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 29, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 299 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );

  // 40x20 (20 bytes x 20)
  m_p40x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 39, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 19 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p40x20->ByteArrayIndices( 19, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 209 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
  m_p40x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 380 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 39, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 399 == ByteIndex );
  CPPUNIT_ASSERT( 4 == BitIndex );
}

//////////////////////////////////////
void BmpInfo4BitTest::testColorTableSize()
{
  CPPUNIT_ASSERT( m_p1x1->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p1x2->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p2x2->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p3x2->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p4x2->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p10x10->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p10x20->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p20x20->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p30x20->ColorTableSize() == 16 );
  CPPUNIT_ASSERT( m_p40x20->ColorTableSize() == 16 );
}

// GetColorIndex() is irrelevant to dimension,
// test it using on instance
///////////////////////////////////////////
void BmpInfo4BitTest::testGetColorIndex()
{
  uint8_t Byte = 0xE9;
  CPPUNIT_ASSERT( 0x0E == m_p1x1->GetColorIndex( Byte, 0 ) );
  CPPUNIT_ASSERT( 0x09 == m_p1x1->GetColorIndex( Byte, 4 ) );

  Byte = 0x9E;
  CPPUNIT_ASSERT( 0x09 == m_p1x1->GetColorIndex( Byte, 0 ) );
  CPPUNIT_ASSERT( 0x0E == m_p1x1->GetColorIndex( Byte, 4 ) );

  Byte = 0x00;
  CPPUNIT_ASSERT( 0 == m_p1x1->GetColorIndex( Byte, 0 ) );
  CPPUNIT_ASSERT( 0 == m_p1x1->GetColorIndex( Byte, 4 ) );

  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 2 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 3 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 5 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 6 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 7 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 8 ), vp::Exception );
}

// SetColorIndex() is irrelevant to dimension,
// test it using on instance
//////////////////////////////////////
void BmpInfo4BitTest::testSetColorIndex()
{
  uint8_t Byte = 0xFF;
  m_p1x1->SetColorIndex( Byte, 0, 6 );
  CPPUNIT_ASSERT( Byte == 0x6F );

  m_p1x1->SetColorIndex( Byte, 4, 9 );
  CPPUNIT_ASSERT( Byte == 0x69 );

  m_p1x1->SetColorIndex( Byte, 0, 0 );
  CPPUNIT_ASSERT( Byte == 0x09 );

  m_p1x1->SetColorIndex( Byte, 4, 0 );
  CPPUNIT_ASSERT( Byte == 0x00 );

  // incorrect bit index
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, -1, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 1, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 2, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 3, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 5, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 6, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 7, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 8, 0 ), vp::Exception );

  // incorrect color index
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 0, -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 0, 16 ), vp::Exception );
}
