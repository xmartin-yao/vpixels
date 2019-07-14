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

#include "GifDecoder.h"
#include "GifCodeReader.h"
#include "Exception.h"

////////////////////////
// Decode codes into pixel bytes.
// Caller is responsible for initialization of PixelStr.
//////////////////////////////////////////////////////////////////////////////
void GifDecoder::operator()( const uint8_t Bpp, const U8String& CodeStr, U8String& PixelStr )
{
  // initialization
  InitConstants( Bpp );
  InitParameters();
  GifCodeReader ReadCode( CodeStr );

  // 1st code must be ClearCode
  uint16_t Code = ReadCode( m_CodeSize );
  if( Code != m_ClearCode )
    VP_THROW( "not a clear code, data may be corrupted" );

  // this is not necessary, as 1st code is supposed to be Clear Code,
  // PrevCode will always get initialized, but to calm down warning
  // when be built with g++ -O2
  uint16_t PrevCode = Code;

  uint16_t EOICode = m_ClearCode + 1;  // End-Of-Information code 
  while( Code != EOICode )
  {
    if( Code == m_ClearCode ) //If it's a clear code
    {
      InitParameters();

      // read a code
      try
      {
        Code = ReadCode( m_CodeSize );    
      }
      catch( const vp::Exception& )
      {
        // hit the end while EOIcode is absent
        Code = EOICode;
      }

      // in case that EOICode follows right after ClearCode
      if( Code == EOICode )
        break;

      // 1st code following ClearCode must be a pixel
      if( Code >= m_ClearCode )
        VP_THROW( "not a pixel, data may be corrupted" );

      OutputPixel( Code, PixelStr );
    } 
    else
    {
      uint8_t FirstPixel;
      if( Code < m_FreeCode ) // if current code is in string table
      {
        FirstPixel = OutputString( Code, PixelStr );
      }
      else
      {
        FirstPixel = OutputString( PrevCode, PixelStr );
        OutputPixel( FirstPixel, PixelStr );
      }

      // this verification is not necessary, if the encoding algorithm
      // is strictly followed. but a few GIF files were found not being
      // encoded so and, in turn, will crash the program without this
      // verification.
      if( m_FreeCode < 4096 )
      {
        // add string + pixel to string table
        m_Prefix[m_FreeCode] = PrevCode;
        m_Suffix[m_FreeCode] = FirstPixel;
        ++m_FreeCode;
      }
    }

    // increase code size
    if( m_FreeCode == m_CodeLimit && m_CodeSize < 12 ) 
    {
      ++m_CodeSize;
      m_CodeLimit <<= 1;
    }

    // next code
    PrevCode = Code;
    try
    {
      Code = ReadCode( m_CodeSize );    
    }
    catch( const vp::Exception& )
    {
      // hit the end while EOIcode is absent
      Code = EOICode;
    }
  }
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
void GifDecoder::InitConstants( const uint8_t Bpp )
{
  m_InitCodeSize = Bpp + 1;  
  m_ClearCode = 1 << Bpp; 
}

/////////////////////////
// Initialize dynamic parameters that change during decoding
//////////////////////////////////////////////////////////
void GifDecoder::InitParameters()
{
  m_CodeSize  = m_InitCodeSize;
  m_CodeLimit = 1 << m_InitCodeSize;
  m_FreeCode  = m_ClearCode + 2;  // initialize string table
}

///////////////////
// Output a pixel to output stream
////////////////////////////////////////////////////////////
void GifDecoder::OutputPixel( const uint16_t Code, U8String& PixelStr )
{
  PixelStr += static_cast<uint8_t>(Code);
}

///////////////////
// Output a string of pixels to output stream.
// Return the 1st pixel in the string that Code represents.
///////////////////////////////////////////////////////////////
uint8_t GifDecoder::OutputString( uint16_t Code, U8String& PixelStr )
{
  if( Code < m_ClearCode )  // Code is a pixel 
  {
    OutputPixel( Code, PixelStr );
  } 
  else  // Code represents a string of pixels
  {
    // decode the string into pixels
    int16_t Index = 0;
    do 
    {
      m_Buffer[Index++] = m_Suffix[Code];
      Code = m_Prefix[Code];
    } while( Code > m_ClearCode );  // until hit a pixel

    m_Buffer[Index] = static_cast<uint8_t>(Code);  // Code now is a pixel

    // output all pixels in the string in reverse order
    while( Index >= 0 )
      OutputPixel( m_Buffer[Index--], PixelStr );
  }

  return static_cast<uint8_t>(Code);  // the 1st pixel
}
