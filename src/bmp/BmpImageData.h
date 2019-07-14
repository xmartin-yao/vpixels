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

#ifndef BmpImageData_h
#define BmpImageData_h

#include <cstdint>
#include <iosfwd>
#include <memory>

class BmpInfo;


///////////////////
class BmpImageData  
{
public:
  explicit BmpImageData( const BmpInfo& );
  BmpImageData( const BmpImageData& );

  ~BmpImageData() = default;

  // not implemented
  BmpImageData( BmpImageData&& ) = delete;
  BmpImageData& operator=( const BmpImageData& ) = delete;
  BmpImageData& operator=( BmpImageData&& ) = delete;

  void Init( const BmpInfo& );

  uint32_t Size() const { return m_Size; }
  uint8_t& operator[]( size_t Index ) const;

  friend std::ostream& operator<<( std::ostream&, const BmpImageData& );
  friend std::istream& operator>>( std::istream&, BmpImageData& );

private:
  uint32_t m_Size;   // size of m_ByteArray
  std::unique_ptr<uint8_t[]> m_ByteArray; 
  uint32_t m_RowLength;  // bytes per row
  int32_t  m_Height;     // image height (in pixels)
};

#endif //BmpImageData_h
