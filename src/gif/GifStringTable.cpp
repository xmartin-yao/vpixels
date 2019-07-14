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

#include "GifStringTable.h"
#include <algorithm>

/////////////////
GifStringTable::GifStringTable()
{
  Reset();
}

//////////////////
void GifStringTable::Reset()
{
  std::fill_n( m_Code, 4096, 0x0000 );
}

//////////////////////
// search for code of String + Pixel
/////////////////////////////////////////////
uint16_t GifStringTable::Search( const uint16_t String, const uint8_t Pixel )
{
  uint16_t Code;
  uint16_t Index = Hash( String, Pixel );
  while( (Code = m_Code[Index]) != 0x0000 )
  {
    if( m_Prefix[Index] == String && m_Suffix[Index] == Pixel )
      return Code;
    else
      Index = Probe( Index );
  }

  // failed to find code
  return 0;
}

///////////////////
// add Code for String + Pixel
/////////////////////////////////////////////////////
void GifStringTable::Add( const uint16_t String, const uint8_t Pixel,
                          const uint16_t Code )
{
  uint16_t Index = Hash( String, Pixel );
  while( m_Code[Index] != 0x0000 )
    Index = Probe( Index );

  m_Prefix[Index] = String;
  m_Suffix[Index] = Pixel;
  m_Code[Index] = Code;
}

//////////////////////
// Search for code of String + Pixel
//
// if String + Pixel are not in table, Index points to an entry
// for adding them and their corresponding Code
//////////////////////////////////////////////////////////////
uint16_t GifStringTable::Search( const uint16_t String, const uint8_t Pixel,
                                 uint16_t& Index )
{
  Index = Hash( String, Pixel );
  uint16_t Code;
  while( (Code = m_Code[Index]) != 0x0000 )
  {
    if( m_Prefix[Index] == String && m_Suffix[Index] == Pixel )
      return Code;
    else
      Index = Probe( Index );
  }

  // failed to find code
  return 0;
}

///////////////////
// Add Code for String + Pixel to the given Index
//
// This function doesn't check the validity of the entry,
// call Search() to get Index.
///////////////////////////////////////////////////////////////
void GifStringTable::Add( const uint16_t Index, const uint16_t String,
                          const uint8_t Pixel, uint16_t Code )
{
  m_Prefix[Index] = String;
  m_Suffix[Index] = Pixel;
  m_Code[Index] = Code;
}
