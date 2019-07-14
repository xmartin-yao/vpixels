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

#include "GifGraphicsControlExtTest.h"
#include "GifGraphicsControlExt.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifGraphicsControlExtTest );

void GifGraphicsControlExtTest::testCtors()
{
  // ctor
  GifGraphicsControlExt gc1;
  CPPUNIT_ASSERT( gc1.ID() == 0x21 ); 
  CPPUNIT_ASSERT( gc1.Label() == 0xF9 ); 
  CPPUNIT_ASSERT( gc1.DisposalMethod() == 1 ); 
  CPPUNIT_ASSERT( gc1.UserInput() == false ); 
  CPPUNIT_ASSERT( gc1.HasTransColor() == false ); 
  CPPUNIT_ASSERT( gc1.TransColor() == 0 ); 
  CPPUNIT_ASSERT( gc1.Delay() == 0 );
  gc1.Delay( 522 );
  CPPUNIT_ASSERT( gc1.Delay() == 522 );

  // copy ctor
  GifGraphicsControlExt gc2 = gc1;
  CPPUNIT_ASSERT( gc2.ID() == 0x21 ); 
  CPPUNIT_ASSERT( gc2.Label() == 0xF9 ); 
  CPPUNIT_ASSERT( gc2.DisposalMethod() == 1 ); 
  CPPUNIT_ASSERT( gc2.UserInput() == false ); 
  CPPUNIT_ASSERT( gc2.HasTransColor() == false ); 
  CPPUNIT_ASSERT( gc2.TransColor() == 0 ); 
  CPPUNIT_ASSERT( gc2.Delay() == 522 );

  // assignment operator
  GifGraphicsControlExt gc3;
  CPPUNIT_ASSERT( gc3.Delay() == 0 );
  gc3 = gc1;
  CPPUNIT_ASSERT( gc3.ID() == 0x21 ); 
  CPPUNIT_ASSERT( gc3.Label() == 0xF9 ); 
  CPPUNIT_ASSERT( gc3.DisposalMethod() == 1 ); 
  CPPUNIT_ASSERT( gc3.UserInput() == false ); 
  CPPUNIT_ASSERT( gc3.HasTransColor() == false ); 
  CPPUNIT_ASSERT( gc3.TransColor() == 0 ); 
  CPPUNIT_ASSERT( gc3.Delay() == 522 );
}

void GifGraphicsControlExtTest::testOutput()
{
  GifGraphicsControlExt gc;
  gc.Delay( 500 );

  std::stringstream stream;
  gc.Write( stream );
  std::string str = stream.str();
  CPPUNIT_ASSERT( str.size() == 8 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[0]) == 0x21 );  // ID
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[1]) == 0xF9 );  // label
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[2]) == 4 );     // block size
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[3]) == 4 );     // packed byte
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[4]) == 0xF4 );  // delay time
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[5]) == 0x01 );
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[6]) == 0 );     // transparent color
  CPPUNIT_ASSERT( static_cast<uint8_t>(str[7]) == 0 );     // terminator
}

void GifGraphicsControlExtTest::testInput()
{
  std::stringstream stream;
  stream << static_cast<uint8_t>(0x21);  // ID 
  stream << static_cast<uint8_t>(0xF9);  // label 
  stream << static_cast<uint8_t>(4);     // block size
  stream << static_cast<uint8_t>(0x0B);  // packed byte 
  stream << static_cast<uint8_t>(0xDC);  // delay time
  stream << static_cast<uint8_t>(0x05); 
  stream << static_cast<uint8_t>(1);     // transparent color
  stream << static_cast<uint8_t>(0); 

  GifGraphicsControlExt gc;
  CPPUNIT_ASSERT( stream.get() == 0x21 );  // ID
  CPPUNIT_ASSERT( stream.get() == 0xF9 );  // label
  gc.Read( stream );
  CPPUNIT_ASSERT( gc.DisposalMethod() == 2 ); 
  CPPUNIT_ASSERT( gc.UserInput() == 1 ); 
  CPPUNIT_ASSERT( gc.HasTransColor() ); 
  CPPUNIT_ASSERT( gc.TransColor() == 1 ); 
  CPPUNIT_ASSERT( gc.Delay() == 1500 );
}
