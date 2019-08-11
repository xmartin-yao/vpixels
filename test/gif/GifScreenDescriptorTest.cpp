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

#include "GifScreenDescriptorTest.h"
#include "GifScreenDescriptor.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifScreenDescriptorTest );

void GifScreenDescriptorTest::testBpp2()
{
  uint8_t  Bpp = 2;
  uint16_t Size = 4;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::testBpp3()
{
  uint8_t  Bpp = 3;
  uint16_t Size = 8;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::testBpp4()
{
  uint8_t  Bpp = 4;
  uint16_t Size = 16;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::testBpp5()
{
  uint8_t  Bpp = 5;
  uint16_t Size = 32;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::testBpp6()
{
  uint8_t  Bpp = 6;
  uint16_t Size = 64;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::testBpp7()
{
  uint8_t  Bpp = 7;
  uint16_t Size = 128;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::testBpp8()
{
  uint8_t  Bpp = 8;
  uint16_t Size = 256;
  Verify( Bpp, Size );
}

void GifScreenDescriptorTest::Verify( const uint8_t& Bpp, const uint16_t& Size )
{
  uint8_t R, G, B;

  // has color table
  GifScreenDescriptor sd1( Bpp, 10, 20 );
  CPPUNIT_ASSERT( sd1.Width() == 10 );
  CPPUNIT_ASSERT( sd1.Height() == 20 );
  CPPUNIT_ASSERT( sd1.GlobalColorTable() );
  CPPUNIT_ASSERT( sd1.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd1.ColorResolution() == Bpp );
  CPPUNIT_ASSERT( sd1.ColorTableSize() == Size );
  CPPUNIT_ASSERT( sd1.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd1.AspectRatio() == 0 );
  sd1.GetColorTable( 1, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( B == 255 );

  // no color table
  GifScreenDescriptor sd2( Bpp, 5, 10, false );
  CPPUNIT_ASSERT( sd2.Width() == 5 );
  CPPUNIT_ASSERT( sd2.Height() == 10 );
  CPPUNIT_ASSERT( sd2.GlobalColorTable() == false );
  CPPUNIT_ASSERT( sd2.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd2.ColorResolution() == Bpp );
  CPPUNIT_ASSERT( sd2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( sd2.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd2.AspectRatio() == 0 );
  CPPUNIT_ASSERT_THROW( sd2.GetColorTable( 1, R, G, B ), vp::Exception );

  // copy ctor
  GifScreenDescriptor sd3 = sd1;
  CPPUNIT_ASSERT( sd3.Width() == 10 );
  CPPUNIT_ASSERT( sd3.Height() == 20 );
  CPPUNIT_ASSERT( sd3.GlobalColorTable() );
  CPPUNIT_ASSERT( sd3.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd3.ColorResolution() == Bpp );
  CPPUNIT_ASSERT( sd3.ColorTableSize() == Size );
  CPPUNIT_ASSERT( sd3.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd3.AspectRatio() == 0 );
  sd3.GetColorTable( 1, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( B == 255 );

  // change color table size
  CPPUNIT_ASSERT_THROW( sd3.ColorTableSize(-1), vp::Exception );  // less than minimum size
  if( Size == 256 )
    CPPUNIT_ASSERT_THROW( sd3.ColorTableSize( Size + 1 ), vp::Exception );  // exceed maximum size
  else
  {
    sd3.ColorTableSize( Size + 1 );
    // resolution and color table size increased
    CPPUNIT_ASSERT( sd3.ColorResolution() == Bpp + 1 );
    CPPUNIT_ASSERT( sd3.ColorTableSize() == (1 << sd3.ColorResolution()) );
  }
  sd3.ColorTableSize( Size - 1 );  // no effect
  CPPUNIT_ASSERT( sd3.ColorTableSize() == Size );
  sd3.ColorTableSize( 1 );  // roundup to 2
  CPPUNIT_ASSERT( sd3.ColorTableSize() == 2 );
  sd3.ColorTableSize( 0 );  // disable color table
  CPPUNIT_ASSERT( sd3.GlobalColorTable() == false );
  CPPUNIT_ASSERT_THROW( sd3.GetColorTable( 1, R, G, B ), vp::Exception );

  // copy ctor
  GifScreenDescriptor sd4 = sd2;
  CPPUNIT_ASSERT( sd4.Width() == 5 );
  CPPUNIT_ASSERT( sd4.Height() == 10 );
  CPPUNIT_ASSERT( sd4.GlobalColorTable() == false );
  CPPUNIT_ASSERT( sd4.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd4.ColorResolution() == Bpp );
  CPPUNIT_ASSERT( sd4.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( sd4.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd4.AspectRatio() == 0 );
  CPPUNIT_ASSERT_THROW( sd4.GetColorTable( 1, R, G, B ), vp::Exception );

  sd4.ColorTableSize( 2 );
  CPPUNIT_ASSERT( sd4.ColorTableSize() == 2 );
  sd4.GetColorTable( 1, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( B == 255 );

  CPPUNIT_ASSERT_THROW( sd4.ColorTableSize(-1), vp::Exception );
}

void GifScreenDescriptorTest::testBackgroundColor()
{
  GifScreenDescriptor sd1( 3, 5, 7 );
  CPPUNIT_ASSERT( sd1.BackgroundColor() == 0 );
  sd1.BackgroundColor(1);
  CPPUNIT_ASSERT( sd1.BackgroundColor() == 1 );
  sd1.BackgroundColor(7);
  CPPUNIT_ASSERT( sd1.BackgroundColor() == 7 );
  CPPUNIT_ASSERT_THROW( sd1.BackgroundColor(8), vp::Exception ); // index exceeds
  sd1.ColorTableSize(0);  // disable color table
  CPPUNIT_ASSERT( sd1.GlobalColorTable() == false );
  CPPUNIT_ASSERT( sd1.BackgroundColor() == 0 );
  CPPUNIT_ASSERT_THROW( sd1.BackgroundColor(0), vp::Exception ); // can't set background color

  GifScreenDescriptor sd2( 2, 4, 6, false );
  CPPUNIT_ASSERT( sd2.GlobalColorTable() == false );
  CPPUNIT_ASSERT( sd2.BackgroundColor() == 0 );
  CPPUNIT_ASSERT_THROW( sd2.BackgroundColor(0), vp::Exception ); // can't set background color
  sd2.ColorTableSize(2); // enable color table
  CPPUNIT_ASSERT( sd2.GlobalColorTable() );
  CPPUNIT_ASSERT( sd2.BackgroundColor() == 0 );
  sd2.BackgroundColor(1);
  CPPUNIT_ASSERT( sd2.BackgroundColor() == 1 );
  CPPUNIT_ASSERT_THROW( sd2.BackgroundColor(2), vp::Exception ); // index exceeds
}

void GifScreenDescriptorTest::testInput()
{
  uint8_t R, G, B;

  // ctor
  GifScreenDescriptor sd( 2, 1, 1 );
  CPPUNIT_ASSERT( sd.Width() == 1 );
  CPPUNIT_ASSERT( sd.Height() == 1 );
  CPPUNIT_ASSERT( sd.GlobalColorTable() == true );
  CPPUNIT_ASSERT( sd.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd.ColorResolution() == 2 );
  CPPUNIT_ASSERT( sd.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( sd.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd.AspectRatio() == 0 );

  //
  // has color table
  //
  std::stringstream stream1;
  stream1 << static_cast<uint8_t>(0x12) << static_cast<uint8_t>(0x00); // width
  stream1 << static_cast<uint8_t>(0x14) << static_cast<uint8_t>(0x00); // height
  stream1 << static_cast<uint8_t>(0xA2);  // packed byte: 10100010(color table, bpp=3)
  stream1 << static_cast<uint8_t>(0x00);  // background color
  stream1 << static_cast<uint8_t>(0x00);  // aspect ratio
  for( uint8_t i = 0; i < 8; ++i )        // color table
    stream1 << i << i << i;

  stream1 >> sd;
  CPPUNIT_ASSERT( sd.Width() == 18 );
  CPPUNIT_ASSERT( sd.Height() == 20 );
  CPPUNIT_ASSERT( sd.GlobalColorTable() );
  CPPUNIT_ASSERT( sd.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd.ColorResolution() == 3 );
  CPPUNIT_ASSERT( sd.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( sd.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd.AspectRatio() == 0 );
  for( uint8_t i = 0; i < 8; ++i )
  {
    sd.GetColorTable( i, R, G, B );
    CPPUNIT_ASSERT( R == i );
    CPPUNIT_ASSERT( G == i );
    CPPUNIT_ASSERT( B == i );
  }

  //
  // no color table
  //
  std::stringstream stream2;
  stream2 << static_cast<uint8_t>(0x14) << static_cast<uint8_t>(0x00); // width
  stream2 << static_cast<uint8_t>(0x12) << static_cast<uint8_t>(0x00); // height
  stream2 << static_cast<uint8_t>(0x30);  // packed byte 00110000
  stream2 << static_cast<uint8_t>(0x00);  // background color
  stream2 << static_cast<uint8_t>(0x00);  // aspect ratio

  stream2 >> sd;
  CPPUNIT_ASSERT( sd.Width() == 20 );
  CPPUNIT_ASSERT( sd.Height() == 18 );
  CPPUNIT_ASSERT( sd.GlobalColorTable() == false );
  CPPUNIT_ASSERT( sd.ColorTableSorted() == false );
  CPPUNIT_ASSERT( sd.ColorResolution() == 4 );
  CPPUNIT_ASSERT( sd.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( sd.BackgroundColor() == 0 );
  CPPUNIT_ASSERT( sd.AspectRatio() == 0 );
  CPPUNIT_ASSERT_THROW( sd.GetColorTable( 0, R, G, B ), vp::Exception );
}

void GifScreenDescriptorTest::testOutput()
{
  //
  // has color table
  //
  GifScreenDescriptor sd1( 3, 18, 20 );
  std::stringstream stream1;
  stream1 << sd1;
  std::string str = stream1.str();
  CPPUNIT_ASSERT( str.size() == 31 ); // 7 + 8*3

  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x12 ); // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0x14 ); // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0xA2 ); // packed byte 10100010
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 0x00 ); // background color
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0x00 ); // aspect ratio
  for( uint8_t i = 0; i < 24; ++i )                       // color table 
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i+7]) == 0xFF );

  //
  // no color table
  //
  GifScreenDescriptor sd2( 4, 20, 18, false );
  std::stringstream stream2;
  stream2 << sd2;
  str = stream2.str();
  CPPUNIT_ASSERT( str.size() == 7 );

  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x14 ); // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0x12 ); // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0x30 ); // 00110000
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 0x00 ); // background color
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0x00 ); // aspect ratio
}
