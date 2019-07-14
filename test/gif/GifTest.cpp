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

#include "GifTest.h"
#include "Gif.h"
#include "GifImage.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifTest );

void GifTest::testCtor()
{
  // ctor with default arguments
  vp::Gif gif1;
  CPPUNIT_ASSERT( gif1.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( gif1.Width() == 1 );
  CPPUNIT_ASSERT( gif1.Height() == 1 );

  CPPUNIT_ASSERT( gif1.ColorTable() == true );
  CPPUNIT_ASSERT( gif1.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( gif1.Images() == 1 );

  // ctor
  vp::Gif gif2( 2, 3, 2 );
  CPPUNIT_ASSERT( gif2.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( gif2.Width() == 3 );
  CPPUNIT_ASSERT( gif2.Height() == 2 );

  CPPUNIT_ASSERT( gif2.ColorTable() );             // has global color
  CPPUNIT_ASSERT( gif2.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( gif2[0].ColorTable() == false );  // no local color
  CPPUNIT_ASSERT( gif2[0].ColorTableSize() == 0 );

  CPPUNIT_ASSERT( gif2.Images() == 1 );
  CPPUNIT_ASSERT_THROW( gif2[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif2[1], vp::Exception );

  // ctor
  vp::Gif gif3( 3, 2, 3, 2 );
  CPPUNIT_ASSERT( gif3.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif3.Width() == 2 );
  CPPUNIT_ASSERT( gif3.Height() == 3 );

  CPPUNIT_ASSERT( gif3.ColorTable() );             // has global color
  CPPUNIT_ASSERT( gif3.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( gif3[0].ColorTable() == false );  // no local color
  CPPUNIT_ASSERT( gif3[0].ColorTableSize() == 0 );

  CPPUNIT_ASSERT( gif3.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gif2[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif2[2], vp::Exception );

  // ctor
  vp::Gif gif4( 3, 2, 3, 2, false );
  CPPUNIT_ASSERT( gif4.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif4.Width() == 2 );
  CPPUNIT_ASSERT( gif4.Height() == 3 );

  CPPUNIT_ASSERT( gif4.ColorTable() == false ); // no global color
  CPPUNIT_ASSERT( gif4.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gif4[0].ColorTable() );        // has local color
  CPPUNIT_ASSERT( gif4[0].ColorTableSize() == 8 );

  CPPUNIT_ASSERT( gif4.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gif4[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif4[2], vp::Exception );

  // copy ctor
  vp::Gif gif5 = gif4;
  CPPUNIT_ASSERT( gif5.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif5.Width() == 2 );
  CPPUNIT_ASSERT( gif5.Height() == 3 );

  CPPUNIT_ASSERT( gif5.ColorTable() == false );
  CPPUNIT_ASSERT( gif5.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gif5[0].ColorTable() );
  CPPUNIT_ASSERT( gif5[0].ColorTableSize() == 8 );

  CPPUNIT_ASSERT( gif5.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gif5[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif5[2], vp::Exception );

  // copy assign
  gif1 = gif3;
  CPPUNIT_ASSERT( gif1.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif1.Width() == 2 );
  CPPUNIT_ASSERT( gif1.Height() == 3 );

  CPPUNIT_ASSERT( gif1.ColorTable() );             // has global color
  CPPUNIT_ASSERT( gif1.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( gif1[0].ColorTable() == false );  // no local color
  CPPUNIT_ASSERT( gif1[0].ColorTableSize() == 0 );

  CPPUNIT_ASSERT( gif1.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gif2[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif2[2], vp::Exception );

  // move ctor
  vp::Gif gif6 = std::move(gif4);
  CPPUNIT_ASSERT( !gif4 );  // gif4 moved
  CPPUNIT_ASSERT( gif6.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif6.Width() == 2 );
  CPPUNIT_ASSERT( gif6.Height() == 3 );

  CPPUNIT_ASSERT( gif6.ColorTable() == false );
  CPPUNIT_ASSERT( gif6.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gif6[0].ColorTable() );
  CPPUNIT_ASSERT( gif6[0].ColorTableSize() == 8 );

  CPPUNIT_ASSERT( gif6.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gif6[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif6[2], vp::Exception );

  // move assign
  gif2 = std::move(gif5);
  CPPUNIT_ASSERT( !gif5 );  // gif5 moved
  CPPUNIT_ASSERT( gif2.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif2.Width() == 2 );
  CPPUNIT_ASSERT( gif2.Height() == 3 );

  CPPUNIT_ASSERT( gif2.ColorTable() == false );
  CPPUNIT_ASSERT( gif2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gif2[0].ColorTable() );
  CPPUNIT_ASSERT( gif2[0].ColorTableSize() == 8 );

  CPPUNIT_ASSERT( gif2.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gif2[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif2[2], vp::Exception );

  // wrong bpp
  try {
    vp::Gif gif( 9, 3, 4 );
    CPPUNIT_ASSERT_MESSAGE( "Expected exception didn't throw", false );
  }
  catch( const vp::Exception& ) {}
}

void GifTest::testOneImage()
{
  vp::Gif gif( 2, 3, 4 );

  CPPUNIT_ASSERT( gif.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( gif.Width() == 3 );
  CPPUNIT_ASSERT( gif.Height() == 4 );

  CPPUNIT_ASSERT( gif.ColorTable() );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 4 );
  gif.SetColorTable( 3, 4, 5, 6 );
  uint8_t R, G, B;
  gif.GetColorTable( 3, R, G, B );
  CPPUNIT_ASSERT( R == 4 );
  CPPUNIT_ASSERT( G == 5 );
  CPPUNIT_ASSERT( B == 6 );

  CPPUNIT_ASSERT( gif.Images() == 1 );
  vp::GifImage& image = gif[0];

  CPPUNIT_ASSERT( image.Left() == 0 );
  CPPUNIT_ASSERT( image.Top() == 0 );
  CPPUNIT_ASSERT( image.Width() == 3 );
  CPPUNIT_ASSERT( image.Height() == 4 );
  CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 0 );
  image.SetAllPixels( 1 );
  CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 1 );
  CPPUNIT_ASSERT( image.Interlaced() == false );
  CPPUNIT_ASSERT( image.Delay() == 0 );
  CPPUNIT_ASSERT( image.Delay( 100 ) == false ); // unable for one image
  CPPUNIT_ASSERT( image.ColorTable() == false );

  // exceed index range: 0 <= index < size
  CPPUNIT_ASSERT_THROW( gif[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif[1], vp::Exception );
}

void GifTest::testTwoImages()
{
  vp::Gif gif( 3, 2, 3, 2 );

  CPPUNIT_ASSERT( gif.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif.Width() == 2 );
  CPPUNIT_ASSERT( gif.Height() == 3 );

  CPPUNIT_ASSERT( gif.ColorTable() );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 8 );
  gif.SetColorTable( 3, 4, 5, 6 );
  uint8_t R, G, B;
  gif.GetColorTable( 3, R, G, B );
  CPPUNIT_ASSERT( R == 4 );
  CPPUNIT_ASSERT( G == 5 );
  CPPUNIT_ASSERT( B == 6 );

  CPPUNIT_ASSERT( gif.Images() == 2 );
  for( uint8_t i = 0; i < 2; ++i )
  {
    vp::GifImage& image = gif[i];
    CPPUNIT_ASSERT( image.Left() == 0 );
    CPPUNIT_ASSERT( image.Top() == 0 );
    CPPUNIT_ASSERT( image.Width() == 2 );
    CPPUNIT_ASSERT( image.Height() == 3 );
    CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 0 );
    image.SetAllPixels( 1 );
    CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 1 );
    CPPUNIT_ASSERT( image.Interlaced() == false );
    CPPUNIT_ASSERT( image.Delay( 100 ) );
    CPPUNIT_ASSERT( image.Delay() == 100 );
    CPPUNIT_ASSERT( image.ColorTable() == false );
  }

  // exceed index range: 0 <= index < size
  CPPUNIT_ASSERT_THROW( gif[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( gif[2], vp::Exception );
}

void GifTest::testImport()
{
  vp::Gif gif;
  CPPUNIT_ASSERT( gif.Import( "not_exist.gif" ) == false );
  CPPUNIT_ASSERT_THROW( gif.Import( "empty.gif" ), vp::Exception );
  CPPUNIT_ASSERT_THROW( gif.Import( "header_only.gif" ), vp::Exception );
}

void GifTest::testExport()
{
  vp::Gif gif( 2, 10, 10 );

  // export to existing file
  CPPUNIT_ASSERT( gif.Export( "export_exist.gif" ) == false );
  CPPUNIT_ASSERT( gif.Export( "export_exist.gif", true ) );  // overwrite

  // export to new file
  CPPUNIT_ASSERT( gif.Export( "export_new.gif" ) );
}