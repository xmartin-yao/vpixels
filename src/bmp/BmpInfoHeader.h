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

#ifndef BmpInfoHeader_h 
#define BmpInfoHeader_h

#include <cstdint>
#include <iosfwd>

// forward
class BmpInfo;


////////////////////
class BmpInfoHeader  
{
public:
  explicit BmpInfoHeader( const BmpInfo& );
  BmpInfoHeader( const BmpInfoHeader& );

  ~BmpInfoHeader() = default;

  // don't need them
  BmpInfoHeader( BmpInfoHeader&& ) = delete;
  BmpInfoHeader& operator=( const BmpInfoHeader& ) = delete;
  BmpInfoHeader& operator=( BmpInfoHeader&& ) = delete;

  explicit operator bool() const;
  bool Check( BmpInfo & ) const;

  uint8_t  BitsPerPixel() const { return static_cast<uint8_t>(m_BitsPerPixel); }
  int32_t  Width()  const { return m_Width; }
  int32_t  Height() const { return m_Height; }
  uint32_t ImageDataSize() const { return m_ImageDataSize; }

  friend std::ostream& operator<<( std::ostream&, const BmpInfoHeader& );
  friend std::istream& operator>>( std::istream&, BmpInfoHeader& );

private:
  // elements of struct BITMAPINFOHEADER 
  uint32_t m_Size;             // size of BITMAPINFOHEADER, 40 bytes
  int32_t  m_Width;            // width of image, pixels
  int32_t  m_Height;           // height of image, pixels
  uint16_t m_Planes;           // number of color planes, always 1
  uint16_t m_BitsPerPixel;     // BITMAPINFOHEADER uses 16 bits for this
  uint32_t m_Compression;      // compression type, 0 means no compression
  uint32_t m_ImageDataSize;    // image data size, bytes
  int32_t  m_Xresolution;      // pixels per meter
  int32_t  m_Yresolution;      // pixels per meter
  uint32_t m_ColorsUsed;       // number of colors used. 0 means full color set
  uint32_t m_ImportantColors;  // important colors. 0 means all colors important
};

/*
struct BITMAPINFOHEADER {
  DWORD  Size;
  LONG   Width;
  LONG   Height;
  WORD   Planes;
  WORD   BitCount;
  DWORD  Compression;
  DWORD  SizeImage;
  LONG   XPelsPerMeter;
  LONG   YPelsPerMeter;
  DWORD  ClrUsed;
  DWORD  ClrImportant;
};
*/

#endif //BmpInfoHeader_h
