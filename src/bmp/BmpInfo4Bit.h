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

#ifndef BmpInfo4Bit_h
#define BmpInfo4Bit_h

#include "BmpInfo.h"

///////////////////////////////////
class BmpInfo4Bit : public BmpInfo
{
public:
  BmpInfo4Bit( const int32_t Width, const int32_t Height );

  virtual ~BmpInfo4Bit() = default;

  // don't need them
  BmpInfo4Bit( const BmpInfo4Bit& ) = delete;
  BmpInfo4Bit( BmpInfo4Bit&& ) = delete;
  BmpInfo4Bit& operator=( const BmpInfo4Bit& ) = delete;
  BmpInfo4Bit& operator=( BmpInfo4Bit&& ) = delete;

  virtual std::unique_ptr<BmpInfo> Clone() const override;
  virtual uint16_t ColorTableSize() const override { return 16; }
  virtual uint8_t  GetColorIndex( const uint8_t Byte, const uint8_t BitIndex ) const override;
  virtual void     SetColorIndex( uint8_t& Byte, const uint8_t BitIndex,
                                  const uint8_t ColorIndex ) const override;
  virtual void     ByteArrayIndices( const int32_t X, const int32_t Y,
                                     uint32_t& ByteIndex, uint8_t& BitIndex ) override;
};

#endif //BmpInfo4Bit_h
