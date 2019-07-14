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
// Unit tests for BmpInfo24Bit

#include "BmpInfo24BitTest.h"
#include "BmpInfo24Bit.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpInfo24BitTest );


/////////////////////////
void BmpInfo24BitTest::setUp()
{
  m_p1x1 = new BmpInfo24Bit( 1, 1 );
  m_p1x2 = new BmpInfo24Bit( 1, 2 );
  m_p2x2 = new BmpInfo24Bit( 2, 2 );
  m_p3x2 = new BmpInfo24Bit( 3, 2 );
  m_p4x2 = new BmpInfo24Bit( 4, 2 );

  m_p10x10 = new BmpInfo24Bit( 10, 10 );
  m_p10x20 = new BmpInfo24Bit( 10, 20 );
  m_p20x20 = new BmpInfo24Bit( 20, 20 );
  m_p30x20 = new BmpInfo24Bit( 30, 20 );
  m_p40x20 = new BmpInfo24Bit( 40, 20 );
}

/////////////////////////
void BmpInfo24BitTest::tearDown()
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
void BmpInfo24BitTest::testBitsPerPixel()
{
  CPPUNIT_ASSERT( m_p1x1->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p1x2->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p2x2->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p3x2->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p4x2->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p10x10->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p10x20->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p20x20->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p30x20->BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( m_p40x20->BitsPerPixel() == 24 );
}

////////////////////////////////
void BmpInfo24BitTest::testRowLength()
{
  CPPUNIT_ASSERT( m_p1x1->RowLength() == 3 );
  CPPUNIT_ASSERT( m_p1x2->RowLength() == 3 );
  CPPUNIT_ASSERT( m_p2x2->RowLength() == 6 );
  CPPUNIT_ASSERT( m_p3x2->RowLength() == 9 );
  CPPUNIT_ASSERT( m_p4x2->RowLength() == 12 );
  CPPUNIT_ASSERT( m_p10x10->RowLength() == 30 );
  CPPUNIT_ASSERT( m_p10x20->RowLength() == 30 );
  CPPUNIT_ASSERT( m_p20x20->RowLength() == 60 );
  CPPUNIT_ASSERT( m_p30x20->RowLength() == 90 );
  CPPUNIT_ASSERT( m_p40x20->RowLength() == 120 );
}

////////////////////////////////
void BmpInfo24BitTest::testByteArraySize()
{
  CPPUNIT_ASSERT( m_p1x1->ByteArraySize() == 3 );
  CPPUNIT_ASSERT( m_p1x2->ByteArraySize() == 6 );
  CPPUNIT_ASSERT( m_p2x2->ByteArraySize() == 12 );
  CPPUNIT_ASSERT( m_p3x2->ByteArraySize() == 18 );
  CPPUNIT_ASSERT( m_p4x2->ByteArraySize() == 24 );
  CPPUNIT_ASSERT( m_p10x10->ByteArraySize() == 300 );
  CPPUNIT_ASSERT( m_p10x20->ByteArraySize() == 600 );
  CPPUNIT_ASSERT( m_p20x20->ByteArraySize() == 1200 );
  CPPUNIT_ASSERT( m_p30x20->ByteArraySize() == 1800 );
  CPPUNIT_ASSERT( m_p40x20->ByteArraySize() == 2400 );
}

////////////////////////////////
void BmpInfo24BitTest::testImageDataSize()
{
  CPPUNIT_ASSERT( m_p1x1->ImageDataSize() == 4 );
  CPPUNIT_ASSERT( m_p1x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p2x2->ImageDataSize() == 16 );
  CPPUNIT_ASSERT( m_p3x2->ImageDataSize() == 24 );
  CPPUNIT_ASSERT( m_p4x2->ImageDataSize() == 24 );
  CPPUNIT_ASSERT( m_p10x10->ImageDataSize() == 320 );
  CPPUNIT_ASSERT( m_p10x20->ImageDataSize() == 640 );
  CPPUNIT_ASSERT( m_p20x20->ImageDataSize() == 1200 );
  CPPUNIT_ASSERT( m_p30x20->ImageDataSize() == 1840 );
  CPPUNIT_ASSERT( m_p40x20->ImageDataSize() == 2400 );
}

////////////////////////////////
void BmpInfo24BitTest::testByteArrayIndices()
{
  uint32_t ByteIndex;
  uint8_t BitIndex;

  // 1x1 (3 bytes)
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

  // 1x2 (3 bytes x 2)
  m_p1x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p1x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 2x2 (6 bytes x 2)
  m_p2x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 1, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 6 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 1, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 9 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 3x2 (9 bytes x 2)
  m_p3x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 2, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 6 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 9 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 2, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 15 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 4x2 (12 bytes x 2)
  m_p4x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 3, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 9 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 12 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 3, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 21 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 10x10 (30 bytes x 10)
  m_p10x10->ByteArrayIndices( 0, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 9, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 27 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 4, 4, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 162 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 270 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 9, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 297 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 10x20 (30 bytes x 20)
  m_p10x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 9, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 27 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 4, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 312 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 570 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 9, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 597 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 20x20 (60 bytes x 20)
  m_p20x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 19, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 57 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 9, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 627 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1140 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 19, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1197 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 30x20 (90 bytes x 20)
  m_p30x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 29, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 87 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 14, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 942 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1710 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 29, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1797 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 40x20 (120 bytes x 20)
  m_p40x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 39, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 117 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 19, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1257 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 2280 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 39, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 2397 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
}

//////////////////////////////////////
void BmpInfo24BitTest::testColorTableSize()
{
  CPPUNIT_ASSERT( m_p1x1->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p1x2->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p2x2->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p3x2->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p4x2->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p10x10->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p10x20->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p20x20->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p30x20->ColorTableSize() == 0 );
  CPPUNIT_ASSERT( m_p40x20->ColorTableSize() == 0 );
}

// GetColorIndex() is irrelevant to dimension,
// test it using on instance
///////////////////////////////////////////
void BmpInfo24BitTest::testGetColorIndex()
{
  uint8_t Byte = 0xE9;
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 2 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 3 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 5 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 6 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->GetColorIndex( Byte, 7 ), vp::Exception );
}

// SetColorIndex() is irrelevant to dimension,
// test it using one instance
//////////////////////////////////////
void BmpInfo24BitTest::testSetColorIndex()
{
  uint8_t Byte = 0;
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 0, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 1, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 2, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 3, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 5, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 6, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 7, 0 ), vp::Exception );
}
