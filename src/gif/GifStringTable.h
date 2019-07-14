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

#ifndef GifStringTable_h
#define GifStringTable_h

#include <cstdint>

// A hash table used by Encoder to search for string + pixel pair.
// Tried std::unordered_map, performance was worse than that of
// using a brute force search. Reason?
/////////////////////////////////////////////////////////////////
class GifStringTable
{
public:
  GifStringTable();
  ~GifStringTable() = default;

  // not implemented
  GifStringTable( const GifStringTable& ) = delete;
  GifStringTable( GifStringTable&& ) = delete;
  GifStringTable& operator=( const GifStringTable& ) = delete;
  GifStringTable& operator=( GifStringTable&& ) = delete;

  void Reset();

  uint16_t Search( const uint16_t String, const uint8_t Pixel );
  void     Add( const uint16_t String, const uint8_t Pixel, const uint16_t Code );

  // use these two to avoid duplicated calls to Hash() and Probe()
  uint16_t Search( const uint16_t String, const uint8_t Pixel, uint16_t& Index );
  void     Add( const uint16_t Index, const uint16_t String, const uint8_t Pixel,
                const uint16_t Code );

private:
  uint16_t Hash( const uint16_t String, const uint8_t Pixel );
  uint16_t Probe( const uint16_t Index );

  // member data
  uint16_t m_Prefix[4096];  // string
  uint8_t  m_Suffix[4096];  // pixel
  uint16_t m_Code[4096];    // code of string + pixel
};

////////////////////////////////////////
inline uint16_t GifStringTable::Hash( const uint16_t String, const uint8_t Pixel )
{
  uint32_t Key = (String << 8) | Pixel;
  return (Key ^ (Key >> 12)) & 0x0FFF;
}

///////////////////////////////////////
inline uint16_t GifStringTable::Probe( const uint16_t Index )
{
  // linear probing
  return (Index + 601) & 0x0FFF;
}

#endif //GifStringTable_h
