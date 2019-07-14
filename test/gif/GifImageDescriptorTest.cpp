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

#include "GifImageDescriptorTest.h"
#include "GifImageDescriptor.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifImageDescriptorTest );

void GifImageDescriptorTest::testCtors()
{
  // default ctor
  GifImageDescriptor id1;
  CPPUNIT_ASSERT( id1.ID() == 0x2C );
  CPPUNIT_ASSERT( id1.Label() == 0x00 );
  CPPUNIT_ASSERT( id1.Left() == 0 );
  CPPUNIT_ASSERT( id1.Top() == 0 );
  CPPUNIT_ASSERT( id1.Width() == 0 );
  CPPUNIT_ASSERT( id1.Height() == 0 );
  CPPUNIT_ASSERT( id1.LocalColorTable() == false ); // no color table
  CPPUNIT_ASSERT( id1.ColorTableSorted() == false );
  CPPUNIT_ASSERT( id1.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( id1.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( id1.Interlaced() == false );
  uint8_t R, G, B;
  CPPUNIT_ASSERT_THROW( id1.GetColorTable( 0, R, G, B ), vp::Exception );
  CPPUNIT_ASSERT_THROW( id1.SetColorTable( 0, R, G, B ), vp::Exception ); 
  CPPUNIT_ASSERT_THROW( id1.GetPixel( 0, 0 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( id1.SetPixel( 0, 0, 0 ), vp::Exception );

  id1.ColorTableSize( 4 );          // enable color table
  CPPUNIT_ASSERT( id1.LocalColorTable() );
  CPPUNIT_ASSERT( id1.ColorTableSize() == 4 );
  id1.GetColorTable( 0, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( G == 255 );

  CPPUNIT_ASSERT_THROW( id1.ColorTableSize(5), vp::Exception );  // exceed limit
  CPPUNIT_ASSERT( id1.ColorTableSize() == 4 );        // size is not changed

  id1.ColorTableSize(0);  // disable color table
  CPPUNIT_ASSERT( id1.LocalColorTable() == false );
  CPPUNIT_ASSERT( id1.ColorTableSize() == 0 );
  CPPUNIT_ASSERT_THROW( id1.GetColorTable( 0, R, G, B ), vp::Exception );
  CPPUNIT_ASSERT_THROW( id1.SetColorTable( 0, R, G, B ), vp::Exception );

  // ctor
  GifImageDescriptor id2( 2, 8, 8, 0, 0, true );
  CPPUNIT_ASSERT( id2.ID() == 0x2C );
  CPPUNIT_ASSERT( id2.Label() == 0x00 );
  CPPUNIT_ASSERT( id2.Left() == 0 );
  CPPUNIT_ASSERT( id2.Top() == 0 );
  CPPUNIT_ASSERT( id2.Width() == 8 );
  CPPUNIT_ASSERT( id2.Height() == 8 );
  CPPUNIT_ASSERT( id2.LocalColorTable() );
  CPPUNIT_ASSERT( id2.ColorTableSorted() == false );
  CPPUNIT_ASSERT( id2.ColorTableSize() == 4 );
  id2.GetColorTable( 0, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( id2.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( id2.Interlaced() == false );
  CPPUNIT_ASSERT( id2.GetPixel( 0, 0 ) == 0 );
  id2.SetPixel( 0, 0, 3 );
  CPPUNIT_ASSERT( id2.GetPixel( 0, 0 ) == 3 );

  // copy ctor
  GifImageDescriptor id3 = id2;
  CPPUNIT_ASSERT( id3.ID() == 0x2C );
  CPPUNIT_ASSERT( id3.Label() == 0x00 );
  CPPUNIT_ASSERT( id3.Left() == 0 );
  CPPUNIT_ASSERT( id3.Top() == 0 );
  CPPUNIT_ASSERT( id3.Width() == 8 );
  CPPUNIT_ASSERT( id3.Height() == 8 );
  CPPUNIT_ASSERT( id3.LocalColorTable() );
  CPPUNIT_ASSERT( id3.Interlaced() == false );
  CPPUNIT_ASSERT( id3.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( id3.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( id3.ColorTableSorted() == false );
  id3.GetColorTable( 0, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( id3.GetPixel( 0, 0 ) == 3 );

  // assignment
  id1 = id3;
  CPPUNIT_ASSERT( id1.ID() == 0x2C );
  CPPUNIT_ASSERT( id1.Label() == 0x00 );
  CPPUNIT_ASSERT( id1.Left() == 0 );
  CPPUNIT_ASSERT( id1.Top() == 0 );
  CPPUNIT_ASSERT( id1.Width() == 8 );
  CPPUNIT_ASSERT( id1.Height() == 8 );
  CPPUNIT_ASSERT( id1.LocalColorTable() );
  CPPUNIT_ASSERT( id1.Interlaced() == false );
  CPPUNIT_ASSERT( id1.ColorTableSize() == 4 );
  id1.GetColorTable( 0, R, G, B );
  CPPUNIT_ASSERT( R == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( id1.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( id1.ColorTableSorted() == false );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 3 );
}

void GifImageDescriptorTest::testPixelIndex()
{
  GifImageDescriptor id( 2, 8, 8 );
  
  CPPUNIT_ASSERT( id.PixelIndex( 0, 0 ) == 0 );
  CPPUNIT_ASSERT( id.PixelIndex( 7, 0 ) == 7 );
  CPPUNIT_ASSERT( id.PixelIndex( 7, 3 ) == 31 );
  CPPUNIT_ASSERT( id.PixelIndex( 0, 7 ) == 56 );
  CPPUNIT_ASSERT( id.PixelIndex( 7, 7 ) == 63 );

  CPPUNIT_ASSERT_THROW( id.PixelIndex( -1, 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( id.PixelIndex( 8, 1 ),  vp::Exception );
  CPPUNIT_ASSERT_THROW( id.PixelIndex( 1, -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( id.PixelIndex( 1, 8 ),  vp::Exception );
}

void GifImageDescriptorTest::testCrop()
{
  // 6x6 image
  GifImageDescriptor id0( 3, 6, 6 );

  // set four 3x3 regions
  uint16_t x, y;
  for( x = 0; x < 3; ++x )
  {
    for( y = 0; y < 3; ++y )
    {
      id0.SetPixel( x, y, 1 );
      id0.SetPixel( x, y + 3, 3 );
    }
  }

  for( x = 3; x < 6; ++x )
  {
    for( y = 0; y < 3; ++y )
    {
      id0.SetPixel( x, y, 2 );
      id0.SetPixel( x, y + 3, 4 );
    }
  }

  // crop: 1x2 on top-left
  GifImageDescriptor id1 = id0;
  id1.Crop( 2, 1, 1, 2 );
  CPPUNIT_ASSERT( id1.Left() == 2 );
  CPPUNIT_ASSERT( id1.Top() == 1 );
  CPPUNIT_ASSERT( id1.Width() == 1 );
  CPPUNIT_ASSERT( id1.Height() == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 1 );

  // crop: 2x1 on top-right
  id1 = id0;
  id1.Crop( 4, 1, 2, 1 );
  CPPUNIT_ASSERT( id1.Left() == 4 );
  CPPUNIT_ASSERT( id1.Top() == 1 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 2 );

  // crop: 2x1 on bottom-left
  id1 = id0;
  id1.Crop( 1, 3, 2, 1 );
  CPPUNIT_ASSERT( id1.Left() == 1 );
  CPPUNIT_ASSERT( id1.Top() == 3 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 3 );

  // crop: 1x2 on bottom-right
  id1 = id0;
  id1.Crop( 3, 4, 1, 2 );
  CPPUNIT_ASSERT( id1.Left() == 3 );
  CPPUNIT_ASSERT( id1.Top() == 4 );
  CPPUNIT_ASSERT( id1.Width() == 1 );
  CPPUNIT_ASSERT( id1.Height() == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 4 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 4 );

  // crop: 2x2 in center
  id1 = id0;
  id1.Crop( 2, 2, 2, 2 );
  CPPUNIT_ASSERT( id1.Left() == 2 );
  CPPUNIT_ASSERT( id1.Top() == 2 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 1 ) == 4 );

  // crop: 4x3, then 2x2
  id1 = id0;
  id1.Crop( 1, 2, 4, 3 );
  CPPUNIT_ASSERT( id1.Left() == 1 );
  CPPUNIT_ASSERT( id1.Top() == 2 );
  CPPUNIT_ASSERT( id1.Width() == 4 );
  CPPUNIT_ASSERT( id1.Height() == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 2, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 3, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 1 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 2 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 2 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 2, 1 ) == 4 );
  CPPUNIT_ASSERT( id1.GetPixel( 3, 1 ) == 4 );
  CPPUNIT_ASSERT( id1.GetPixel( 2, 2 ) == 4 );
  CPPUNIT_ASSERT( id1.GetPixel( 3, 2 ) == 4 );
  // 2x2
  id1.Crop( 2, 2, 2, 2 );
  CPPUNIT_ASSERT( id1.Left() == 2 );
  CPPUNIT_ASSERT( id1.Top() == 2 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 1 ) == 4 );

  // crop again with same arguments, no change
  id1.Crop( 2, 2, 2, 2 );
  CPPUNIT_ASSERT( id1.Left() == 2 );
  CPPUNIT_ASSERT( id1.Top() == 2 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 1 ) == 4 );

  // wrong value
  CPPUNIT_ASSERT_THROW( id1.Crop( 1, 2, 1, 1 ), vp::Exception ); // Left exceeds lower limit
  CPPUNIT_ASSERT_THROW( id1.Crop( 4, 2, 1, 1 ), vp::Exception ); // Left exceeds upper limit
  CPPUNIT_ASSERT_THROW( id1.Crop( 2, 1, 1, 1 ), vp::Exception ); // Top exceeds lower limit
  CPPUNIT_ASSERT_THROW( id1.Crop( 2, 4, 1, 1 ), vp::Exception ); // Top exceeds upper limit
  CPPUNIT_ASSERT_THROW( id1.Crop( 2, 2, 0, 1 ), vp::Exception ); // zero Width
  CPPUNIT_ASSERT_THROW( id1.Crop( 2, 2, 3, 1 ), vp::Exception ); // Width exceeds upper limit
  CPPUNIT_ASSERT_THROW( id1.Crop( 2, 2, 1, 0 ), vp::Exception ); // zero Height
  CPPUNIT_ASSERT_THROW( id1.Crop( 2, 2, 1, 3 ), vp::Exception ); // Height exceeds upper limit
  // no change
  CPPUNIT_ASSERT( id1.Left() == 2 );
  CPPUNIT_ASSERT( id1.Top() == 2 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 0 ) == 2 );
  CPPUNIT_ASSERT( id1.GetPixel( 0, 1 ) == 3 );
  CPPUNIT_ASSERT( id1.GetPixel( 1, 1 ) == 4 );
}

void GifImageDescriptorTest::testOutput()
{
  //
  // no color table
  //
  GifImageDescriptor id1( 7, 3, 2, 4, 3 );
  std::stringstream stream1;
  id1.Write( stream1 );
  std::string str = stream1.str();
  CPPUNIT_ASSERT( str.size() == 18 );

  // GifImageDescriptor: 10 bytes
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x2C ); // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 4 );    // left
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 3 );    // top
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 3 );    // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 2 );   // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[9]) == 0x00 ); // packed byte

  // ImageData: 8 bytes
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[10]) == 7 );   // bpp
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[11]) == 5 );   // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[12]) == 128 ); // clear code
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[13]) == 0 ); 
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[14]) == 130 ); 
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[15]) == 131 ); 
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[16]) == 129 ); // EOI code
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[17]) == 0 );   // terminator

  //
  // has color table
  //
  GifImageDescriptor id2( 3, 2, 2, 5, 8, true );
  std::stringstream stream2;
  id2.Write( stream2 );
  str = stream2.str();
  CPPUNIT_ASSERT( str.size() == 40 );  // 10 + 24 + 6

  // GifImageDescriptor: 10 bytes
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x2C ); // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 5 );    // left
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 8 );    // top
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 2 );    // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 2 );    // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[9]) == 0x82 ); // packed byte

  // ColorTable: 8 entries = 24 bytes
  for( uint8_t i = 10; i <= 33; ++i )
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i]) == 0xFF );

  // ImageData: 6 bytes
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[34]) == 3 );    // bpp
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[35]) == 3 );    // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[36]) == 0x08 ); // clear code 8, code 0
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[37]) == 0x0A ); // code 10, code 0
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[38]) == 0x09 ); // EOI code 9
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[39]) == 0 );    // terminator
}

void GifImageDescriptorTest::testInput()
{
  //
  // no color table
  //
  std::stringstream stream1;
  
  // GifImageDescriptor
  stream1 << static_cast<uint8_t>(0x2C);  // ID
  stream1 << static_cast<uint8_t>(0x04) << static_cast<uint8_t>(0x00); // left
  stream1 << static_cast<uint8_t>(0x02) << static_cast<uint8_t>(0x00); // top
  stream1 << static_cast<uint8_t>(0x02) << static_cast<uint8_t>(0x00); // width
  stream1 << static_cast<uint8_t>(0x03) << static_cast<uint8_t>(0x00); // height
  stream1 << static_cast<uint8_t>(0x00);  // packed byte

  // ImageData
  stream1 << static_cast<uint8_t>(7);   // bpp
  stream1 << static_cast<uint8_t>(5);   // sub-block size
  stream1 << static_cast<uint8_t>(128); // clear code
  stream1 << static_cast<uint8_t>(0); 
  stream1 << static_cast<uint8_t>(130);
  stream1 << static_cast<uint8_t>(131);
  stream1 << static_cast<uint8_t>(129); // EOI code
  stream1 << static_cast<uint8_t>(0);   // terminator

  std::string str = stream1.str();
  CPPUNIT_ASSERT( str.size() == 18 );

  GifImageDescriptor id1;
  CPPUNIT_ASSERT( stream1.get() == 0x2C );  // ID
  id1.Read( stream1 );
  CPPUNIT_ASSERT( id1.ID() == 0x2C );
  CPPUNIT_ASSERT( id1.Label() == 0x00 );
  CPPUNIT_ASSERT( id1.Left() == 4 );
  CPPUNIT_ASSERT( id1.Top() == 2 );
  CPPUNIT_ASSERT( id1.Width() == 2 );
  CPPUNIT_ASSERT( id1.Height() == 3 );
  CPPUNIT_ASSERT( id1.LocalColorTable() == false );
  CPPUNIT_ASSERT( id1.ColorTableSorted() == false );
  CPPUNIT_ASSERT( id1.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( id1.BitsPerPixel() == 7 );
  CPPUNIT_ASSERT( id1.Interlaced() == false );
  for( uint8_t x = 0; x <= 1; ++x )
    for( uint8_t y = 0; y <= 2; ++y )
      CPPUNIT_ASSERT( id1.GetPixel( x, y ) == 0 );

  //
  // has color table
  //
  std::stringstream stream2;
  
  // GifImageDescriptor
  stream2 << static_cast<uint8_t>(0x2C);  // ID
  stream2 << static_cast<uint8_t>(0x04) << static_cast<uint8_t>(0x00); // left
  stream2 << static_cast<uint8_t>(0x02) << static_cast<uint8_t>(0x00); // top
  stream2 << static_cast<uint8_t>(0x02) << static_cast<uint8_t>(0x00); // width
  stream2 << static_cast<uint8_t>(0x02) << static_cast<uint8_t>(0x00); // height
  stream2 << static_cast<uint8_t>(0x82);  // packed byte
  
  // ColorTable
  for( uint8_t i = 0; i < 8; ++i )
  {
    stream2 << static_cast<uint8_t>(i);
    stream2 << static_cast<uint8_t>(i+1);
    stream2 << static_cast<uint8_t>(i+2);
  }

  // ImageData
  stream2 << static_cast<uint8_t>(3);    // bpp
  stream2 << static_cast<uint8_t>(3);    // sub-block size
  stream2 << static_cast<uint8_t>(0x58); // clear code 8, code 5
  stream2 << static_cast<uint8_t>(0x5A); // code 10, code 5
  stream2 << static_cast<uint8_t>(0x09); // EOI code 9
  stream2 << static_cast<uint8_t>(0);    // terminator

  str = stream2.str();
  CPPUNIT_ASSERT( str.size() == 40 );

  GifImageDescriptor id2;
  CPPUNIT_ASSERT( stream2.get() == 0x2C );  // ID
  id2.Read( stream2 );
  CPPUNIT_ASSERT( id2.ID() == 0x2C );
  CPPUNIT_ASSERT( id2.Label() == 0x00 );
  CPPUNIT_ASSERT( id2.Left() == 4 );
  CPPUNIT_ASSERT( id2.Top() == 2 );
  CPPUNIT_ASSERT( id2.Width() == 2 );
  CPPUNIT_ASSERT( id2.Height() == 2 );
  CPPUNIT_ASSERT( id2.LocalColorTable() );
  CPPUNIT_ASSERT( id2.Interlaced() == false );
  CPPUNIT_ASSERT( id2.ColorTableSorted() == false );
  CPPUNIT_ASSERT( id2.ColorTableSize() == 8 );

  for( uint8_t i = 0; i < 8; ++i )
  {
    uint8_t R, G, B;
    id2.GetColorTable( i, R, G, B );
    CPPUNIT_ASSERT( R == i );
    CPPUNIT_ASSERT( G == i+1 );
    CPPUNIT_ASSERT( B == i+2 );
  }

  CPPUNIT_ASSERT( id2.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( id2.Interlaced() == false );
  for( uint8_t x = 0; x <= 1; ++x )
    for( uint8_t y = 0; y <= 1; ++y )
      CPPUNIT_ASSERT( id2.GetPixel( x, y ) == 5 );
}
