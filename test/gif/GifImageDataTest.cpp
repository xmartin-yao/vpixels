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

#include <iostream>
#include "GifImageDataTest.h"
#include "GifImageData.h"
#include "GifCodeWriter.h"
#include "Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifImageDataTest );

void GifImageDataTest::testCtors()
{
  uint8_t bpp = 4;
  uint32_t Size = 64;

  // default ctor
  GifImageData id1;
  CPPUNIT_ASSERT( id1.BitsPerPixel() == 2 );
  CPPUNIT_ASSERT( id1.Size() == 0 );

  // ctor
  GifImageData id2( bpp, Size );
  CPPUNIT_ASSERT( id2.BitsPerPixel() == bpp );

  uint32_t Index;
  for( Index = 0; Index < Size; ++Index )
    CPPUNIT_ASSERT( id2.GetPixel(Index) == 0x00 );

  id2.SetAllPixels( 0xF1 );

  for( Index = 0; Index < Size; ++Index )
    CPPUNIT_ASSERT( id2.GetPixel(Index) == 0xF1 );

  // index exceeds limit
  CPPUNIT_ASSERT_THROW( id2.GetPixel( -1 ), vp::Exception );
  CPPUNIT_ASSERT_THROW( id2.GetPixel( Size ), vp::Exception );

  // copy ctor
  GifImageData id3 = id2;
  CPPUNIT_ASSERT( id3.BitsPerPixel() == bpp );
  for( Index = 0; Index < Size; ++Index )
    CPPUNIT_ASSERT( id3.GetPixel(Index) == 0xF1 );

  // assignment
  id1 = id3;
  CPPUNIT_ASSERT( id1.BitsPerPixel() == bpp );
  for( Index = 0; Index < Size; ++Index )
    CPPUNIT_ASSERT( id1.GetPixel(Index) == 0xF1 );
}

// see EncoderTest::testBpp8()
void GifImageDataTest::testOutput()
{
  // prepare GifImageData
  uint8_t bpp = 8;
  uint32_t size = 273;
  GifImageData id( bpp, size );
  uint32_t index = 0;
  uint8_t i, j;
  for( i = 1; i <= 16; ++i )
    for( j = i + 1; j <= 17; ++j )
    {
      id.SetPixel( index++, i ); 
      id.SetPixel( index++, j );
    }

  id.SetPixel( index, 1 ); 
  CPPUNIT_ASSERT( index == size - 1 ); 

  // output GifImageData
  std::stringstream stream;
  stream << id;
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 316 );  // 312 codes + 4 bytes
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == bpp );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 255 );   // 1st block
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[257]) == 57 );  // 2nd block  
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[316]) == 0 );   // terminator
}

// see EncoderTest::testBpp8()
void GifImageDataTest::testInput()
{
  // codes: 256,1,2,1,3,1,4,1,5,...15,16,15,17,16,17,1,257
  U8String Codes;
  GifCodeWriter WriteCode( Codes ); 

  // 9-bit: 256,1,2,1,3,1,4,1,5,...12,16,12,17,13,14,13
  WriteCode( 256, 9 );  // clear code
  uint8_t i, j;
  for( i = 1; i <= 12; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      WriteCode( i, 9 ); 
      WriteCode( j, 9 ); 
    }
  }
  WriteCode( 13, 9 ); 
  WriteCode( 14, 9 ); 
  WriteCode( 13, 9 ); 

  // 10-bit:  15,13,16,13,17,14,15,14,16,14,17,15,16,15,17,16,17,1,257
  WriteCode( 15, 10 ); 
  for( j = 16; j <= 17; ++j )
  {
    WriteCode( 13, 10 ); 
    WriteCode( j, 10 ); 
  }
  for( i = 14; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      WriteCode( i, 10 ); 
      WriteCode( j, 10 ); 
    }
  }
  WriteCode( 1, 10 ); 
  WriteCode( 257, 10 );   // EOI code
  WriteCode.End();
  CPPUNIT_ASSERT( Codes.size() == 312 );

  // prepare stream
  std::stringstream stream;
  stream << static_cast<uint8_t>(8);    // bpp;
  stream << static_cast<uint8_t>(255);  // 1st block
  uint32_t index;
  for( index = 0; index < 255; ++index )
    stream << Codes[index];
  stream << static_cast<uint8_t>(57);   // 2nd block
  for( index = 255; index < 312; ++index )
    stream << Codes[index];
  stream << static_cast<uint8_t>(0);    // terminator

  // input GifImageData
  GifImageData id;
  CPPUNIT_ASSERT( id.BitsPerPixel() == 2 );  // default bpp = 2

  stream >> id;
  CPPUNIT_ASSERT( id.BitsPerPixel() == 8 );

  index = 0;
  for( i = 1; i <= 16; ++i )
  {
    for( j = i + 1; j <= 17; ++j )
    {
      CPPUNIT_ASSERT( id.GetPixel(index++) == i ); 
      CPPUNIT_ASSERT( id.GetPixel(index++) == j ); 
    }
  }

  CPPUNIT_ASSERT( id.GetPixel(index) == 1 ); 
  CPPUNIT_ASSERT( index == 272 ); 
}
