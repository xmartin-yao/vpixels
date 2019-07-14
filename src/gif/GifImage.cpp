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

#include "GifImage.h"
#include "GifImageImpl.h"

using namespace vp;

////////////////////////////////
GifImage::GifImage( std::unique_ptr<GifImageImpl> pImpl  )
 : m_pImpl( std::move(pImpl) )
{
}

//////////////////////
GifImage::~GifImage() = default;

///////////////////////////////////////////////
GifImage& GifImage::operator=( const GifImage& other )
{
  if( this != &other )
    *m_pImpl = *other.m_pImpl;

  return *this;
}

//////////////////////
uint8_t GifImage::BitsPerPixel() const
{
  return GetImpl()->BitsPerPixel();
}

//////////////////////
uint16_t GifImage::Left() const
{
  return GetImpl()->Left();
}

//////////////////////
uint16_t GifImage::Top() const
{
  return GetImpl()->Top();
}

//////////////////////
uint16_t GifImage::Width() const
{
  return GetImpl()->Width();
}

//////////////////////
uint16_t GifImage::Height() const
{
  return GetImpl()->Height();
}

//////////////////////////////
void GifImage::Crop( const uint16_t Left, const uint16_t Top,
                     const uint16_t Width, const uint16_t Height )
{
  GetImpl()->Crop( Left, Top, Width, Height );
}

//////////////////////////////
bool GifImage::ColorTable() const
{
  return GetImpl()->ColorTable();
}

///////////////////////////////////////
uint16_t GifImage::ColorTableSize() const
{
  return GetImpl()->ColorTableSize();
}

///////////////////////////////////////
void GifImage::ColorTableSize( uint16_t Size )
{
  GetImpl()->ColorTableSize( Size );
}

///////////////////////////////////////
bool GifImage::ColorTableSorted() const
{
  return GetImpl()->ColorTableSorted();
}

///////////////////////////////////////////////////////////
void GifImage::SetColorTable( const uint8_t Index, const uint8_t Red, 
                              const uint8_t Green, const uint8_t Blue )
{
  GetImpl()->SetColorTable( Index, Red, Green, Blue );
}

//////////////////////////////////////////////////////////////////////
void GifImage::GetColorTable( const uint8_t Index, uint8_t& Red, 
                              uint8_t& Green, uint8_t& Blue ) const
{
  GetImpl()->GetColorTable( Index, Red, Green, Blue );
}

/////////////////////////
bool GifImage::Interlaced() const
{
  return GetImpl()->Interlaced();
}

/////////////////////////////////////////
void GifImage::SetAllPixels( const uint8_t ColorIndex )
{
  GetImpl()->SetAllPixels( ColorIndex );
}

//////////////////////////////////////////////////////////////////////
void GifImage::SetPixel( const uint16_t X, const uint16_t Y, const uint8_t ColorIndex )
{
  GetImpl()->SetPixel( X, Y, ColorIndex );
}

////////////////////////////////////////////////////////////////
uint8_t GifImage::GetPixel( const uint16_t X, const uint16_t Y ) const
{
  return GetImpl()->GetPixel( X, Y );
}

/////////////////////////////////////////////
bool GifImage::Delay( uint16_t MilliSec )
{
  return GetImpl()->Delay( MilliSec );
}

/////////////////////////////////
uint16_t GifImage::Delay() const
{
  return GetImpl()->Delay();
}

////////////////////////////
const GifImageImpl* GifImage::GetImpl() const
{
  return m_pImpl.get();
}

////////////////////////
GifImageImpl* GifImage::GetImpl()
{
  return m_pImpl.get();
}
