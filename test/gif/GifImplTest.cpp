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

#include "GifImplTest.h"
#include "GifImpl.h"
#include "GifImage.h"
#include "GifImageVecBuilder.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifImplTest );

void GifImplTest::testCtor()
{
  // ctor with default arguments
  GifImpl gifimpl1;
  CPPUNIT_ASSERT( gifimpl1.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( gifimpl1.Width() == 1 );
  CPPUNIT_ASSERT( gifimpl1.Height() == 1 );

  CPPUNIT_ASSERT( gifimpl1.ColorTable() == true );
  CPPUNIT_ASSERT( gifimpl1.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( gifimpl1.Images() == 1 );

  // ctor
  GifImpl gifimpl2( 2, 3, 2 );
  CPPUNIT_ASSERT( gifimpl2.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( gifimpl2.Width() == 3 );
  CPPUNIT_ASSERT( gifimpl2.Height() == 2 );

  CPPUNIT_ASSERT( gifimpl2.ColorTable() );             // has global color
  CPPUNIT_ASSERT( gifimpl2.ColorTableSize() == 4 );
  CPPUNIT_ASSERT( gifimpl2[0].ColorTable() == false );  // no local color
  CPPUNIT_ASSERT( gifimpl2[0].ColorTableSize() == 0 );

  CPPUNIT_ASSERT( gifimpl2.Images() == 1 );
  CPPUNIT_ASSERT_THROW( gifimpl2[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( gifimpl2[1], vp::Exception );

  // ctor
  GifImpl gifimpl3( 3, 2, 3, 2 );
  CPPUNIT_ASSERT( gifimpl3.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gifimpl3.Width() == 2 );
  CPPUNIT_ASSERT( gifimpl3.Height() == 3 );

  CPPUNIT_ASSERT( gifimpl3.ColorTable() );             // has global color
  CPPUNIT_ASSERT( gifimpl3.ColorTableSize() == 8 );
  CPPUNIT_ASSERT( gifimpl3[0].ColorTable() == false );  // no local color
  CPPUNIT_ASSERT( gifimpl3[0].ColorTableSize() == 0 );

  CPPUNIT_ASSERT( gifimpl3.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gifimpl3[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gifimpl3[2], vp::Exception );

  // ctor
  GifImpl gifimpl4( 3, 2, 3, 2, false );
  CPPUNIT_ASSERT( gifimpl4.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gifimpl4.Width() == 2 );
  CPPUNIT_ASSERT( gifimpl4.Height() == 3 );

  CPPUNIT_ASSERT( gifimpl4.ColorTable() == false ); // no global color
  CPPUNIT_ASSERT( gifimpl4.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gifimpl4[0].ColorTable() );        // has local color
  CPPUNIT_ASSERT( gifimpl4[0].ColorTableSize() == 8 );

  CPPUNIT_ASSERT( gifimpl4.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gifimpl4[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gifimpl4[2], vp::Exception );

  // copy ctor
  GifImpl gifimpl5 = gifimpl4;
  CPPUNIT_ASSERT( gifimpl5.BitsPerPixel() == 3 );
  CPPUNIT_ASSERT( gifimpl5.Width() == 2 );
  CPPUNIT_ASSERT( gifimpl5.Height() == 3 );

  CPPUNIT_ASSERT( gifimpl5.ColorTable() == false );
  CPPUNIT_ASSERT( gifimpl5.ColorTableSize() == 0 );
  CPPUNIT_ASSERT( gifimpl5[0].ColorTable() );
  CPPUNIT_ASSERT( gifimpl5[0].ColorTableSize() == 8 );

  CPPUNIT_ASSERT( gifimpl5.Images() == 2 );
  CPPUNIT_ASSERT_THROW( gifimpl5[-2], vp::Exception );
  CPPUNIT_ASSERT_THROW( gifimpl5[2], vp::Exception );

  // wrong bpp
  try {
    GifImpl gifimpl( 1, 3, 4 );
    CPPUNIT_ASSERT_MESSAGE( "Expected exception didn't throw", false );
  }
  catch( const vp::Exception& ) {}
}

void GifImplTest::testReadOneImage()
{
  std::stringstream stream;

  // GifHeader
  stream << "GIF89a";

  // GifScreenDescriptor
  stream << static_cast<uint8_t>(0x03) <<  static_cast<uint8_t>(0x00);  // width
  stream << static_cast<uint8_t>(0x02) <<  static_cast<uint8_t>(0x00);  // height
  stream << static_cast<uint8_t>(0xA2); // 10100010 (color table, bpp = 3)
  stream << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00);

  // GifColorTable: 8 entries = 24 bytes
  for( uint8_t i = 0; i < 8; ++i )
    stream << static_cast<uint8_t>(i) << static_cast<uint8_t>(i) << static_cast<uint8_t>(i) ;

  // GifImageDescriptor
  stream << static_cast<uint8_t>(0x2C);  // ID
  stream << static_cast<uint8_t>(0x00) <<  static_cast<uint8_t>(0x00);  // left
  stream << static_cast<uint8_t>(0x00) <<  static_cast<uint8_t>(0x00);  // top
  stream << static_cast<uint8_t>(0x03) <<  static_cast<uint8_t>(0x00);  // width
  stream << static_cast<uint8_t>(0x02) <<  static_cast<uint8_t>(0x00);  // height
  stream << static_cast<uint8_t>(0x00);  // packed byte

  // GifImageData: 7,7,7,7,7, -> 8,7,10,11,9 (4 bits/code)
  stream << static_cast<uint8_t>(0x03);  // bpp
  stream << static_cast<uint8_t>(0x03);  // sub-block size
  stream << static_cast<uint8_t>(0x78);  // codes: 8,7
  stream << static_cast<uint8_t>(0xBA);  // codes: 10,11
  stream << static_cast<uint8_t>(0x09);  // code:  9
  stream << static_cast<uint8_t>(0x00);  // sub-block ends

  // Gif Trailer
  stream << GifImpl::m_GifTrailer;

  // read from stream
  GifImpl gifimpl;
  // simplified version of GifImpl::Import()
  CPPUNIT_ASSERT( gifimpl.Read( stream ) == GifImpl::m_GifTrailer ); 
  gifimpl.m_ImageVec = GifImageVecBuilder()( gifimpl, gifimpl.m_ComponentVec );
  CPPUNIT_ASSERT( gifimpl.m_ImageVec.size() == 1 );

  CPPUNIT_ASSERT( gifimpl.Width() == 3 );
  CPPUNIT_ASSERT( gifimpl.Height() == 2 );

  CPPUNIT_ASSERT( gifimpl.ColorTable() );
  CPPUNIT_ASSERT( gifimpl.ColorTableSize() == 8 );
  uint8_t R, G, B;
  gifimpl.GetColorTable( 3, R, G, B );
  CPPUNIT_ASSERT( R == 3 );
  CPPUNIT_ASSERT( G == 3 );
  CPPUNIT_ASSERT( B == 3 );

  CPPUNIT_ASSERT( gifimpl.Images() == 1 );
  vp::GifImage& image = gifimpl[0];
  CPPUNIT_ASSERT( image.Left() == 0 );
  CPPUNIT_ASSERT( image.Top() == 0 );
  CPPUNIT_ASSERT( image.Width() == 3 );
  CPPUNIT_ASSERT( image.Height() == 2 );
  CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 7 );
  CPPUNIT_ASSERT( image.Interlaced() == false );
  CPPUNIT_ASSERT_THROW( image.Delay(), vp::Exception );  // single image
  CPPUNIT_ASSERT_THROW( image.Delay( 100 ), vp::Exception );
  CPPUNIT_ASSERT( image.ColorTable() == false );

  // exceed index range: 0 <= index < size
  CPPUNIT_ASSERT_THROW( gifimpl[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( gifimpl[1], vp::Exception );
}

void GifImplTest::testReadTwoImages()
{
  std::stringstream stream;

  // GifHeader
  stream << "GIF89a";

  // GifScreenDescriptor
  stream << static_cast<uint8_t>(0x02) <<  static_cast<uint8_t>(0x00);  // width
  stream << static_cast<uint8_t>(0x03) <<  static_cast<uint8_t>(0x00);  // height
  stream << static_cast<uint8_t>(0xA2);  //10100010 (color table, bpp = 3)
  stream << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00);

  // GifColorTable: 8 entries = 24 bytes
  for( uint8_t i = 0; i < 8; ++i )
    stream << static_cast<uint8_t>(i) << static_cast<uint8_t>(i) << static_cast<uint8_t>(i);

  // GifApplicationExt
  stream << static_cast<uint8_t>(0x21);  // ID
  stream << static_cast<uint8_t>(0xFF);  // label
  stream << static_cast<uint8_t>(0x0B);  // size
  stream << "NETSCAPE2.0";
  stream << static_cast<uint8_t>(0x03);  // sub-block size
  stream << static_cast<uint8_t>(0x01);
  stream << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00);  // sub-block terminator

  // image #1

  // GifGraphicsControlExt
  stream << static_cast<uint8_t>(0x21);  // ID
  stream << static_cast<uint8_t>(0xF9);  // label
  stream << static_cast<uint8_t>(0x04);  // sub-block size
  stream << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00);  // sub-block terminator

  // GifImageDescriptor
  stream << static_cast<uint8_t>(0x2C);  // ID
  stream << static_cast<uint8_t>(0x00) <<  static_cast<uint8_t>(0x00);  // left
  stream << static_cast<uint8_t>(0x00) <<  static_cast<uint8_t>(0x00);  // top
  stream << static_cast<uint8_t>(0x02) <<  static_cast<uint8_t>(0x00);  // width
  stream << static_cast<uint8_t>(0x03) <<  static_cast<uint8_t>(0x00);  // height
  stream << static_cast<uint8_t>(0x00);  // packed byte

  // GifImageData: 3,3,3,3,3,3 -> 8,3,10,11,9 (4 bits/code)
  stream << static_cast<uint8_t>(0x03);  // bpp
  stream << static_cast<uint8_t>(0x03);  // sub-block size
  stream << static_cast<uint8_t>(0x38);  // codes: 8,3
  stream << static_cast<uint8_t>(0xBA);  // codes: 10,11
  stream << static_cast<uint8_t>(0x09);  // code:  9
  stream << static_cast<uint8_t>(0x00);  // sub-block ends

  // image #2

  // GifGraphicsControlExt
  stream << static_cast<uint8_t>(0x21);  // ID
  stream << static_cast<uint8_t>(0xF9);  // label
  stream << static_cast<uint8_t>(0x04);  // sub-block size
  stream << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00) << static_cast<uint8_t>(0x00);
  stream << static_cast<uint8_t>(0x00);  // sub-block terminator

  // GifImageDescriptor
  stream << static_cast<uint8_t>(0x2C);  // ID
  stream << static_cast<uint8_t>(0x00) <<  static_cast<uint8_t>(0x00);  // left
  stream << static_cast<uint8_t>(0x00) <<  static_cast<uint8_t>(0x00);  // top
  stream << static_cast<uint8_t>(0x02) <<  static_cast<uint8_t>(0x00);  // width
  stream << static_cast<uint8_t>(0x03) <<  static_cast<uint8_t>(0x00);  // height
  stream << static_cast<uint8_t>(0x00);  // packed byte

  // GifImageData: 5,5,5,5,5,5 -> 8,5,10,11,9 (4 bits/code)
  stream << static_cast<uint8_t>(0x03);  // bpp
  stream << static_cast<uint8_t>(0x03);  // sub-block size
  stream << static_cast<uint8_t>(0x58);  // codes: 8,5
  stream << static_cast<uint8_t>(0xBA);  // codes: 10,11
  stream << static_cast<uint8_t>(0x09);  // code:  9
  stream << static_cast<uint8_t>(0x00);  // sub-block ends

  // Gif Trailer
  stream << GifImpl::m_GifTrailer;

  // verify size
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 105 );

  GifImpl gifimpl;

  // read from stream
  // simplified version of GifImpl::Import()
  CPPUNIT_ASSERT( gifimpl.Read( stream ) == GifImpl::m_GifTrailer );
  gifimpl.m_ImageVec = GifImageVecBuilder()( gifimpl, gifimpl.m_ComponentVec );
  CPPUNIT_ASSERT( gifimpl.m_ImageVec.size() == 2 );

  // test GifImpl methods
  CPPUNIT_ASSERT( gifimpl.Width() == 2 );
  CPPUNIT_ASSERT( gifimpl.Height() == 3 );

  CPPUNIT_ASSERT( gifimpl.ColorTable() );
  CPPUNIT_ASSERT( gifimpl.ColorTableSize() == 8 );
  uint8_t R, G, B;
  gifimpl.GetColorTable( 3, R, G, B );
  CPPUNIT_ASSERT( R == 3 );
  CPPUNIT_ASSERT( G == 3 );
  CPPUNIT_ASSERT( B == 3 );

  CPPUNIT_ASSERT( gifimpl.Images() == 2 );

  // image #1
  vp::GifImage& image = gifimpl[0];
  CPPUNIT_ASSERT( image.Left() == 0 );
  CPPUNIT_ASSERT( image.Top() == 0 );
  CPPUNIT_ASSERT( image.Width() == 2 );
  CPPUNIT_ASSERT( image.Height() == 3 );
  CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 3 );
  CPPUNIT_ASSERT( image.Interlaced() == false );
  CPPUNIT_ASSERT( image.Delay() == 0 );
  image.Delay( 100 );
  CPPUNIT_ASSERT( image.Delay() == 100 );
  CPPUNIT_ASSERT( image.ColorTable() == false );

  // image #2
  image = gifimpl[1];
  CPPUNIT_ASSERT( image.Left() == 0 );
  CPPUNIT_ASSERT( image.Top() == 0 );
  CPPUNIT_ASSERT( image.Width() == 2 );
  CPPUNIT_ASSERT( image.Height() == 3 );
  CPPUNIT_ASSERT( image.GetPixel( 1, 1 ) == 5 );
  CPPUNIT_ASSERT( image.Interlaced() == false );
  CPPUNIT_ASSERT( image.Delay() == 0 );
  image.Delay( 200 );
  CPPUNIT_ASSERT( image.Delay() == 200 );
  CPPUNIT_ASSERT( image.ColorTable() == false );

  // exceed index range: 0 <= index < size
  CPPUNIT_ASSERT_THROW( gifimpl[-1], vp::Exception );
  CPPUNIT_ASSERT_THROW( gifimpl[2], vp::Exception );
}

void GifImplTest::testWriteOneImage()
{
  GifImpl gifimpl( 2, 2, 3 );
  std::stringstream stream;
  gifimpl.Write( stream );
  std::string str = stream.str();

  // GifHeader
  CPPUNIT_ASSERT( str.substr( 0, 6 ) == "GIF89a" );

  // GifScreenDescriptor
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0x02 );  // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0x03 );  // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[9]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[10]) == 0x91 ); //color table, bpp=2, color table size 2^2
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[11]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[12]) == 0x00 );

  // GifColorTable: 4 entries = 12 bytes
  for( uint8_t i = 1; i <= 12; ++i )
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i+12]) == 0xFF );

  // GifImageDescriptor
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[25]) == 0x2C );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[26]) == 0x00 );  // left
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[27]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[28]) == 0x00 );  // top
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[29]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[30]) == 0x02 );  // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[31]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[32]) == 0x03 );  // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[33]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[34]) == 0x00 );  // packed byte

  // GifImageData: 0,0,0,0,0,0 -> 4,0,6,7,5 (3 bits/code)
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[35]) == 0x02 );  // bpp
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[36]) == 0x02 );  // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[37]) == 0x84 );  // codes: 4,0,6,7,5
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[38]) == 0x5F ); 
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[39]) == 0x00 );  // sub-block ends

  // GifCommentExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[40]) == 0x21 );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[41]) == 0xFE );  // label
  uint8_t size = static_cast<uint8_t>(str[42]);             // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[42+size+1]) == 0x00 ); // sub-block ends

  // Gif Trailer
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[42+size+2]) == GifImpl::m_GifTrailer );
}

void GifImplTest::testWriteTwoImages()
{
  GifImpl gif( 3, 2, 3, 2 );
  std::stringstream stream;
  gif.Write( stream );
  std::string str = stream.str();

  // GifHeader
  CPPUNIT_ASSERT( str.substr( 0, 6 ) == "GIF89a" );

  // GifScreenDescriptor
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0x02 ); // width
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[8]) == 0x03 ); // height
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[9]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[10]) == 0xA2 ); //color table, bpp=3, color table size 2^3
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[11]) == 0x00 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[12]) == 0x00 );

  // GifColorTable: 8 entries = 24 bytes
  for( uint8_t i = 1; i <= 24; ++i )
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i+12]) == 0xFF );

  // GifApplicationExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[37]) == 0x21 ); // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[38]) == 0xFF ); // label
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[39]) == 0x0B ); // size = 11
  CPPUNIT_ASSERT( str.substr( 40, 11 ) == "NETSCAPE2.0" );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[51]) == 0x03 ); // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[55]) == 0x00 ); // sub-block terminator

  // image #1

  // GifGraphicsControlExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[56]) == 0x21 ); // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[57]) == 0xF9 ); // label
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[58]) == 0x04 ); // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[63]) == 0x00 ); // sub-block terminator

  // GifImageDescriptor
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[64]) == 0x2C );  // ID

  // GifImageData: 0,0,0,0,0,0 -> 8,0,10,11,9 (4 bits/code)
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[74]) == 0x03 );  // bpp
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[75]) == 0x03 );  // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[76]) == 0x08 );  // codes: 8,0
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[77]) == 0xBA );  // codes: 10,11
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[78]) == 0x09 );  // code:  9
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[79]) == 0x00 );  // sub-block ends

  // image #2

  // GifGraphicsControlExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[80]) == 0x21 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[81]) == 0xF9 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[82]) == 0x04 ); // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[87]) == 0x00 ); // sub-block terminator

  // GifImageDescriptor
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[88]) == 0x2C );  // ID

  // GifImageData: 0,0,0,0,0,0 -> 8,0,10,11,9 (4 bits/code)
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[98]) == 0x03 );   // bpp
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[99]) == 0x03 );   // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[100]) == 0x08 );  // codes: 8,0
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[101]) == 0xBA );  // codes: 10,11
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[102]) == 0x09 );  // code:  9
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[103]) == 0x00 );  // sub-block ends

  // GifCommentExt
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[104]) == 0x21 );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[105]) == 0xFE );  // label
  uint8_t size = static_cast<uint8_t>(str[106]);             // sub-block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[106+size+1]) == 0x00 ); // sub-block ends

  // Gif Trailer
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[106+size+2]) == GifImpl::m_GifTrailer );
}
