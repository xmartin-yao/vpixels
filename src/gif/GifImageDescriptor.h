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

#ifndef GifImageDescriptor_h
#define GifImageDescriptor_h

#include "GifComponent.h"
#include "GifColorTable.h"
#include "GifImageData.h"

//////////////////////////////////////////////////////////////////////
class GifImageDescriptor : public GifComponent
{
public:
  GifImageDescriptor();
  GifImageDescriptor( const uint8_t BitsPerPixel,
                      const uint16_t Width, const uint16_t Height,
                      const uint16_t Left = 0, const uint16_t Top = 0,
                      const bool ColorTable = false );  // no local color table by default
  GifImageDescriptor( const GifImageDescriptor& other );
  GifImageDescriptor& operator=( const GifImageDescriptor& other );

  virtual ~GifImageDescriptor() = default;

  // not in use
  GifImageDescriptor( GifImageDescriptor&& ) = delete;
  GifImageDescriptor& operator=( GifImageDescriptor&& ) = delete;

  // location and dimension
  uint16_t Left() const   { return m_Left; }
  uint16_t Top() const    { return m_Top; }
  uint16_t Width() const  { return m_Width; }
  uint16_t Height() const { return m_Height; }
  void     Crop( const uint16_t Left, const uint16_t Top,
                 const uint16_t Width, const uint16_t Height );

  // local color table
  bool     LocalColorTable() const;
  uint16_t ColorTableSize() const;
  void     ColorTableSize( uint16_t );
  bool     ColorTableSorted() const;
  void     SetColorTable( const uint8_t Index, const uint8_t Red,
                          const uint8_t Green, const uint8_t Blue );
  void     GetColorTable( const uint8_t Index, uint8_t& Red, 
                          uint8_t& Green, uint8_t& Blue ) const;

  // image data
  uint8_t  BitsPerPixel() const;
  void     SetAllPixels( const uint8_t ColorIndex );
  void     SetPixel( uint16_t X, uint16_t Y, uint8_t  Index );
  uint8_t  GetPixel( uint16_t X, uint16_t Y ) const;
  bool     Interlaced() const;
  uint32_t PixelIndex( const uint16_t X, const uint16_t Y ) const;

  // overrides
  virtual GifComponent* Clone() const override; 
  virtual void Read ( std::istream& ) override;
  virtual void Write( std::ostream& ) const override;

private:
  uint16_t m_Left;
  uint16_t m_Top;
  uint16_t m_Width;
  uint16_t m_Height;
  uint8_t  m_PackedByte; 

  GifColorTable m_ColorTable; // local color table, optional, none when size=0
  GifImageData  m_ImageData;
};

// m_PackedByte:
//   Local Color Table Flag      1 Bit
//   Interlaced Flag             1 Bit
//   Sorted Flag                 1 Bit
//   Reserved                    2 Bit
//   Size of Global Color Table  3 Bits

#endif //GifImageDescriptor_h
