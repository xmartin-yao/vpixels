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

#include "GifImageImpl.h"
#include "GifImpl.h"
#include "GifGraphicsControlExt.h"
#include "GifImageDescriptor.h"
#include "Exception.h"

////////////////////////////////
GifImageImpl::GifImageImpl( const GifImpl& RefGifImpl )
 : m_GifImpl( RefGifImpl ),
   m_pGraphicsControlExt( nullptr ),
   m_pImageDescriptor( nullptr )
{
}

//////////////////////
GifImageImpl::~GifImageImpl() = default;


///////////////////////////////////////////////
GifImageImpl& GifImageImpl::operator=( const GifImageImpl& other )
{
  if( this == &other )
    return *this;

  // GifImageImpl objects that belong to different GifImpl,
  // but have the same dimension, are allowed
  if( m_GifImpl.Width()  != other.m_GifImpl.Width() ||
      m_GifImpl.Height() != other.m_GifImpl.Height() )
    VP_THROW( "Gif objects are incompatible" )

  // deep copy
  if( m_pGraphicsControlExt != nullptr && other.m_pGraphicsControlExt != nullptr )
    *GraphicsControlExt() = *other.GraphicsControlExt();

  *ImageDescriptor() = *other.ImageDescriptor();

  return *this;
}

////////////////////////////////
// Two images are the same, if
//   1) their origins are the same, and
//   2) their dimensions are the same, and
//   3) pixels of the same location are the same.
// Two pixels of the same location are the same, if
//   a) both of them are transparent, or
//   b) both are not transparent and have the same color.
//////////////////////////////////////////////////////////
bool GifImageImpl::operator==( const GifImageImpl& other ) const
{
  if( this == &other )
    return true;

  // compare origin and dimension
  if( Left() != other.Left() || Top() != other.Top() ||
      Width() != other.Width() || Height() != other.Height() )
    return false;

  // compare pixel by pixel
  for( uint16_t y = 0; y < Height(); ++y )
  {
    for( uint16_t x = 0; x < Width(); ++x )
    {
      auto ThisTransparent = Transparent( x, y );
      auto OtherTransparent = other.Transparent( x, y );
      if( ThisTransparent && OtherTransparent)
      {
        // the two pixels are transparent
        continue;
      }
      else if( !ThisTransparent && !OtherTransparent )
      {
        // the two pixels have same color
        uint8_t R1, G1, B1, R2, G2, B2;
        GetPixel( x, y, R1, G1, B1 );
        other.GetPixel( x, y, R2, G2, B2 );
        if( R1 == R2 && G1 == G2 && B1 == B2 )
          continue;
      }

      // the two pixels are different
      return false;
    }
  }

  return true;
}

//////////////////////
uint8_t GifImageImpl::BitsPerPixel() const
{
  return ImageDescriptor()->BitsPerPixel();
}

//////////////////////
void GifImageImpl::BitsPerPixel( uint8_t Bpp )
{
#ifndef VP_EXTENSION
  if( Bpp < 2 || Bpp > 8 )
    VP_THROW( "color resolution not supported" );

  // Bpp is larger than global Bpp
  if( !ImageDescriptor()->LocalColorTable() && Bpp > m_GifImpl.BitsPerPixel() )
    VP_THROW( "color resolution incompatible" );
#endif

  ImageDescriptor()->BitsPerPixel( Bpp );
  if( ImageDescriptor()->LocalColorTable() )
    ImageDescriptor()->ColorTableSize( (1 << Bpp) );
}

//////////////////////
uint16_t GifImageImpl::Left() const
{
  return ImageDescriptor()->Left();
}

//////////////////////
uint16_t GifImageImpl::Top() const
{
  return ImageDescriptor()->Top();
}

//////////////////////
uint16_t GifImageImpl::Width() const
{
  return ImageDescriptor()->Width();
}

//////////////////////
uint16_t GifImageImpl::Height() const
{
  return ImageDescriptor()->Height();
}

//////////////////////////////
void GifImageImpl::Crop( const uint16_t Left, const uint16_t Top,
                         const uint16_t Width, const uint16_t Height )
{
  ImageDescriptor()->Crop( Left, Top, Width, Height );
}

//////////////////////////////
bool GifImageImpl::ColorTable() const
{
  return ImageDescriptor()->LocalColorTable();
}

///////////////////////////////////////
uint16_t GifImageImpl::ColorTableSize() const
{
  return ImageDescriptor()->ColorTableSize();
}

///////////////////////////////////////
void GifImageImpl::ColorTableSize( uint16_t Size )
{
  // size not changed
  if( !ImageDescriptor()->ColorTableSize( Size ) )
    return;

  // if color table disabled
  // set bpp using the value stored in GifScreenDescriptor
  if( !ImageDescriptor()->LocalColorTable() )
    ImageDescriptor()->BitsPerPixel( m_GifImpl.BitsPerPixel() );
}

///////////////////////////////////////
bool GifImageImpl::ColorTableSorted() const
{
  return ImageDescriptor()->ColorTableSorted();
}

///////////////////////////////////////////////////////////
void GifImageImpl::SetColorTable( const uint8_t Index, const uint8_t Red, 
                              const uint8_t Green, const uint8_t Blue )
{
  ImageDescriptor()->SetColorTable( Index, Red, Green, Blue );
}

//////////////////////////////////////////////////////////////////////
void GifImageImpl::GetColorTable( const uint8_t Index, uint8_t& Red, 
                              uint8_t& Green, uint8_t& Blue ) const
{
  ImageDescriptor()->GetColorTable( Index, Red, Green, Blue );
}

/////////////////////////
bool GifImageImpl::Interlaced() const
{
  return ImageDescriptor()->Interlaced();
}

/////////////////
// get size of local or global color table
///////////////////////////////////////////
uint16_t GifImageImpl::CheckColorTable() const
{
  uint16_t Size = 0;
  if( ImageDescriptor()->LocalColorTable() )
    Size = ImageDescriptor()->ColorTableSize();
  else
    Size = m_GifImpl.ColorTableSize();

  return Size;
}

/////////////////////////////////////////////
bool GifImageImpl::CheckColorIndex( const uint8_t ColorIndex ) const
{
  return ColorIndex < CheckColorTable();
}

/////////////////////////////////////////
void GifImageImpl::SetAllPixels( const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( !CheckColorIndex(ColorIndex) )
    VP_THROW( "color index out of range" )
#endif

  ImageDescriptor()->SetAllPixels( ColorIndex );
}

//////////////////////////////////////////////////////////////////////
void GifImageImpl::SetPixel( const uint16_t X, const uint16_t Y, const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( !CheckColorIndex(ColorIndex) )
    VP_THROW( "color index out of range" )
#endif

  ImageDescriptor()->SetPixel( X, Y, ColorIndex );
}

////////////////////////////////////////////////////////////////
uint8_t GifImageImpl::GetPixel( const uint16_t X, const uint16_t Y ) const
{
  return ImageDescriptor()->GetPixel( X, Y );
}

///////////////////////////////////////////////
void GifImageImpl::GetPixel( const uint16_t X, const uint16_t Y,
                             uint8_t& Red, uint8_t& Green, uint8_t& Blue ) const
{
#ifndef VP_EXTENSION
  if( !(ImageDescriptor()->LocalColorTable() || m_GifImpl.ColorTable()) )
    VP_THROW( "there's neither global nor local color table" )
#endif

  auto Index = GetPixel( X, Y );
  if( ColorTable() )
    GetColorTable( Index, Red, Green, Blue );
  else
    m_GifImpl.GetColorTable( Index, Red, Green, Blue );
}

// check whether the pixel is transparent or not
/////////////////////////////////////////////////
bool GifImageImpl::Transparent( const uint16_t X, const uint16_t Y ) const
{
  return HasTransColor() && GetPixel(X, Y) == TransColor();
}

/////////////////////////////////////////////
void GifImageImpl::Delay( uint16_t Centisecond )
{
#ifndef VP_EXTENSION
  if( m_pGraphicsControlExt == nullptr )
    VP_THROW( "can not set delay time for single image" )
#endif

  GraphicsControlExt()->Delay( Centisecond );
}

/////////////////////////////////
uint16_t GifImageImpl::Delay() const
{
  if( m_pGraphicsControlExt == nullptr )
    return 0;
  else
    return GraphicsControlExt()->Delay();
}

/////////////////////////////////
uint8_t GifImageImpl::DisposalMethod() const
{
  if( m_pGraphicsControlExt == nullptr )
    return 0;
  else
    return GraphicsControlExt()->DisposalMethod();
}

////////////////////////////////////////
void GifImageImpl::DisposalMethod( const uint8_t MethodID )
{
#ifndef VP_EXTENSION
  if( m_pGraphicsControlExt == nullptr )
    VP_THROW( "can not get disposal method for single image" )
#endif

  GraphicsControlExt()->DisposalMethod( MethodID );
}

/////////////////////////////////
bool GifImageImpl::UserInput() const
{
  if( m_pGraphicsControlExt == nullptr )
    return false;
  else
    return GraphicsControlExt()->UserInput();
}

/////////////////////////////////
bool GifImageImpl::HasTransColor() const
{
  if( m_pGraphicsControlExt == nullptr )
    return false;
  else
    return GraphicsControlExt()->HasTransColor();
}

/////////////////////////////////
void GifImageImpl::HasTransColor( const bool TrunOn )
{
#ifndef VP_EXTENSION
  if( m_pGraphicsControlExt == nullptr )
    VP_THROW( "can not set transparent color for single image" )
#endif

  GraphicsControlExt()->HasTransColor( TrunOn );
}

/////////////////////////////////
void GifImageImpl::TransColor( const uint8_t ColorIndex )
{
#ifndef VP_EXTENSION
  if( m_pGraphicsControlExt == nullptr )
    VP_THROW( "can not set transparent color for single image" )

  if( !CheckColorIndex(ColorIndex) )
    VP_THROW( "color index out of range" )
#endif

  GraphicsControlExt()->TransColor( ColorIndex );
}

/////////////////////////////////
uint8_t GifImageImpl::TransColor() const
{
#ifndef VP_EXTENSION
  if( m_pGraphicsControlExt == nullptr )
    VP_THROW( "single image has no transparent color" )
#endif

  return GraphicsControlExt()->TransColor();
}

/////////////////////////////////
void GifImageImpl::TransColor( uint8_t& Red, uint8_t& Green, uint8_t& Blue ) const
{
  auto Index = TransColor();
  if( ColorTable() )
    GetColorTable( Index, Red, Green, Blue );
  else
    m_GifImpl.GetColorTable( Index, Red, Green, Blue );
}

/////////////////////////////////
GifGraphicsControlExt* GifImageImpl::GraphicsControlExt()
{
  return m_pGraphicsControlExt.get();
}

/////////////////////////////////
const GifGraphicsControlExt* GifImageImpl::GraphicsControlExt() const
{
  return m_pGraphicsControlExt.get();
}

/////////////////////////////////
GifImageDescriptor* GifImageImpl::ImageDescriptor()
{
  return m_pImageDescriptor.get();
}

/////////////////////////////////
const GifImageDescriptor* GifImageImpl::ImageDescriptor() const
{
  return m_pImageDescriptor.get();
}
