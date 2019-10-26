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
  CPPUNIT_ASSERT_THROW( image.Delay( 100 ), vp::Exception );  // single image
  CPPUNIT_ASSERT( image.DisposalMethod() == 0 );
  CPPUNIT_ASSERT_THROW( image.DisposalMethod(1), vp::Exception );
  CPPUNIT_ASSERT( image.HasTransColor() == false );
  CPPUNIT_ASSERT_THROW( image.HasTransColor(true), vp::Exception );
  CPPUNIT_ASSERT_THROW( image.TransColor(), vp::Exception );
  CPPUNIT_ASSERT_THROW( image.TransColor(3), vp::Exception );
  CPPUNIT_ASSERT( image.ColorTable() == false );

  CPPUNIT_ASSERT( gif.BackgroundColor() == 0 );
  gif.BackgroundColor(3);
  CPPUNIT_ASSERT( gif.BackgroundColor() == 3 );
  CPPUNIT_ASSERT_THROW( gif.BackgroundColor(4), vp::Exception );  // exceeds
  gif.ColorTableSize(0);  // disable color table
  CPPUNIT_ASSERT( gif.BackgroundColor() == 0 );

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
    CPPUNIT_ASSERT( image.Delay() == 0 );
    image.Delay( 100 );
    CPPUNIT_ASSERT( image.Delay() == 100 );
    CPPUNIT_ASSERT( image.DisposalMethod() == 1 );
    image.DisposalMethod(0);
    CPPUNIT_ASSERT( image.DisposalMethod() == 0 );
    CPPUNIT_ASSERT( image.HasTransColor() == false );
    CPPUNIT_ASSERT( image.TransColor() == 0 );
    image.TransColor(2);
    CPPUNIT_ASSERT( image.HasTransColor() );
    CPPUNIT_ASSERT( image.TransColor() == 2 );
    image.HasTransColor(false);
    CPPUNIT_ASSERT( image.TransColor() == 0 );
    CPPUNIT_ASSERT( image.ColorTable() == false );
  }

  CPPUNIT_ASSERT( gif.BackgroundColor() == 0 );
  gif.BackgroundColor(7);
  CPPUNIT_ASSERT( gif.BackgroundColor() == 7 );
  CPPUNIT_ASSERT_THROW( gif.BackgroundColor(8), vp::Exception );  // exceeds
  gif.ColorTableSize(0);  // disable color table
  CPPUNIT_ASSERT( gif.BackgroundColor() == 0 );

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

void GifTest::testColorTableSize()
{
  vp::Gif gif( 3, 10, 10, 3, true );
  CPPUNIT_ASSERT( gif.ColorTable() );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 3 );
  gif.SetColorTable( 0, 23, 24, 25 );

  vp::GifImage& img0 = gif[0];
  vp::GifImage& img1 = gif[1];
  vp::GifImage& img2 = gif[2];

  // images have no local color table
  CPPUNIT_ASSERT( img0.ColorTable() == false );
  CPPUNIT_ASSERT( img1.ColorTable() == false );
  CPPUNIT_ASSERT( img2.ColorTable() == false );
  CPPUNIT_ASSERT( img0.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 3 );

  // enable local color table of img1
  img1.ColorTableSize(32);
  CPPUNIT_ASSERT( img1.ColorTable() );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 32 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 5 );

  // change global color table size, img1 is not affected
  gif.ColorTableSize(2);
  CPPUNIT_ASSERT( gif.ColorTable() );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 2 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 2 );
  gif.SetColorTable( 1, 23, 24, 25 );

  CPPUNIT_ASSERT( img0.ColorTable() == false );
  CPPUNIT_ASSERT( img1.ColorTable() );
  CPPUNIT_ASSERT( img2.ColorTable() == false );
  CPPUNIT_ASSERT( img0.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 32 );
  CPPUNIT_ASSERT( img2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 2 );

  // disable global color table, some calls throw exception
  gif.ColorTableSize(0);
  CPPUNIT_ASSERT( gif.ColorTable() == false );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT_THROW( gif.SetColorTable( 0, 23, 24, 25 ), vp::Exception );

  CPPUNIT_ASSERT( img0.ColorTable() == false );
  CPPUNIT_ASSERT( img1.ColorTable() );
  CPPUNIT_ASSERT( img2.ColorTable() == false );
  CPPUNIT_ASSERT( img0.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 32 );
  CPPUNIT_ASSERT( img2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 2 );

  CPPUNIT_ASSERT_THROW( img0.SetPixel( 0, 0, 1 ), vp::Exception );
  img1.SetPixel( 0, 0, 1 );
  CPPUNIT_ASSERT_THROW( img2.SetPixel( 0, 0, 1 ), vp::Exception );

  CPPUNIT_ASSERT( img0.GetPixel( 0, 0 ) == 0 );
  CPPUNIT_ASSERT( img1.GetPixel( 0, 0 ) == 1 );
  CPPUNIT_ASSERT( img2.GetPixel( 0, 0 ) == 0 );

  uint8_t R, G, B;
  CPPUNIT_ASSERT_THROW( img0.GetPixel( 0, 0, R, G, B ), vp::Exception );
  img1.GetPixel( 0, 0, R, G, B );
  CPPUNIT_ASSERT_THROW( img2.GetPixel( 0, 0, R, G, B ), vp::Exception );

  // enlarge to max size
  gif.ColorTableSize(256);
  CPPUNIT_ASSERT( gif.ColorTable() );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 256 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 8 );

  CPPUNIT_ASSERT( img0.ColorTable() == false );
  CPPUNIT_ASSERT( img1.ColorTable() );
  CPPUNIT_ASSERT( img2.ColorTable() == false );
  CPPUNIT_ASSERT( img0.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 32 );
  CPPUNIT_ASSERT( img2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 8 );

  // size exceeds 256, no change
  CPPUNIT_ASSERT_THROW( gif.ColorTableSize(257), vp::Exception );
  CPPUNIT_ASSERT( gif.ColorTable() );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 256 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 8 );

  CPPUNIT_ASSERT( img0.ColorTable() == false );
  CPPUNIT_ASSERT( img1.ColorTable() );
  CPPUNIT_ASSERT( img2.ColorTable() == false );
  CPPUNIT_ASSERT( img0.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 32 );
  CPPUNIT_ASSERT( img2.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 8 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 8 );
}

// test Gif::BitsPerPixel() and GifImage::BitsPerPixel() 
void GifTest::testBitsPerPixel()
{
  vp::Gif gif( 3, 10, 10, 3, true );
  vp::GifImage& img0 = gif[0];
  vp::GifImage& img1 = gif[1];
  vp::GifImage& img2 = gif[2];

  // bpp = 3
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 3 );

  // change bpp
  gif.BitsPerPixel(6);
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 64 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 6 );

  // bpp of images may be smaller
  img0.BitsPerPixel(5);
  img1.BitsPerPixel(4);
  img2.BitsPerPixel(3);
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 64 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 3 );

  // bpp of images cannot be larger
  CPPUNIT_ASSERT_THROW( img0.BitsPerPixel( 7 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img1.BitsPerPixel( 7 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img2.BitsPerPixel( 8 ), vp::Exception );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 64 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 4 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 3 );

  // enable local color table of img1
  img1.ColorTableSize(2);
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 2 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 64 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 3 );

  // change bpp of img1
  img1.BitsPerPixel(3);
  CPPUNIT_ASSERT( img1.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( img1.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( gif.BitsPerPixel() == 6 );
  CPPUNIT_ASSERT( gif.ColorTableSize() == 64 );
  CPPUNIT_ASSERT( img0.BitsPerPixel() == 5 );
  CPPUNIT_ASSERT( img2.BitsPerPixel() == 3 );

  // bpp out of range
  CPPUNIT_ASSERT_THROW( gif.BitsPerPixel( 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( gif.BitsPerPixel( 9 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img0.BitsPerPixel( 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img0.BitsPerPixel( 9 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img1.BitsPerPixel( 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img1.BitsPerPixel( 9 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img2.BitsPerPixel( 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img2.BitsPerPixel( 9 ), vp::Exception );
}

void GifTest::testRemove()
{
  vp::Gif gif( 2, 3, 4, 9 );
  CPPUNIT_ASSERT( gif.Images() == 9 );

  CPPUNIT_ASSERT( gif.Remove( 0 ) );  // remove 1st image
  CPPUNIT_ASSERT( gif.Images() == 8 );
  CPPUNIT_ASSERT( gif.Remove( 7 ) );  // remove last one
  CPPUNIT_ASSERT( gif.Images() == 7 );
  CPPUNIT_ASSERT( gif.Remove( 3 ) );  // remove the middle one
  CPPUNIT_ASSERT( gif.Images() == 6 );

  // index out of range
  CPPUNIT_ASSERT_THROW( gif.Remove( -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( gif.Remove( 6 ), vp::Exception );

  // remove images until only one left
  for( size_t nImages = gif.Images() - 1; gif.Images() > 1; --nImages )
  {
    CPPUNIT_ASSERT( gif.Remove( 0 ) );
    CPPUNIT_ASSERT( gif.Images() == nImages );
  }

  // now gif contains only one image
  CPPUNIT_ASSERT( gif.Images() == 1 );
  CPPUNIT_ASSERT_THROW( gif.Remove( 0 ), vp::Exception );
  vp::GifImage& image = gif[0];
  CPPUNIT_ASSERT( image.Delay() == 0 );
  CPPUNIT_ASSERT_THROW( image.Delay( 10 ), vp::Exception );
  CPPUNIT_ASSERT( image.DisposalMethod() == 0 );
  CPPUNIT_ASSERT_THROW( image.DisposalMethod( 1 ), vp::Exception );
  CPPUNIT_ASSERT( image.HasTransColor() == false );
  CPPUNIT_ASSERT_THROW( image.HasTransColor( true ), vp::Exception );
  CPPUNIT_ASSERT_THROW( image.TransColor(), vp::Exception );
  CPPUNIT_ASSERT_THROW( image.TransColor( 3 ), vp::Exception );

  // single image GIF object
  vp::Gif gif2( 2, 3, 4, 1 );
  CPPUNIT_ASSERT_THROW( gif2.Remove( 0 ), vp::Exception );
}
