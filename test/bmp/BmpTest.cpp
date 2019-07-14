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

#include "BmpTest.h"
#include "Bmp.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpTest );

void BmpTest::testCtors()
{
  uint8_t B, G, R;

  // ctor with default values
  vp::Bmp bmp1;
  CPPUNIT_ASSERT( bmp1 );
  CPPUNIT_ASSERT( bmp1.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( bmp1.Width() == 1 );
  CPPUNIT_ASSERT( bmp1.Height() == 1 );
  // one pixel
  bmp1.SetPixel( 0, 0, 1 );
  CPPUNIT_ASSERT( bmp1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT_THROW( bmp1.SetPixel( 0, 0, 23, 24, 25 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( bmp1.GetPixel( 0, 0, B, G, R ), vp::Exception );
  // color table
  CPPUNIT_ASSERT( bmp1.ColorTableSize() == 2 );
  bmp1.SetColorTable( 0, 23, 24, 25 );
  bmp1.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  // ctor
  vp::Bmp bmp2{ 4, 10, 10 };
  CPPUNIT_ASSERT( bmp2 );
  CPPUNIT_ASSERT( bmp2.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( bmp2.Width() == 10 );
  CPPUNIT_ASSERT( bmp2.Height() == 10 );
  CPPUNIT_ASSERT( bmp2.ColorTableSize() == 16 );

  // copy ctor
  vp::Bmp bmp3{bmp1};
  CPPUNIT_ASSERT( bmp1 );
  CPPUNIT_ASSERT( bmp3 );
  CPPUNIT_ASSERT( bmp3.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( bmp3.Width() == 1 );
  CPPUNIT_ASSERT( bmp3.Height() == 1 );
  CPPUNIT_ASSERT( bmp3.ColorTableSize() == 2 );

  // copy assign
  bmp1 = bmp2;
  CPPUNIT_ASSERT( bmp1 );
  CPPUNIT_ASSERT( bmp2 );
  CPPUNIT_ASSERT( bmp1.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( bmp1.Width() == 10 );
  CPPUNIT_ASSERT( bmp1.Height() == 10 );
  CPPUNIT_ASSERT( bmp1.ColorTableSize() == 16 );

  // move ctor
  vp::Bmp bmp4{std::move(bmp2)};
  CPPUNIT_ASSERT( !bmp2 ); // bmp2 moved
  CPPUNIT_ASSERT( bmp4 );
  CPPUNIT_ASSERT( bmp4.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( bmp4.Width() == 10 );
  CPPUNIT_ASSERT( bmp4.Height() == 10 );
  CPPUNIT_ASSERT( bmp4.ColorTableSize() == 16 );

  // move assign
  bmp2 = std::move(bmp3);
  CPPUNIT_ASSERT( !bmp3 ); // bmp3 moved
  CPPUNIT_ASSERT( bmp2 );
  CPPUNIT_ASSERT( bmp2.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( bmp2.Width() == 1 );
  CPPUNIT_ASSERT( bmp2.Height() == 1 );
  CPPUNIT_ASSERT( bmp2.ColorTableSize() == 2 );

  // wrong bpp
  try {
    vp::Bmp bmp( 2, 3, 4 );
    CPPUNIT_ASSERT_MESSAGE( "Expected exception didn't throw", false );
  }
  catch( const vp::Exception& ) {}
}

void BmpTest::test1Bit()
{
  uint8_t B, G, R;
  vp::Bmp bmp( 1, 5, 5 );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( bmp.Width() == 5 );
  CPPUNIT_ASSERT( bmp.Height() == 5 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 2 );

  // pixels
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 0 ); // 0 by default 
  }

  bmp.SetPixel( 0, 0, 1 );
  CPPUNIT_ASSERT( bmp.GetPixel( 0, 0 ) == 1 );
  bmp.SetPixel( 4, 4, 1 );
  CPPUNIT_ASSERT( bmp.GetPixel( 4, 4 ) == 1 );

  bmp.SetAllPixels( 1 );
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 1 );
  }

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( -1, 0, 1 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 5, 1 ), vp::Exception ); // y exceeds
  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 0, 2 ), vp::Exception );  // color index exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( -1, 0 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 5 ), vp::Exception ); // y exceeds

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 0, 3, 4, 5 ), vp::Exception ); // not 24-bit
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 0, B, G, R ), vp::Exception ); // not 24-bit

  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( 2 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( 3, 4, 5 ), vp::Exception ); // not 24-bit

  // color table
  for( uint8_t i = 0; i < bmp.ColorTableSize(); ++i )
  {
    bmp.GetColorTable( i, B, G, R );
    CPPUNIT_ASSERT( B == 0 );
    CPPUNIT_ASSERT( G == 0 );
    CPPUNIT_ASSERT( R == 0 );
  }

  bmp.SetColorTable( 1, 23, 24, 25 );
  bmp.GetColorTable( 1, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  CPPUNIT_ASSERT_THROW( bmp.SetColorTable( -1, 23, 24, 25 ), vp::Exception ); // color index
  CPPUNIT_ASSERT_THROW( bmp.SetColorTable( 2, 23, 24, 25 ), vp::Exception );  // color index
  CPPUNIT_ASSERT_THROW( bmp.GetColorTable( -1, B, G, R ), vp::Exception ); // color index
  CPPUNIT_ASSERT_THROW( bmp.GetColorTable( 2, B, G, R ), vp::Exception );  // color index
}

void BmpTest::test4Bits()
{
  uint8_t B, G, R;
  vp::Bmp bmp( 4, 8, 8 );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( bmp.Width() == 8 );
  CPPUNIT_ASSERT( bmp.Height() == 8 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 16 );

  // pixels
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 0 ); // 0 by default 
  }

  bmp.SetPixel( 0, 0, 15 );
  CPPUNIT_ASSERT( bmp.GetPixel( 0, 0 ) == 15 );
  bmp.SetPixel( 7, 7, 15 );
  CPPUNIT_ASSERT( bmp.GetPixel( 7, 7 ) == 15 );

  bmp.SetAllPixels( 5 );
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 5 );
  }

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( -1, 0, 1 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 8, 1 ), vp::Exception ); // y exceeds
  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 0, 16 ), vp::Exception );  // color index exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( -1, 0 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 8 ), vp::Exception ); // y exceeds

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 0, 3, 4, 5 ), vp::Exception ); // not 24-bit
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 0, B, G, R ), vp::Exception ); // not 24-bit

  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( 16 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( 3, 4, 5 ), vp::Exception ); // not 24-bit

  // color table
  for( uint8_t i = 0; i < bmp.ColorTableSize(); ++i )
  {
    bmp.GetColorTable( i, B, G, R );
    CPPUNIT_ASSERT( B == 0 );
    CPPUNIT_ASSERT( G == 0 );
    CPPUNIT_ASSERT( R == 0 );
  }

  bmp.SetColorTable( 15, 23, 24, 25 );
  bmp.GetColorTable( 15, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  CPPUNIT_ASSERT_THROW( bmp.SetColorTable( -1, 23, 24, 25 ), vp::Exception ); // color index
  CPPUNIT_ASSERT_THROW( bmp.SetColorTable( 16, 23, 24, 25 ), vp::Exception );  // color index
  CPPUNIT_ASSERT_THROW( bmp.GetColorTable( -1, B, G, R ), vp::Exception ); // color index
  CPPUNIT_ASSERT_THROW( bmp.GetColorTable( 16, B, G, R ), vp::Exception );  // color index
}

void BmpTest::test8Bits()
{
  uint8_t B, G, R;
  vp::Bmp bmp( 8, 12, 12 );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( bmp.Width() == 12 );
  CPPUNIT_ASSERT( bmp.Height() == 12 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 256 );

  // pixels
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 0 ); // 0 by default 
  }

  bmp.SetPixel( 0, 0, 255 );
  CPPUNIT_ASSERT( bmp.GetPixel( 0, 0 ) == 255 );
  bmp.SetPixel( 11, 11, 255 );
  CPPUNIT_ASSERT( bmp.GetPixel( 11, 11 ) == 255 );

  bmp.SetAllPixels( 5 );
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 5 );
  }

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( -1, 0, 1 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 12, 1 ), vp::Exception ); // y exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( -1, 0 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 12 ), vp::Exception ); // y exceeds

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 0, 3, 4, 5 ), vp::Exception ); // not 24-bit
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 0, B, G, R ), vp::Exception ); // not 24-bit
  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( 3, 4, 5 ), vp::Exception ); // not 24-bit

  // color table
  for( uint16_t i = 0; i < bmp.ColorTableSize(); ++i )
  {
    bmp.GetColorTable( i, B, G, R );
    CPPUNIT_ASSERT( B == 0 );
    CPPUNIT_ASSERT( G == 0 );
    CPPUNIT_ASSERT( R == 0 );
  }

  bmp.SetColorTable( 255, 23, 24, 25 );
  bmp.GetColorTable( 255, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  // caveat: -1 implicitly converted to 255
  bmp.SetColorTable( -1, 203, 204, 205 );
  bmp.GetColorTable( 255, B, G, R );
  CPPUNIT_ASSERT( B == 203 );
  CPPUNIT_ASSERT( G == 204 );
  CPPUNIT_ASSERT( R == 205 );

  // caveat: uint16_t 256 implicitly converted to 0
  bmp.SetColorTable( bmp.ColorTableSize(), 23, 24, 25 );
  bmp.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );
}

void BmpTest::test24Bits()
{
  uint8_t B, G, R;
  vp::Bmp bmp( 24, 12, 12 );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( bmp.Width() == 12 );
  CPPUNIT_ASSERT( bmp.Height() == 12 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 0 );

  // pixels
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
    {
      bmp.GetPixel( x, y, B, G, R );
      CPPUNIT_ASSERT( B == 0 ); // 0 by default
      CPPUNIT_ASSERT( G == 0 );
      CPPUNIT_ASSERT( R == 0 );
    }
  }

  bmp.SetPixel( 0, 0, 23, 24, 25 );
  bmp.GetPixel( 0, 0, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  bmp.SetPixel( 11, 11, 203, 204, 205 );
  bmp.GetPixel( 11, 11, B, G, R );
  CPPUNIT_ASSERT( B == 203 );
  CPPUNIT_ASSERT( G == 204 );
  CPPUNIT_ASSERT( R == 205 );

  bmp.SetAllPixels( 50, 51, 52 );
  for( uint8_t y = 0; y < bmp.Width(); ++y )
  {
    for( uint8_t x = 0; x < bmp.Width(); ++x )
    {
      bmp.GetPixel( x, y, B, G, R );
      CPPUNIT_ASSERT( B == 50 );
      CPPUNIT_ASSERT( G == 51 );
      CPPUNIT_ASSERT( R == 52 );
    }
  }

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( -1, 0, 23, 24, 25 ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 12, 23, 24, 25 ), vp::Exception ); // y exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( -1, 0, B, G, R ), vp::Exception ); // x exceeds
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 12, B, G, R ), vp::Exception ); // y exceeds

  CPPUNIT_ASSERT_THROW( bmp.SetPixel( 0, 0, 255 ), vp::Exception ); // not indexed
  CPPUNIT_ASSERT_THROW( bmp.GetPixel( 0, 0 ), vp::Exception );     // not indexed

  CPPUNIT_ASSERT_THROW( bmp.SetAllPixels( 255 ), vp::Exception ); // not indexed

  // no color table
  CPPUNIT_ASSERT_THROW( bmp.SetColorTable( 0, 23, 24, 25 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( bmp.GetColorTable( 0, B, G, R ), vp::Exception );
}
#if 0
void BmpTest::testRead1Bit()
{
  std::stringstream stream;

  // FileHeader
  stream << "BM" // Id
         << static_cast<uint8_t>(0x66) << static_cast<uint8_t>(0x00) // file size: 102
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) //  = 54 + 4*2 + 4*10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x3E) << static_cast<uint8_t>(0x00) // offset: 62
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);//  = 54 + 4*2

  // InfoHeader
  stream << static_cast<uint8_t>(0x28) << static_cast<uint8_t>(0x00) // header size: 40
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // width: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // height: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x01) << static_cast<uint8_t>(0x00) // planes: 1
         << static_cast<uint8_t>(0x01) << static_cast<uint8_t>(0x00) // bpp: 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // compression: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x28) << static_cast<uint8_t>(0x00) // image data siz: 40
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // x resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // y resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // colors used: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // important colors: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // color table
  for( uint8_t i = 0; i < 2; ++i )
  {
    stream << static_cast<uint8_t>(i+2) 
           << static_cast<uint8_t>(i+4)
           << static_cast<uint8_t>(i+6)
           << static_cast<uint8_t>(0);
  }

  // image data
  for( uint8_t i = 0; i < 40; ++i )
  {
    uint8_t j = i%4;
    if( j < 2 )
      stream << static_cast<uint8_t>(0xFF);  // pixel bytes
    else 
      stream << static_cast<uint8_t>(0x00);  // padding bytes
  }

  // read stream
  vp::Bmp bmp;
  bmp.Read( stream );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( bmp.Width() == 10 );
  CPPUNIT_ASSERT( bmp.Height() == 10 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 2 );

  uint8_t B, G, R;
  bmp.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 2 );
  CPPUNIT_ASSERT( G == 4 );
  CPPUNIT_ASSERT( R == 6 );
  bmp.GetColorTable( 1, B, G, R );
  CPPUNIT_ASSERT( B == 3 );
  CPPUNIT_ASSERT( G == 5 );
  CPPUNIT_ASSERT( R == 7 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 1 );
}

void BmpTest::testRead4Bits()
{
  std::stringstream stream;

  // FileHeader
  stream << "BM" // Id
         << static_cast<uint8_t>(0xC6) << static_cast<uint8_t>(0x00) // file size: 198
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // 54 + 4*16 + 8*10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x76) << static_cast<uint8_t>(0x00) // offset: 118
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // InfoHeader
  stream << static_cast<uint8_t>(0x28) << static_cast<uint8_t>(0x00) // header size: 40
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // width: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // height: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x01) << static_cast<uint8_t>(0x00) // planes: 1
         << static_cast<uint8_t>(0x04) << static_cast<uint8_t>(0x00) // bpp: 4
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // compression: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x50) << static_cast<uint8_t>(0x00) // image data siz: 80
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // x resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // y resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // colors used: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // important colors: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // color table
  for( uint8_t i = 0; i < 16; ++i )
  {
    stream << static_cast<uint8_t>(i+2) 
           << static_cast<uint8_t>(i+4)
           << static_cast<uint8_t>(i+6)
           << static_cast<uint8_t>(0);
  }

  // image data
  for( uint8_t i = 0; i < 80; ++i )
  {
    uint8_t j = i%8;
    if( j < 5 )
      stream << static_cast<uint8_t>(0x33);  // pixel bytes
    else 
      stream << static_cast<uint8_t>(0x00);  // padding bytes
  }

  // read stream
  vp::Bmp bmp;
  bmp.Read( stream );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( bmp.Width() == 10 );
  CPPUNIT_ASSERT( bmp.Height() == 10 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 16 );

  uint8_t B, G, R;
  bmp.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 2 );
  CPPUNIT_ASSERT( G == 4 );
  CPPUNIT_ASSERT( R == 6 );
  bmp.GetColorTable( 15, B, G, R );
  CPPUNIT_ASSERT( B == 17 );
  CPPUNIT_ASSERT( G == 19 );
  CPPUNIT_ASSERT( R == 21 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 3 );
}

void BmpTest::testRead8Bits()
{
  std::stringstream stream;

  // FileHeader
  stream << "BM" // Id
         << static_cast<uint8_t>(0xAE) << static_cast<uint8_t>(0x00) // file size: 174
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) //  = 54 + 0 + 12*10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x36) << static_cast<uint8_t>(0x00) // offset: 54
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // InfoHeader
  stream << static_cast<uint8_t>(0x28) << static_cast<uint8_t>(0x00) // header size: 40
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // width: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // height: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x01) << static_cast<uint8_t>(0x00) // planes: 1
         << static_cast<uint8_t>(0x08) << static_cast<uint8_t>(0x00) // bpp: 8
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // compression: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x78) << static_cast<uint8_t>(0x00) // image data siz: 120
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // x resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // y resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // colors used: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // important colors: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // color table
  for( uint16_t i = 0; i < 256; ++i )
    stream << static_cast<uint8_t>(i) << static_cast<uint8_t>(i)
           << static_cast<uint8_t>(i) << static_cast<uint8_t>(0);

  // image data
  for( uint16_t i = 0; i < 120; ++i )
  {
    uint8_t j = i%12;
    if( j < 10 )
      stream << static_cast<uint8_t>(0xFF);  // pixel bytes
    else
      stream << static_cast<uint8_t>(0x00);  // padding bytes
  }

  // read stream
  vp::Bmp bmp;
  bmp.Read( stream );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( bmp.Width() == 10 );
  CPPUNIT_ASSERT( bmp.Height() == 10 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 256 );

  uint8_t B, G, R;
  bmp.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 0 );
  CPPUNIT_ASSERT( G == 0 );
  CPPUNIT_ASSERT( R == 0 );
  bmp.GetColorTable( 255, B, G, R );
  CPPUNIT_ASSERT( B == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( R == 255 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
      CPPUNIT_ASSERT( bmp.GetPixel( x, y ) == 255 );
}

void BmpTest::testRead24Bits()
{
  std::stringstream stream;

  // FileHeader
  stream << "BM" // Id
         << static_cast<uint8_t>(0x76) << static_cast<uint8_t>(0x01) // file size: 374
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved1: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved2: 0
         << static_cast<uint8_t>(0x36) << static_cast<uint8_t>(0x00) // offset: 54
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // InfoHeader
  stream << static_cast<uint8_t>(0x28) << static_cast<uint8_t>(0x00) // header size: 40
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // width: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x0A) << static_cast<uint8_t>(0x00) // height: 10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x01) << static_cast<uint8_t>(0x00) // planes: 1
         << static_cast<uint8_t>(0x18) << static_cast<uint8_t>(0x00) // bpp: 24
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // compression: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x40) << static_cast<uint8_t>(0x01) // image data siz: 320
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // x resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // y resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // colors used: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // important colors: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // image data
  for( uint16_t i = 0; i < 320; ++i )
  {
    uint8_t j = i%32;
    if( j < 30 )
      stream << static_cast<uint8_t>(0xAB);  // pixel bytes
    else 
      stream << static_cast<uint8_t>(0x00);  // padding bytes
  }

  // read stream
  vp::Bmp bmp;
  bmp.Read( stream );

  CPPUNIT_ASSERT( bmp.BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( bmp.Width() == 10 );
  CPPUNIT_ASSERT( bmp.Height() == 10 );
  CPPUNIT_ASSERT( bmp.ColorTableSize() == 0 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
    {
      uint8_t B, G, R;
      bmp.GetPixel( x, y, B, G, R );
      CPPUNIT_ASSERT( B == 171 );
      CPPUNIT_ASSERT( G == 171 );
      CPPUNIT_ASSERT( R == 171 );
    }
}
#endif
void BmpTest::testImport()
{
  vp::Bmp bmp;
  CPPUNIT_ASSERT( bmp.Import( "not_exist.bmp" ) == false );
  CPPUNIT_ASSERT_THROW( bmp.Import( "empty.bmp" ), vp::Exception );
}

void BmpTest::testExport()
{
  vp::Bmp bmp( 4, 10, 10 );

  // export to existing file
  CPPUNIT_ASSERT( bmp.Export( "export_exist.bmp" ) == false );
  CPPUNIT_ASSERT( bmp.Export( "export_exist.bmp", true ) );  // overwrite

  // export to new file
  CPPUNIT_ASSERT( bmp.Export( "export_new.bmp" ) );
}