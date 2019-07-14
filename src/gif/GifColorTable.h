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

#ifndef GifColorTable_h
#define GifColorTable_h

#include <cstdint>
#include <iosfwd>
#include <memory>


////////////////////////////////////
class GifColorTable  
{
public:
  explicit GifColorTable( uint16_t Size );
  GifColorTable( const GifColorTable& );
  GifColorTable& operator=( const GifColorTable& );

  ~GifColorTable() = default;

  // not implemented
  GifColorTable() = delete;
  GifColorTable( GifColorTable&& ) = delete;
  GifColorTable& operator=( GifColorTable&& ) = delete;

  uint16_t Size() const { return m_ArraySize/3; }
  void     Size( uint16_t Size );
  void     Size( uint16_t Size, uint8_t& PackedByte );

  void Set( const uint8_t Index, const uint8_t Red, const uint8_t Green, const uint8_t Blue );
  void Get( const uint8_t Index, uint8_t& Red, uint8_t& Green, uint8_t& Blue ) const;

  friend std::ostream& operator<<( std::ostream&, const GifColorTable& );
  friend std::istream& operator>>( std::istream&, GifColorTable& );

  // utils
  static uint8_t Roundup( uint16_t Size );

private:
  // each entry of color table consists of three bytes.
  // maximum number of entries == 256, when bpp = 8.
  // m_ArraySize == 3*entries, so use uint16_t
  uint16_t m_ArraySize;
  std::unique_ptr<uint8_t[]> m_ByteArray; 
};

#endif //GifColorTable_h
