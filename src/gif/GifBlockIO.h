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

#ifndef GifBlockIO_h
#define GifBlockIO_h

#include <iostream>
#include "U8String.h"

// A sub-block is such format that the first byte of the block 
// is the size (in bytes) of data that follows. The size does not
// count the size byte itself. A sub-block may contain from 0 to 255
// bytes of data.
//
// In GIF, compressed data is stored in a contiguous sub-block chain
// which starts with a sub-block of size > 0 until a sub-block of size = 0
// which is called terminating byte.
//
// A block, like a sub-block, is such format that the first byte of 
// the block is the size (in bytes) of data that follows. Sub-blocks
// can form a chain, a block exists by itself, therefore there is no
// terminating byte.
//
// PlainTextExt and ApplicationExt use such a block


namespace GifBlockIO  
{
  ///////////////////////////
  // Read bytes from stream
  // called by ReadSubBlocks() or ReadBlock() when T is uint8_t
  ///////////////////////////////////////////////////////////////
  inline void Read( std::istream& is, uint8_t* buffer, const uint8_t& size )
  {
    is.read( reinterpret_cast<char*>(buffer), size );
  }

  ////////////////////////////
  // Write bytes into stream
  // called by WriteSubBlocks() or WriteBlock() when T is uint8_t
  /////////////////////////////////////////////////////////////////
  inline void Write( std::ostream& os, const uint8_t* buffer, const uint8_t& size )
  {
    os.write( reinterpret_cast<const char*>(buffer), size );
  }

  ///////////////////////////
  // Read chars from stream
  // called by ReadSubBlocks() or ReadBlock() when T is char
  ////////////////////////////////////////////////////////////
  inline void Read( std::istream& is, char* buffer, const uint8_t& size )
  {
    is.read( buffer, size );
  }

  ////////////////////////////
  // Write chars into stream
  // called by WriteSubBlocks() or WriteBlock() when T is char
  //////////////////////////////////////////////////////////////
  inline void Write( std::ostream& os, const char* buffer, const uint8_t& size )
  {
    os.write( buffer, size );
  }

  //////////////////////////////////
  // Read a sub-block chain from stream into U8String or std::string
  ////////////////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint8_t>::value ||
                          std::is_same<T,char>::value>::type
  ReadSubBlocks( std::istream& is, std::basic_string<T>& String )
  {
    String.erase( String.begin(), String.end() );
  
    uint8_t BlockSize = is.get();
    if( BlockSize == 0 )
      return;

    while( BlockSize > 0 )
    {
      T Block[255];
      GifBlockIO::Read( is, Block, BlockSize );
      String.append( Block, BlockSize );
  
      BlockSize = is.get(); 
    }
  }

  //////////////////////////////////
  // Write U8String or std::string into stream as a sub-block chain
  ///////////////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint8_t>::value ||
                          std::is_same<T,char>::value>::type
  WriteSubBlocks( std::ostream& os, const std::basic_string<T>& String )
  {
    // image width and height are of type uint16_t
    // need uint32_t to accommodate the max number of pixels
    uint8_t  BlockSize = 255;
    uint32_t Size = String.size();
    uint32_t Blocks = Size/BlockSize;
    if( (Size % BlockSize) != 0 ) 
      ++Blocks;

    uint32_t Index = 0;
    for( uint32_t i = 1; i <= Blocks; ++i )
    {
      uint32_t Remaining = Size - Index;
      if( Remaining < BlockSize )
        BlockSize = static_cast<uint8_t>(Remaining);
  
      os.put( BlockSize );
      GifBlockIO::Write( os, &String[Index], BlockSize );
  
      Index += BlockSize;
    }

    os.put( 0x00 );  // sub block ends
  }

  //////////////////////////////////
  // Read a block from stream into a string of type U8String or std::string
  ///////////////////////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint8_t>::value ||
                          std::is_same<T,char>::value>::type
  ReadBlock( std::istream& is, std::basic_string<T>& String )
  {
    String.erase( String.begin(), String.end() );
    uint8_t BlockSize = is.get();
    if( BlockSize > 0 )
    {
      T Block[255];
      GifBlockIO::Read( is, Block, BlockSize );
      String.append( Block, BlockSize );
    }
  }

  //////////////////////////////////
  // Write a string of type U8String or std::string into stream as a block
  //////////////////////////////////////////////////////////////////////////
  template<typename T>
  typename std::enable_if<std::is_same<T,uint8_t>::value ||
                          std::is_same<T,char>::value>::type
  WriteBlock( std::ostream& os, const std::basic_string<T>& String )
  {
    uint8_t BlockSize = static_cast<uint8_t>(String.size());
    os.put( BlockSize );
    GifBlockIO::Write( os, &String[0], BlockSize );
  }

} //namespace GifBlockIO 
#endif //GifBlockIO_h
