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

#ifndef BmpFileHeader_h
#define BmpFileHeader_h

#include <cstdint>
#include <iosfwd>

// forward
class BmpInfo;


/////////////////////////////
class BmpFileHeader  
{
public:
  explicit BmpFileHeader( const BmpInfo& );
  BmpFileHeader( const BmpFileHeader& );

  ~BmpFileHeader() = default;

  // don't need them
  BmpFileHeader( BmpFileHeader&& ) = delete;
  BmpFileHeader& operator=( const BmpFileHeader& ) = delete;
  BmpFileHeader& operator=( BmpFileHeader&& ) = delete;

  explicit operator bool() const;
  bool Check( const BmpInfo& ) const;

  uint32_t FileSize() const    { return m_Filesize; }
  uint32_t ImageOffset() const { return m_ImageOffset; }

  friend std::ostream& operator<<( std::ostream&, const BmpFileHeader& );
  friend std::istream& operator>>( std::istream&, BmpFileHeader& );

private:
  // elements of struct BITMAPFILEHEADER
  uint16_t m_Identifier;
  uint32_t m_Filesize;     // File size, in bytes 
  uint16_t m_Reserved1;
  uint16_t m_Reserved2;
  uint32_t m_ImageOffset;  // Offset to image data, in bytes

  static const uint16_t m_BmpID = 0x4D42;  // "MB";
};

/*
struct BITMAPFILEHEADER {
  WORD   Type;
  DWORD  Size;
  WORD   Reserved1;
  WORD   Reserved2;
  DWORD  OffBits;
};
*/

#endif //BmpFileHeader_h
