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

#include "BmpImplTest.h"
#include "BmpImpl.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BmpImplTest );

void BmpImplTest::testCtors()
{
  uint8_t B, G, R;

  // ctor with default values
  BmpImpl impl1;
  CPPUNIT_ASSERT( impl1.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( impl1.Width() == 1 );
  CPPUNIT_ASSERT( impl1.Height() == 1 );
  // one pixel
  impl1.SetPixel( 0, 0, 1 );
  CPPUNIT_ASSERT( impl1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT_THROW( impl1.SetPixel( 0, 0, 23, 24, 25 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( impl1.GetPixel( 0, 0, B, G, R ), vp::Exception );
  // color table
  CPPUNIT_ASSERT( impl1.ColorTableSize() == 2 );
  impl1.SetColorTable( 0, 23, 24, 25 );
  impl1.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 23 );
  CPPUNIT_ASSERT( G == 24 );
  CPPUNIT_ASSERT( R == 25 );

  // ctor
  BmpImpl impl2( 4, 10, 10 );
  CPPUNIT_ASSERT( impl2.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( impl2.Width() == 10 );
  CPPUNIT_ASSERT( impl2.Height() == 10 );
  CPPUNIT_ASSERT( impl2.ColorTableSize() == 16 );

  // copy ctor
  BmpImpl impl3(impl1);
  CPPUNIT_ASSERT( impl3.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( impl3.Width() == 1 );
  CPPUNIT_ASSERT( impl3.Height() == 1 );
  CPPUNIT_ASSERT( impl3.ColorTableSize() == 2 );

  // wrong bpp
  try {
    BmpImpl bmpimpl( 2, 3, 4 );
    CPPUNIT_ASSERT_MESSAGE( "Expected exception didn't throw", false );
  }
  catch( const vp::Exception& ) {}
}

void BmpImplTest::testRead1Bit()
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
  BmpImpl impl;
  impl.Read( stream );

  CPPUNIT_ASSERT( impl.BitsPerPixel() == 1 );
  CPPUNIT_ASSERT( impl.Width() == 10 );
  CPPUNIT_ASSERT( impl.Height() == 10 );
  CPPUNIT_ASSERT( impl.ColorTableSize() == 2 );

  uint8_t B, G, R;
  impl.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 2 );
  CPPUNIT_ASSERT( G == 4 );
  CPPUNIT_ASSERT( R == 6 );
  impl.GetColorTable( 1, B, G, R );
  CPPUNIT_ASSERT( B == 3 );
  CPPUNIT_ASSERT( G == 5 );
  CPPUNIT_ASSERT( R == 7 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
      CPPUNIT_ASSERT( impl.GetPixel( x, y ) == 1 );
}

void BmpImplTest::testRead4Bits()
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
  BmpImpl impl;
  impl.Read( stream );

  CPPUNIT_ASSERT( impl.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( impl.Width() == 10 );
  CPPUNIT_ASSERT( impl.Height() == 10 );
  CPPUNIT_ASSERT( impl.ColorTableSize() == 16 );

  uint8_t B, G, R;
  impl.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 2 );
  CPPUNIT_ASSERT( G == 4 );
  CPPUNIT_ASSERT( R == 6 );
  impl.GetColorTable( 15, B, G, R );
  CPPUNIT_ASSERT( B == 17 );
  CPPUNIT_ASSERT( G == 19 );
  CPPUNIT_ASSERT( R == 21 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
      CPPUNIT_ASSERT( impl.GetPixel( x, y ) == 3 );
}

void BmpImplTest::testRead8Bits()
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
  BmpImpl impl;
  impl.Read( stream );

  CPPUNIT_ASSERT( impl.BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( impl.Width() == 10 );
  CPPUNIT_ASSERT( impl.Height() == 10 );
  CPPUNIT_ASSERT( impl.ColorTableSize() == 256 );

  uint8_t B, G, R;
  impl.GetColorTable( 0, B, G, R );
  CPPUNIT_ASSERT( B == 0 );
  CPPUNIT_ASSERT( G == 0 );
  CPPUNIT_ASSERT( R == 0 );
  impl.GetColorTable( 255, B, G, R );
  CPPUNIT_ASSERT( B == 255 );
  CPPUNIT_ASSERT( G == 255 );
  CPPUNIT_ASSERT( R == 255 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
      CPPUNIT_ASSERT( impl.GetPixel( x, y ) == 255 );
}

void BmpImplTest::testRead24Bits()
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
  BmpImpl impl;
  impl.Read( stream );

  CPPUNIT_ASSERT( impl.BitsPerPixel() == 24 );
  CPPUNIT_ASSERT( impl.Width() == 10 );
  CPPUNIT_ASSERT( impl.Height() == 10 );
  CPPUNIT_ASSERT( impl.ColorTableSize() == 0 );

  for( uint8_t y = 0; y < 10; ++y )
    for( uint8_t x = 0; x < 10; ++x )
    {
      uint8_t B, G, R;
      impl.GetPixel( x, y, B, G, R );
      CPPUNIT_ASSERT( B == 171 );
      CPPUNIT_ASSERT( G == 171 );
      CPPUNIT_ASSERT( R == 171 );
    }
}

void BmpImplTest::testReadWrongID()
{
  std::stringstream stream;

  // FileHeader
  stream << "MB" // wrong ID
         << static_cast<uint8_t>(0x66) << static_cast<uint8_t>(0x00) // file size: 102
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) //  = 54 + 4*2 + 4*10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x3E) << static_cast<uint8_t>(0x00) // offset: 62
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);//  = 54 + 4*2

  // read stream
  BmpImpl impl;
  CPPUNIT_ASSERT_THROW( impl.Read(stream), vp::Exception );
}

void BmpImplTest::testReadWrongFileHeaderSize()
{
  std::stringstream stream;

  // FileHeader
  stream << "BM" // ID
         << static_cast<uint8_t>(0x66) << static_cast<uint8_t>(0x00) // file size: 102
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) //  = 54 + 4*2 + 4*10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x3E) << static_cast<uint8_t>(0x00) // offset: 62
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);//  = 54 + 4*2

  // InfoHeader
  stream << static_cast<uint8_t>(0x29) << static_cast<uint8_t>(0x00) // WRON header size: 41
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

  // read stream
  BmpImpl impl;
  CPPUNIT_ASSERT_THROW( impl.Read(stream), vp::Exception );
}

void BmpImplTest::testReadWrongBpp()
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
         << static_cast<uint8_t>(0x02) << static_cast<uint8_t>(0x00) // WRONG bpp: 2
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

  // read stream
  BmpImpl impl;
  CPPUNIT_ASSERT_THROW( impl.Read(stream), vp::Exception );
}

void BmpImplTest::testReadWrongFileHeader()
{
  std::stringstream stream;

  // FileHeader
  stream << "BM" // Id
         << static_cast<uint8_t>(0x66) << static_cast<uint8_t>(0x00) // file size: 102
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) //  = 54 + 4*2 + 4*10
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // reserved 1
         << static_cast<uint8_t>(0x3F) << static_cast<uint8_t>(0x00) // WRONG offset: 63
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);// 

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

  // read stream
  BmpImpl impl;
  CPPUNIT_ASSERT_THROW( impl.Read(stream), vp::Exception );
}

void BmpImplTest::testReadWrongInfoHeader()
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
         << static_cast<uint8_t>(0x29) << static_cast<uint8_t>(0x00) // WRON image data siz: 41
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // x resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0xC4) << static_cast<uint8_t>(0x0E) // y resolution: 3780
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // colors used: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00)
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00) // important colors: 0
         << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);

  // read stream
  BmpImpl impl;
  CPPUNIT_ASSERT_THROW( impl.Read(stream), vp::Exception );
}
