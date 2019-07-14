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

#ifndef BmpColorTable_h
#define BmpColorTable_h

#include <cstdint>
#include <iosfwd>
#include <memory>

//////////////////////////
class BmpColorTable  
{
public:
  explicit BmpColorTable( uint16_t Size );
  BmpColorTable( const BmpColorTable& );

  ~BmpColorTable() = default;

  // don't need them
  BmpColorTable( BmpColorTable&& ) = delete;
  BmpColorTable& operator=( const BmpColorTable& ) = delete;
  BmpColorTable& operator=( BmpColorTable&& ) = delete;

  uint16_t Size() const { return m_ArraySize/4; }
  void     Size( uint16_t Size );

  void Set( const uint8_t Index, const uint8_t Blue, const uint8_t Green, const uint8_t Red );
  void Get( const uint8_t Index, uint8_t& Blue, uint8_t& Green, uint8_t& Red ) const;

  friend std::ostream& operator<<( std::ostream&, const BmpColorTable& );
  friend std::istream& operator>>( std::istream&, BmpColorTable& );

private:
  // each entry of color table consists of four bytes, three of them are in use.
  // maximum number of entries == 256, when bpp = 8.
  // m_ArraySize == 4*entries, so use uint16_t
  uint16_t m_ArraySize;
  std::unique_ptr<uint8_t[]> m_ByteArray; 
};

#endif //BmpColorTable_h
