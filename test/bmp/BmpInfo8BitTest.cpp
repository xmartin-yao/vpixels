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
// Unit tests for BmpInfo8Bit

#include "BmpInfo8BitTest.h"
#include "BmpInfo8Bit.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpInfo8BitTest );


/////////////////////////
void BmpInfo8BitTest::setUp()
{
  m_p1x1 = new BmpInfo8Bit( 1, 1 );
  m_p1x2 = new BmpInfo8Bit( 1, 2 );
  m_p2x2 = new BmpInfo8Bit( 2, 2 );
  m_p3x2 = new BmpInfo8Bit( 3, 2 );
  m_p4x2 = new BmpInfo8Bit( 4, 2 );

  m_p10x10 = new BmpInfo8Bit( 10, 10 );
  m_p10x20 = new BmpInfo8Bit( 10, 20 );
  m_p20x20 = new BmpInfo8Bit( 20, 20 );
  m_p30x20 = new BmpInfo8Bit( 30, 20 );
  m_p40x20 = new BmpInfo8Bit( 40, 20 );
}

/////////////////////////
void BmpInfo8BitTest::tearDown()
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
void BmpInfo8BitTest::testBitsPerPixel()
{
  CPPUNIT_ASSERT( m_p1x1->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p1x2->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p2x2->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p3x2->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p4x2->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p10x10->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p10x20->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p20x20->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p30x20->BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( m_p40x20->BitsPerPixel() == 8 );
}

////////////////////////////////
void BmpInfo8BitTest::testRowLength()
{
  CPPUNIT_ASSERT( m_p1x1->RowLength() == 1 );
  CPPUNIT_ASSERT( m_p1x2->RowLength() == 1 );
  CPPUNIT_ASSERT( m_p2x2->RowLength() == 2 );
  CPPUNIT_ASSERT( m_p3x2->RowLength() == 3 );
  CPPUNIT_ASSERT( m_p4x2->RowLength() == 4 );
  CPPUNIT_ASSERT( m_p10x10->RowLength() == 10 );
  CPPUNIT_ASSERT( m_p10x20->RowLength() == 10 );
  CPPUNIT_ASSERT( m_p20x20->RowLength() == 20 );
  CPPUNIT_ASSERT( m_p30x20->RowLength() == 30 );
  CPPUNIT_ASSERT( m_p40x20->RowLength() == 40 );
}

////////////////////////////////
void BmpInfo8BitTest::testByteArraySize()
{
  CPPUNIT_ASSERT( m_p1x1->ByteArraySize() == 1 );
  CPPUNIT_ASSERT( m_p1x2->ByteArraySize() == 2 );
  CPPUNIT_ASSERT( m_p2x2->ByteArraySize() == 4 );
  CPPUNIT_ASSERT( m_p3x2->ByteArraySize() == 6 );
  CPPUNIT_ASSERT( m_p4x2->ByteArraySize() == 8 );
  CPPUNIT_ASSERT( m_p10x10->ByteArraySize() == 100 );
  CPPUNIT_ASSERT( m_p10x20->ByteArraySize() == 200 );
  CPPUNIT_ASSERT( m_p20x20->ByteArraySize() == 400 );
  CPPUNIT_ASSERT( m_p30x20->ByteArraySize() == 600 );
  CPPUNIT_ASSERT( m_p40x20->ByteArraySize() == 800 );
}

////////////////////////////////
void BmpInfo8BitTest::testImageDataSize()
{
  CPPUNIT_ASSERT( m_p1x1->ImageDataSize() == 4 );
  CPPUNIT_ASSERT( m_p1x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p2x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p3x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p4x2->ImageDataSize() == 8 );
  CPPUNIT_ASSERT( m_p10x10->ImageDataSize() == 120 );
  CPPUNIT_ASSERT( m_p10x20->ImageDataSize() == 240 );
  CPPUNIT_ASSERT( m_p20x20->ImageDataSize() == 400 );
  CPPUNIT_ASSERT( m_p30x20->ImageDataSize() == 640 );
  CPPUNIT_ASSERT( m_p40x20->ImageDataSize() == 800 );
}

////////////////////////////////
void BmpInfo8BitTest::testByteArrayIndices()
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

  // 2x2 (2 byte x 2)
  m_p2x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 1, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 1 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 2 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p2x2->ByteArrayIndices( 1, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 3x2 (3 bytes x 2)
  m_p3x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 2, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 2 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p3x2->ByteArrayIndices( 2, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 5 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 4x2 (2 bytes x 2)
  m_p4x2->ByteArrayIndices( 0, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 3, 1, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 3 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 4 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p4x2->ByteArrayIndices( 3, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 7 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 10x10 (10 bytes x 10)
  m_p10x10->ByteArrayIndices( 0, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 9, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 9 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 4, 4, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 54 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 90 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x10->ByteArrayIndices( 9, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 99 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 10x20 (10 bytes x 20)
  m_p10x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 9, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 9 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 4, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 104 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 190 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p10x20->ByteArrayIndices( 9, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 199 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 20x20 (20 bytes x 20)
  m_p20x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 19, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 19 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 9, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 209 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 380 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p20x20->ByteArrayIndices( 19, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 399 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 30x20 (30 bytes x 20)
  m_p30x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 29, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 29 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 14, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 314 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 570 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p30x20->ByteArrayIndices( 29, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 599 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );

  // 40x20 (40 bytes x 20)
  m_p40x20->ByteArrayIndices( 0, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 0 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 39, 19, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 39 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 19, 9, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 419 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 0, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 760 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
  m_p40x20->ByteArrayIndices( 39, 0, ByteIndex, BitIndex );
  CPPUNIT_ASSERT( 799 == ByteIndex );
  CPPUNIT_ASSERT( 0 == BitIndex );
}

//////////////////////////////////////
void BmpInfo8BitTest::testColorTableSize()
{
  CPPUNIT_ASSERT( m_p1x1->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p1x2->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p2x2->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p3x2->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p4x2->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p10x10->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p10x20->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p20x20->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p30x20->ColorTableSize() == 256 );
  CPPUNIT_ASSERT( m_p40x20->ColorTableSize() == 256 );
}

// GetColorIndex() is irrelevant to dimension,
// test it using on instance
///////////////////////////////////////////
void BmpInfo8BitTest::testGetColorIndex()
{
  uint8_t Byte = 0xE9;
  CPPUNIT_ASSERT( 0xE9 == m_p1x1->GetColorIndex( Byte, 0 ) );

  Byte = 0x9E;
  CPPUNIT_ASSERT( 0x9E == m_p1x1->GetColorIndex( Byte, 0) );

  Byte = 0x00;
  CPPUNIT_ASSERT( 0 == m_p1x1->GetColorIndex( Byte, 0 ) );

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
void BmpInfo8BitTest::testSetColorIndex()
{
  uint8_t Byte = 0;
  m_p1x1->SetColorIndex( Byte, 0, 255 );
  CPPUNIT_ASSERT( Byte == 255 );

  m_p1x1->SetColorIndex( Byte, 0, 128 );
  CPPUNIT_ASSERT( Byte == 128 );

  m_p1x1->SetColorIndex( Byte, 0, 0 );
  CPPUNIT_ASSERT( Byte == 0 );

  // type conversion: 256 -> 0
  uint16_t Index = 256;
  m_p1x1->SetColorIndex( Byte, 0, Index );
  CPPUNIT_ASSERT( Byte == 0 );

  // type conversion: 257 -> 1
  Index = 257;
  m_p1x1->SetColorIndex( Byte, 0, Index );
  CPPUNIT_ASSERT( Byte == 1 );

  // type conversion: -1 -> 255
  m_p1x1->SetColorIndex( Byte, 0, -1 );
  CPPUNIT_ASSERT( Byte == 255 );

  // type conversion: -2 -> 254
  m_p1x1->SetColorIndex( Byte, 0, -2 );
  CPPUNIT_ASSERT( Byte == 254 );

  // incorrect bit index
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, -1, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 1, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 2, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 3, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 5, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 6, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 7, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( m_p1x1->SetColorIndex( Byte, 8, 0 ), vp::Exception );
}
