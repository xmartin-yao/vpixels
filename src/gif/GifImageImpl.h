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

#ifndef GifImageImpl_h
#define GifImageImpl_h

#include <cstdint>
#include <memory>

// forward
struct GifImpl;
class GifComponent;
class GifGraphicsControlExt;
class GifImageDescriptor;


//////////////////////////////////////////////
// This class implements class vp::GifImage.
//
// A GIF image consists of
//     GifGraphicsControlExt
//     GifImageDescriptor
// GifGraphicsControlExt is optional. When there is only one image
// in GIF GifGraphicsControlExt does not present. (see GifImageVecBuilder
// and GIF89a Specification).
//
// 1) The class delegates the function calls to either GifGraphicsControlExt
//    or GifImageDescriptor.
// 2) The class holds only the pointers of GifGraphicsControlExt and 
//    GifImageDescriptor, which are instantiated somewhere else.
// 3) The assignment operator allows copying an image from different
//    but compatible GifImpl object.
////////////////////////////////////////////////////////////////
struct GifImageImpl
{
  explicit GifImageImpl( const GifImpl& );
  ~GifImageImpl();

  GifImageImpl& operator=( const GifImageImpl& );

  // not implemented
  GifImageImpl( const GifImageImpl& ) = delete;
  GifImageImpl( GifImageImpl&& ) = delete;
  GifImageImpl& operator=( GifImageImpl&& ) = delete;

  // compare
  bool operator==( const GifImageImpl& ) const;

  // bpp
  uint8_t  BitsPerPixel() const;
  void     BitsPerPixel( const uint8_t bpp );

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
  void    GetPixel( const uint16_t X, const uint16_t Y,
                    uint8_t& Red, uint8_t& Green, uint8_t& Blue ) const;
  bool    Transparent( const uint16_t X, const uint16_t Y ) const;

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
  void    TransColor( uint8_t& Red, uint8_t& Green, uint8_t& Blue ) const;
  void    TransColor( const uint8_t ColorIndex );

  bool UserInput() const;

  // utils
  uint16_t CheckColorTable() const;
  bool     CheckColorIndex( const uint8_t ColorIndex ) const;
  bool     SingleImage() const { return m_pGraphicsControlExt == nullptr; }

  const GifGraphicsControlExt* GraphicsControlExt() const ;
  GifGraphicsControlExt*       GraphicsControlExt();
  const GifImageDescriptor*    ImageDescriptor() const ;
  GifImageDescriptor*          ImageDescriptor();

  // data members
  const GifImpl& m_GifImpl;
  std::shared_ptr<GifGraphicsControlExt> m_pGraphicsControlExt; // optional
  std::shared_ptr<GifImageDescriptor>    m_pImageDescriptor;
};

#endif //GifImageImpl_h

