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

#include "GifBlockIOTest.h"
#include "GifBlockIO.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifBlockIOTest );

void GifBlockIOTest::setUp()
{
  // empty stream
  strstream.clear();
  strstream.str(std::string());
}

void GifBlockIOTest::testReadSubBlocks()
{
  // 1st block
  uint16_t i = 0;
  strstream << static_cast<uint8_t>(0xFF);  // size 255
  while( ++i <= 255 )
    strstream << static_cast<uint8_t>(0xF1);

  // 2nd block
  i = 0;
  strstream << static_cast<uint8_t>(0xFF);  // size 255
  while( ++i <= 255 )
    strstream << static_cast<uint8_t>(0xF1);

  // 3rd block
  strstream << static_cast<uint8_t>(0x02);  // size 2
  strstream << static_cast<uint8_t>(0xF1);
  strstream << static_cast<uint8_t>(0xF1);
  strstream << static_cast<uint8_t>(0x00);  // end

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 516 );  // 512 + 4

  U8String bytes;
  GifBlockIO::ReadSubBlocks( strstream, bytes );
  CPPUNIT_ASSERT( bytes.size() == 512 );
  for( i = 0; i < 512; ++i )
    CPPUNIT_ASSERT( bytes[i] == 0xF1 );
}

void GifBlockIOTest::testWriteSubBlocks()
{
  U8String bytes;
  uint16_t i = 0;
  while( ++i <= 512 ) bytes += 0xA1;
  CPPUNIT_ASSERT( bytes.size() == 512 ); 

  GifBlockIO::WriteSubBlocks( strstream, bytes );
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 516 );  // 512 + 4

  // 1st block
  i = 0;
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[i]) == 0xFF );
  while( ++i <= 255 )
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i]) == 0xA1 );

  // 2nd block
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[i]) == 0xFF );
  while( ++i <= 511 )
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i]) == 0xA1 );

  // 3rd block
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[512]) == 0x02 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[513]) == 0xA1 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[514]) == 0xA1 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[515]) == 0x00 );
}

void GifBlockIOTest::testReadSubBlocksChars()
{
  strstream << static_cast<uint8_t>(0x1B);     // block size
  strstream << "A string consists of chars.";  // string
  strstream << static_cast<uint8_t>(0x00);     // terminator

  std::string chars;
  GifBlockIO::ReadSubBlocks( strstream, chars );
  CPPUNIT_ASSERT( chars.size() == 0x1B );
  CPPUNIT_ASSERT( chars == "A string consists of chars." );
}

void GifBlockIOTest::testWriteSubBlocksChars()
{
  std::string chars( "A string consists of chars." );
  CPPUNIT_ASSERT( chars.size() == 27 ); 

  GifBlockIO::WriteSubBlocks( strstream, chars );
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 29 );  // 27 + 2
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x1B );
  CPPUNIT_ASSERT( str.substr(1, 27) == "A string consists of chars." );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[28]) == 0x00 );
}

void GifBlockIOTest::testReadBlock()
{
  strstream << static_cast<uint8_t>(0xFF);  // size 255
  uint16_t i = 0;
  while( ++i <= 255 ) strstream << static_cast<uint8_t>(0x1A);
  
  U8String bytes;
  GifBlockIO::ReadBlock( strstream, bytes );
  CPPUNIT_ASSERT( bytes.size() == 255 );
  for( i = 0; i < 255; ++i )
    CPPUNIT_ASSERT( bytes[i] == 0x1A );
}

void GifBlockIOTest::testWriteBlock()
{
  U8String bytes;
  uint16_t i = 0;
  while( ++i <= 255 ) bytes += 0xB1;
  CPPUNIT_ASSERT( bytes.size() == 255 ); 

  GifBlockIO::WriteBlock( strstream, bytes );
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 256 ); // 255 + size byte

  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0xFF ); // size
  i = 0;
  while( ++i < 256 )
    CPPUNIT_ASSERT( static_cast<uint8_t>(str[i]) == 0xB1 );
}

void GifBlockIOTest::testReadBlockChars()
{
  strstream << static_cast<uint8_t>(27);     // block size
  strstream << "A string consists of chars.";  // string

  std::string chars;
  GifBlockIO::ReadBlock( strstream, chars );
  CPPUNIT_ASSERT( chars.size() == 27 );
  CPPUNIT_ASSERT( chars == "A string consists of chars." );
}

void GifBlockIOTest::testWriteBlockChars()
{
  std::string chars( "A string consists of chars." );
  CPPUNIT_ASSERT( chars.size() == 27 ); 

  GifBlockIO::WriteBlock( strstream, chars );
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 28 );  // 27 + 1 ( size byte)
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x1B );
  CPPUNIT_ASSERT( str.substr(1, 27) == "A string consists of chars." );
}
