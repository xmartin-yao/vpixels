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

#ifndef VP_GIFIMAGE_H
#define VP_GIFIMAGE_H

#include <cstdint>
#include <memory>

// forward
struct GifImageImpl;

namespace vp
{
  ////////////////////////////////////////////////////////////////
  // This class provides an interface to manipulate an image.
  //
  // Directly instantiating it is not allowed, therefore constructor
  // is declared as private, copy and move constructor are not
  // implemented.
  /////////////////////////////////////////////////////////////////
  class GifImage
  {
  public:
    GifImage& operator=( const GifImage& );
    virtual ~GifImage();

    // not implemented
    GifImage( const GifImage& ) = delete;
    GifImage( GifImage&& ) = delete;
    GifImage& operator=( GifImage&& ) = delete;

    // bpp
    uint8_t  BitsPerPixel() const;

    // dimension
    uint16_t Left() const;
    uint16_t Top() const;
    uint16_t Width() const;
    uint16_t Height() const;
    void     Crop( const uint16_t Left, const uint16_t Top,
                   const uint16_t Width, const uint16_t Height );

    // pixels
    bool    Interlaced() const;
    void    SetAllPixels( const uint8_t Index );
    void    SetPixel( const uint16_t X, const uint16_t Y, const uint8_t Index );
    uint8_t GetPixel( const uint16_t X, const uint16_t Y ) const;

    // delay
    uint16_t Delay() const;
    void     Delay( const uint16_t Centisecond );

    // local color table
    bool     ColorTable() const;
    uint16_t ColorTableSize() const;
    void     ColorTableSize( uint16_t Size );
    bool     ColorTableSorted() const;
    void     SetColorTable( const uint8_t Index, const uint8_t Red,
                            const uint8_t Green, const uint8_t Blue );
    void     GetColorTable( const uint8_t Index, uint8_t& Red,
                            uint8_t& Green, uint8_t& Blue ) const;

    // disposal method
    uint8_t DisposalMethod() const;
    void    DisposalMethod( const uint8_t MethodID );

    // transparent color
    bool    HasTransColor() const;
    void    HasTransColor( const bool TrunOn );
    uint8_t TransColor() const;
    void    TransColor( const uint8_t ColorIndex );

    bool UserInput() const;

    // utils
    uint16_t CheckColorTable() const;
    bool     SingleImage() const;

  private:
    // ctor
    explicit GifImage( std::unique_ptr<GifImageImpl> pImpl );

    // b/c ctor is private, in order to instantiate vp::GifImage, 
    // std::make_unique() should be friend. see GifImageVecBuilder
    friend std::unique_ptr<vp::GifImage>
    std::make_unique<vp::GifImage>( std::unique_ptr<GifImageImpl>&& );
    
    const GifImageImpl* GetImpl() const;
    GifImageImpl*       GetImpl();

    std::unique_ptr<GifImageImpl> m_pImpl;
  };

} //namespace vp

#endif // VP_GIFIMAGE_H
