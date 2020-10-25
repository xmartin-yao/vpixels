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

#ifndef IOutil_h
#define IOutil_h

#include <cstdint>
#include <type_traits>
#include <iostream>

////////////////
// I/O utilities
/////////////////////////
namespace IOutil 
{
  inline void Write( std::ostream& os, const uint8_t& i8 )
  {
    os.put( static_cast<char>(i8) );
  }

  ////////////////////////////
  // Read uint16_t or int16_t from stream, little-endian
  ////////////////////////////////////////////////////////
  template<typename T> 
  typename std::enable_if<std::is_same<T,uint16_t>::value ||
                          std::is_same<T,int16_t>::value>::type
  Read( std::istream& is, T& i16 )
  {
    uint8_t bytes[2] = {}; 
    is.read( reinterpret_cast<char*>(bytes), 2 );

    i16 = static_cast<T>((bytes[1] << 8) | bytes[0]);
  }

  /////////////////////////////
  // Write uint16_t or int16_t into stream, little-endian
  /////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint16_t>::value ||
                          std::is_same<T,int16_t>::value>::type
  Write( std::ostream& os, const T& i16 )
  {
    uint8_t bytes[2]; 
    bytes[0] = static_cast<uint8_t>(i16);
    bytes[1] = static_cast<uint8_t>(i16 >> 8);

    os.write( reinterpret_cast<char*>(bytes), 2 );
  }

  /////////////////////////////
  // Read uint32_t or int32_t from stream, little-endian
  ////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint32_t>::value ||
                          std::is_same<T,int32_t>::value>::type
  Read( std::istream& is, T& i32 )
  {
    uint8_t bytes[4] = {}; 
    is.read( reinterpret_cast<char*>(bytes), 4 );

    i32 = static_cast<T>((bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0]);
  }

  /////////////////////////////
  // Write uint32_t or int32_t into stream, little-endian
  /////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint32_t>::value ||
                          std::is_same<T,int32_t>::value>::type
  Write( std::ostream& os, const T& i32 )
  {
    uint8_t bytes[4]; 
    bytes[0] = static_cast<uint8_t>(i32);
    bytes[1] = static_cast<uint8_t>(i32 >> 8);
    bytes[2] = static_cast<uint8_t>(i32 >> 16);
    bytes[3] = static_cast<uint8_t>(i32 >> 24);

    os.write( reinterpret_cast<char*>(bytes), 4 );
  }

} //namespace IOutil  
#endif //IOutil_h
