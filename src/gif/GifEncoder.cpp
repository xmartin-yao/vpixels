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

#include "GifEncoder.h"
#include "GifCodeWriter.h"

///////////////////////////////////////////
// Assume the compliance of the input pixels with the passed Bpp, i.e.
// all pixels have the same bits/pixel rate as defined by the passed Bpp.
// Caller is responsible for initialization of CodeStr.
//////////////////////////////////////////////////////////////////////////////////
void GifEncoder::operator()( const uint8_t Bpp, const U8String& PixelStr, U8String& CodeStr )
{
  // Initialization
  InitConstants( Bpp );
  InitParameters();
  m_StringTable.Reset();
  GifCodeWriter WriteCode( CodeStr );

  // output clear code
  WriteCode( m_ClearCode, m_CodeSize );

  // empty stream
  size_t TotPixels = PixelStr.size();
  if( TotPixels == 0 )
  {
    WriteCode( m_ClearCode + 1, m_CodeSize ); // EOI code
    WriteCode.End();
    return;
  }

  // encode pixels
  size_t Index = 0;
  uint16_t String = PixelStr[Index++];  // read 1st pixel
  while( Index < TotPixels )
  {
    uint8_t Pixel = PixelStr[Index++];  // read a pixel

    uint16_t Entry;
    uint16_t Code = m_StringTable.Search( String, Pixel, Entry );
    if( Code == 0 ) // String + Pixel not in string table
    {
      WriteCode( String, m_CodeSize );

      if( m_FreeCode <= 4095 ) // next code is still within 12 bits
      {
        m_StringTable.Add( Entry, String, Pixel, m_FreeCode );
        UpdateParameters();
      }
      else // next code exceeds 12 bits, string table is full
      {
        // output clear code and start over again
        WriteCode( m_ClearCode, m_CodeSize );
        InitParameters();
        m_StringTable.Reset();
      }

      String = Pixel;
    }
    else // String + Pixel in string table
    {
      String = Code;
    }
  }

  // hit the end of PixelStr
  WriteCode( String, m_CodeSize );
  WriteCode( m_ClearCode + 1, m_CodeSize );  // EOI code
  WriteCode.End();
}

//////////////////////////////////
// Initialize constants that depend on Bpp
//
// Bpp InitCodeSize ClearCode EOICode FirstCode  SizeChange
//  1       2           2        3     4 (3-bit)    4 (when 1st code is added)
//  2       3           4        5     6 (3-bit)    8 (when code 8 is added)
//  3       4           8        9     10(4-bit)   16 (when code 16 is added)
//  4       5          16       17     18(5-bit)   32 (when code 32 is added)
//  5       6          32       33     34(6-bit)   64 (when code 64 is added)
//  6       7          64       65     66(7-bit)  128 (when code 128 is added)
//  7       8         128      129    130(8-bit)  256 (when code 256 is added)
//  8       9         256      257    258(9-bit)  512 (when code 512 is added)
// EOICode = ClearCode + 1
// FirstCode = EOICode + 1 = ClearCode + 2
////////////////////////////////////////////////////////
void GifEncoder::InitConstants( const uint8_t Bpp )
{
  m_InitCodeSize = Bpp + 1;  // in bits
  m_ClearCode = 1 << Bpp; 
}

/////////////////////
// initialize dynamic parameters that changes during encoding
///////////////////////////////////////////////////////
void GifEncoder::InitParameters()
{
  m_CodeSize  = m_InitCodeSize;  // in bits
  m_CodeLimit = 1 << m_InitCodeSize;
  m_FreeCode  = m_ClearCode + 2;
}

/////////////////////
// update dynamic parameters
/////////////////////////////////
void GifEncoder::UpdateParameters()
{
  ++m_FreeCode;
  // if next code exceeds current limit, but is still
  // less than 12 bits, increase code size by 1 bit.
  if( m_FreeCode > m_CodeLimit && m_CodeSize < 12 )
  {
     ++m_CodeSize;
     m_CodeLimit <<= 1;
  }  
}
