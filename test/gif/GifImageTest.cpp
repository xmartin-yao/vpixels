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

#include "GifImageTest.h"
#include "GifImage.h"
#include "Gif.h"
#include "GifImageDescriptor.h"
#include "GifGraphicsControlExt.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifImageTest );

void GifImageTest::testOneComponent()
{
  // gif contains one image
  vp::Gif gif( 2, 3, 4 );
  vp::GifImage& img = gif[0];

  CPPUNIT_ASSERT( img.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( img.Left() == 0 );
  CPPUNIT_ASSERT( img.Top() == 0 );
  CPPUNIT_ASSERT( img.Width() == 3 );
  CPPUNIT_ASSERT( img.Height() == 4 );

  CPPUNIT_ASSERT( img.ColorTable() == false ); // no local color table
  CPPUNIT_ASSERT( img.ColorTableSize() == 0 );
  img.ColorTableSize(4);                       // enable local color table
  CPPUNIT_ASSERT( img.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( img.CheckColorTable() == 4 );

  // single image
  CPPUNIT_ASSERT( img.SingleImage() );
  CPPUNIT_ASSERT_THROW( img.Delay(), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.Delay( 100 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.DisposalMethod(), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.DisposalMethod( 1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.HasTransColor(), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.HasTransColor( true ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.TransColor(), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.TransColor( 1 ), vp::Exception );
}

void GifImageTest::testTwoComponents()
{
  // gif contains three images
  vp::Gif gif( 2, 3, 4, 3 );
  vp::GifImage& img = gif[0];

  CPPUNIT_ASSERT( img.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( img.Left() == 0 );
  CPPUNIT_ASSERT( img.Top() == 0 );
  CPPUNIT_ASSERT( img.Width() == 3 );
  CPPUNIT_ASSERT( img.Height() == 4 );

  CPPUNIT_ASSERT( img.ColorTable() == false ); // no local color table
  CPPUNIT_ASSERT( img.ColorTableSize() == 0 );
  img.ColorTableSize(4);                       // enable local color table
  CPPUNIT_ASSERT( img.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( img.CheckColorTable() == 4 );

  // multiple images
  CPPUNIT_ASSERT( img.SingleImage() == false );
  CPPUNIT_ASSERT( img.Delay() == 0 );
  img.Delay( 100 ); // set delay
  CPPUNIT_ASSERT( img.Delay() == 100 );
  CPPUNIT_ASSERT( img.DisposalMethod() == 1 );
  img.DisposalMethod( 2 );
  CPPUNIT_ASSERT( img.DisposalMethod() == 2 );
  CPPUNIT_ASSERT( img.HasTransColor() == false );
  CPPUNIT_ASSERT( img.TransColor() == 0 );
  img.TransColor( 2 );
  CPPUNIT_ASSERT( img.TransColor() == 2 );
  img.HasTransColor( false );
  CPPUNIT_ASSERT( img.HasTransColor() == false );
  CPPUNIT_ASSERT( img.TransColor() == 0 );
}


void GifImageTest::testAssignment()
{
  vp::Gif gif( 2, 3, 3, 2 );

  // image 0
  vp::GifImage& img0 = gif[0];
  img0.SetPixel( 1, 1, 3 );
  img0.Delay( 500 );

  // image 1
  vp::GifImage& img1 = gif[1];
  CPPUNIT_ASSERT( img1.GetPixel( 1, 1 ) == 0 );
  CPPUNIT_ASSERT( img1.Delay() == 0 );
  img1 = img0;
  CPPUNIT_ASSERT( img1.GetPixel( 1, 1 ) == 3 );
  CPPUNIT_ASSERT( img1.Delay() == 500 );

  // no effect
  img0 = img0;
  CPPUNIT_ASSERT( img0.GetPixel( 1, 1 ) == 3 );
  CPPUNIT_ASSERT( img0.Delay() == 500 );

  // images belong to different but compatible vp::Gif objects
  vp::Gif gif2( 2, 3, 3, 2 ); // same bpp, width and height
  vp::GifImage& img2 = gif2[0];
  img2 = img0;
  CPPUNIT_ASSERT( img2.GetPixel( 1, 1 ) == 3 );
  CPPUNIT_ASSERT( img2.Delay() == 500 );

  // cannot copy, b/c images belong to incompatible vp::Gif objects
  vp::Gif gif3( 2, 4, 3, 2 ); // different width
  CPPUNIT_ASSERT_THROW( gif3[0] = img0, vp::Exception );
  vp::Gif gif4( 2, 3, 2, 2 ); // different height
  CPPUNIT_ASSERT_THROW( gif4[0] = img0, vp::Exception );
}

void GifImageTest::testSetPixel()
{
  vp::Gif gif( 2, 5, 5, 2 );
  vp::GifImage& img = gif[0];
  CPPUNIT_ASSERT( img.GetPixel( 0, 0 ) == 0 );
  CPPUNIT_ASSERT( img.GetPixel( 4, 4 ) == 0 );

  img.SetPixel( 0, 0, 2 );
  img.SetPixel( 4, 4, 3 );
  CPPUNIT_ASSERT( img.GetPixel( 0, 0 ) == 2 );
  CPPUNIT_ASSERT( img.GetPixel( 4, 4 ) == 3 );

  // color index exceeds 3 
  CPPUNIT_ASSERT_THROW( img.SetPixel( 0, 0, 4 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.SetPixel( 4, 4, 5 ), vp::Exception );

  // when there's neither global nor local color table
  gif.ColorTableSize(0);
  CPPUNIT_ASSERT_THROW( img.SetPixel( 0, 0, 2 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( img.SetPixel( 4, 4, 3 ), vp::Exception );
}

void GifImageTest::testSetAllPixels()
{
  vp::Gif gif( 2, 5, 5, 2 );
  vp::GifImage& img = gif[0];
  for( uint8_t x = 0; x < 5; ++x )
    for( uint8_t y = 0; y < 5; ++y )
      CPPUNIT_ASSERT( img.GetPixel( x, y ) == 0 );

  img.SetAllPixels(3);
  for( uint8_t x = 0; x < 5; ++x )
    for( uint8_t y = 0; y < 5; ++y )
      CPPUNIT_ASSERT( img.GetPixel( x, y ) == 3 );

  // color index exceeds 3 
  CPPUNIT_ASSERT_THROW( img.SetAllPixels(4), vp::Exception );

  // when there's neither global nor local color table
  gif.ColorTableSize(0);
  CPPUNIT_ASSERT_THROW( img.SetAllPixels(0), vp::Exception );
}
