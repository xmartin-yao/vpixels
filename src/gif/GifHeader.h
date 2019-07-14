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

#ifndef GifHeader_h
#define GifHeader_h

#include <cstdint>
#include <iosfwd>

///////////////////////////
enum class GifVersion : uint8_t { V87a, V89a, Invalid };

////////////////////
class GifHeader
{
public:
  explicit GifHeader( GifVersion );
  GifHeader( const GifHeader& );

  ~GifHeader() = default;

  // not implemented
  GifHeader( GifHeader&& ) = delete;
  GifHeader& operator=( const GifHeader& ) = delete;
  GifHeader& operator=( GifHeader&& ) = delete;

  explicit operator bool() const;
  GifVersion Version() const; 

  friend std::ostream& operator<<( std::ostream&, const GifHeader& );
  friend std::istream& operator>>( std::istream&, GifHeader& );

private:
  char m_Signature[3]; 
  char m_Version[3];
};

#endif //GifHeader_h 
