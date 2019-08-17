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

#ifndef GifImageData_h
#define GifImageData_h

#include <cstdint>
#include <iosfwd>

#include "U8String.h"

//////////////////
class GifImageData
{
public:
  GifImageData( const uint8_t BitsPerPixel = 2, const uint32_t Size = 0 );
  GifImageData( const GifImageData& other );
  GifImageData& operator=( const GifImageData& other );

  ~GifImageData() = default;

  // not implemented
  GifImageData( GifImageData&& ) = delete;
  GifImageData& operator=( GifImageData&& ) = delete;

  // bpp
  uint8_t BitsPerPixel() const { return m_BitsPerPixel; }
  void    BitsPerPixel( const uint8_t Bpp ) { m_BitsPerPixel = Bpp; }

  // size
  size_t Size() const { return m_Pixels.size(); }

  // pixels
  void    SetAllPixels( const uint8_t ColorIndex );
  void    SetPixel( const uint32_t Index, const uint8_t ColorIndex );
  uint8_t GetPixel( const uint32_t Index ) const;

  // IO
  void Init( size_t Capacity );
  friend std::istream& operator>>( std::istream&, GifImageData& );
  friend std::ostream& operator<<( std::ostream&, const GifImageData& );

private:
  uint8_t  m_BitsPerPixel;
  U8String m_Pixels;
};

#endif //GifImageData_h
