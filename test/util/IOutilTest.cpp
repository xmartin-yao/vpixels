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

#include "IOutilTest.h"
#include "IOutil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( IOutilTest );

void IOutilTest::setUp()
{
  // empty stream
  strstream.clear();
  strstream.str(std::string());
}

// sign bit not set
void IOutilTest::testRead16()
{  
  strstream << static_cast<char>(0x70) << static_cast<char>(0x17);
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x70 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x17 );

  uint16_t u16; 
  IOutil::Read( strstream, u16 );
  CPPUNIT_ASSERT( u16 == 6000  );

  strstream.seekg(0);
  int16_t i16;
  IOutil::Read( strstream, i16 );
  CPPUNIT_ASSERT( i16 == 6000  );
}

// sign bit set
void IOutilTest::testRead16SignBit()
{  
  strstream << static_cast<uint8_t>(0x38) << static_cast<uint8_t>(0xFF);
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x38 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xFF );

  uint16_t u16; 
  IOutil::Read( strstream, u16 );
  CPPUNIT_ASSERT( u16 == 65336 );

  strstream.seekg(0);
  int16_t i16;
  IOutil::Read( strstream, i16 );
  CPPUNIT_ASSERT( i16 == -200  );
}

void IOutilTest::testWrite16Unsiged()
{
  uint16_t u16 = 0xF1F2;
  IOutil::Write( strstream, u16 );

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0xF2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xF1 );
}

void IOutilTest::testWrite16()
{
  int16_t i16 = 6000;
  IOutil::Write( strstream, i16 );

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x70 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x17 );
}

// negative number
void IOutilTest::testWrite16Negative()
{
  int16_t i16 = -200;
  IOutil::Write( strstream, i16 );

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x38 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xFF );
}

// sign bit not set
void IOutilTest::testRead32()
{
  strstream << static_cast<char>(0x40) << static_cast<char>(0x4B)
            << static_cast<char>(0x4C) << static_cast<char>(0x00);
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x40 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x4B );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0x4C );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0x00 );

  uint32_t u16; 
  IOutil::Read( strstream, u16 );
  CPPUNIT_ASSERT( u16 == 5000000  );

  strstream.seekg(0);
  int32_t i16;
  IOutil::Read( strstream, i16 );
  CPPUNIT_ASSERT( i16 == 5000000  );
}

// sign bit set
void IOutilTest::testRead32SignBit()
{
  strstream << static_cast<uint8_t>(0xC0) << static_cast<uint8_t>(0xB4)
            << static_cast<uint8_t>(0xB3) << static_cast<uint8_t>(0xFF);
  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0xC0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xB4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0xB3 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0xFF );

  uint32_t u32; 
  IOutil::Read( strstream, u32 );
  CPPUNIT_ASSERT( u32 == 4289967296 );

  strstream.seekg(0);
  int32_t i32;
  IOutil::Read( strstream, i32 );
  CPPUNIT_ASSERT( i32 == -5000000  );
}

void IOutilTest::testWrite32Unsiged()
{
  uint32_t u32 = 0xF1F2A1A2;
  IOutil::Write( strstream, u32 );

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0xA2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xA1 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0xF2 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0xF1 );
}

void IOutilTest::testWrite32()
{
  int32_t i32 = 5000000;
  IOutil::Write( strstream, i32 );

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x40 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0x4B );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0x4C );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0x00 );
}

// negative number
void IOutilTest::testWrite32Negative()
{
  int32_t i32 = -5000000;
  IOutil::Write( strstream, i32 );

  std::string str = strstream.str();
  CPPUNIT_ASSERT( str.size() == 4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0xC0 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xB4 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 0xB3 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 0xFF );
}
