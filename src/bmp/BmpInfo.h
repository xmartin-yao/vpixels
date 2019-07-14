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

#ifndef BmpInfo_h
#define BmpInfo_h

#include <cstdint>
#include <cstddef>  // size_t
#include <memory> 

////////////////////////////
enum class BPP : uint8_t
{ BMP_1_BIT = 1,
  BMP_4_BIT = 4,
  BMP_8_BIT = 8,
  BMP_24_BIT = 24
};

////////////////////////////////
class BmpInfo
{
protected:
  // use factory method to instantiate BmpInfo and its subclasses
  BmpInfo( const BPP bpp, const int32_t Width, const int32_t Height );

public:
  // factory method
  static std::unique_ptr<BmpInfo> Create( const uint8_t BitsPerPixel,
                                          const int32_t Width,
                                          const int32_t Height );

  virtual ~BmpInfo() = default;

  // don't need them
  BmpInfo( const BmpInfo& ) = delete;
  BmpInfo( BmpInfo&& ) = delete;
  BmpInfo& operator=( const BmpInfo& ) = delete;
  BmpInfo& operator=( BmpInfo&& ) = delete;

  uint8_t  BitsPerPixel() const { return m_BitsPerPixel; }
  int32_t  Width() const        { return m_Width; }
  int32_t  Height() const       { return m_Height; }
  uint32_t RowLength() const    { return m_RowLength; }
  uint32_t ImageDataSize() const;
  uint32_t ByteArraySize() const;

  // virtual functions 
  virtual std::unique_ptr<BmpInfo> Clone() const = 0;
  virtual uint16_t ColorTableSize() const = 0;
  virtual uint8_t  GetColorIndex( const uint8_t Byte, const uint8_t BitIndex ) const = 0;
  virtual void     SetColorIndex( uint8_t& Byte, const uint8_t BitIndex,
                                  const uint8_t ColorIndex ) const = 0;
  virtual void     ByteArrayIndices( const int32_t X, const int32_t Y,
                                     uint32_t& ByteIndex, uint8_t& BitIndex ) = 0;

  // class methods
  static bool    Supported( const uint8_t BitsPerPixel );
  static uint8_t PaddingBytes( const uint32_t RowLength );

protected:
  uint32_t CalculateRowLength() const;

  // data members
  uint8_t  m_BitsPerPixel; 
  int32_t  m_Width;
  int32_t  m_Height;
  uint32_t m_RowLength; // length of each row(in bytes), excluding padding bytes
};

#endif //BmpInfo_h
