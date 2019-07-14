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

#ifndef GifGraphicsControlExt_h
#define GifGraphicsControlExt_h

#include "GifExtension.h"

//////////////////////////////////////////////////////////////////////
class GifGraphicsControlExt : public GifExtension
{
public:
  GifGraphicsControlExt();
  GifGraphicsControlExt( const GifGraphicsControlExt& other );
  GifGraphicsControlExt& operator=( const GifGraphicsControlExt& other );

  // not implemented
  GifGraphicsControlExt( GifGraphicsControlExt&& ) = delete;
  GifGraphicsControlExt& operator=( GifGraphicsControlExt&& ) = delete;

  virtual ~GifGraphicsControlExt() {}

  uint8_t DisposalMethod() const { return( (m_PackedByte & 0x1C) >> 2 ); }
  bool    UserInput() const      { return( (m_PackedByte & 0x02) != 0 ); }
  bool    HasTransColor() const  { return( (m_PackedByte & 0x01) != 0 ); }
  uint8_t TransColor() const     { return m_TransColor; }

  void     Delay( uint16_t MilliSecond ) { m_DelayTime = MilliSecond; }
  uint16_t Delay() const                 { return m_DelayTime; }

  // overrides
  virtual GifComponent* Clone() const override; 
  virtual void Read ( std::istream& ) override;
  virtual void Write( std::ostream& ) const override;

private:
  uint8_t  m_PackedByte;
  uint16_t m_DelayTime;
  uint8_t  m_TransColor;
};

// m_PackedByte:
//   Reserved                3 Bits
//   Disposal Method         3 Bits
//   User Input Flag         1 Bit
//   Transparent Color Flag  1 Bit
// Disposal Method
//   1: next image is drawn on top of previous
//   2: next image is drawn after screen is restored to background
//   3: next image is drawn after screen is restored to previous state 

#endif //GifGraphicsControlExt_h
