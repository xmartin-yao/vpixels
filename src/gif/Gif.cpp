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

#include "Gif.h"
#include "GifImpl.h"
#include "GifImage.h"
#include "config.h"
#include <fstream>

using namespace vp;

/////////////////////////////
std::string Gif::PackageVersion()
{
  return PACKAGE_STRING;
}

/////////////////////////////////////////////
bool Gif::IsSupported( const uint8_t BitsPerPixel )
{
  return BitsPerPixel >= 2 && BitsPerPixel <= 8;
}

///////////////////////////////////////////////////////////
Gif::Gif( const uint8_t BitsPerPixel,
          const uint16_t Width, const uint16_t Height,
          const size_t Images, const bool GlobalColorTable )
 : m_pImpl( std::make_unique<GifImpl>(BitsPerPixel, Width, Height, Images, GlobalColorTable) )
{
}

////////////////////////////////
Gif::Gif( const Gif& other )
 : m_pImpl( std::make_unique<GifImpl>(*other.m_pImpl) )
{
}

///////////////////////////
Gif& Gif::operator=( const Gif& other )
{
  if( this != &other )
    m_pImpl.reset( new GifImpl(*other.m_pImpl) );

  return *this;
}

////////////////////////////
Gif::Gif( Gif&& ) = default;
Gif& Gif::operator=( Gif&& ) = default;
Gif::~Gif() = default;

///////////////////////////
std::string Gif::Version() const
{
  return GetImpl()->Version();
}

//////////////////////////
uint16_t Gif::Width() const
{
  return GetImpl()->Width();
}

//////////////////////////
uint16_t Gif::Height() const
{
  return GetImpl()->Height();
}

/////////////////////////////////
uint8_t Gif::BitsPerPixel() const
{
  return GetImpl()->BitsPerPixel();
}

/////////////////////////////////
void Gif::BitsPerPixel( const uint8_t Bpp )
{
  GetImpl()->BitsPerPixel( Bpp );
}

//////////////////////////////////
bool Gif::ColorTable() const
{
  return GetImpl()->ColorTable();
}

////////////////////////////////////
uint16_t Gif::ColorTableSize() const
{
  return GetImpl()->ColorTableSize();
}

////////////////////////////////////////
void Gif::ColorTableSize( uint16_t Size )
{
  GetImpl()->ColorTableSize( Size );
}

//////////////////////////////////
bool Gif::ColorTableSorted() const
{
  return GetImpl()->ColorTableSorted();
}

/////////////////////////////////////////////////////////
void Gif::SetColorTable( const uint8_t Index, const uint8_t Red,
                         const uint8_t Green, const uint8_t Blue )
{
  GetImpl()->SetColorTable( Index, Red, Green, Blue );
}

/////////////////////////////////////////////////////////
void Gif::GetColorTable( const uint8_t Index, uint8_t& Red,
                         uint8_t& Green, uint8_t& Blue ) const
{
  GetImpl()->GetColorTable( Index, Red, Green, Blue );
}

////////////////////////////
void Gif::BackgroundColor( const uint8_t ColorIndex )
{
  return GetImpl()->BackgroundColor( ColorIndex );
}

////////////////////////////
uint8_t Gif::BackgroundColor() const
{
  return GetImpl()->BackgroundColor();
}

////////////////////////////
uint8_t Gif::AspectRatio() const
{
  return GetImpl()->AspectRatio();
}

////////////////////////////
size_t Gif::Images() const
{
  return GetImpl()->Images();
}

/////////////////////////////////////////
// access an image by index
//////////////////////////////////////////////////
GifImage& Gif::operator[]( const size_t Index ) &
{
  return (*GetImpl())[Index];
}

//////////////////////////////////////////////////
const GifImage& Gif::operator[]( const size_t Index ) const &
{
  return (*GetImpl())[Index];
}

//////////////////////////////////////////////
bool Gif::Import( const std::string& FileName )
{
  return GetImpl()->Import(FileName);
}

///////////////////////////////////////////////////////////////
bool Gif::Export( const std::string& FileName, const bool OverWrite )
{
  return GetImpl()->Export(FileName, OverWrite);
}

/////////////////////////
Gif::operator bool() const
{
  return m_pImpl.get() != nullptr;
}

////////////////////////////
const GifImpl* Gif::GetImpl() const
{
  return m_pImpl.get();
}

////////////////////////
GifImpl* Gif::GetImpl()
{
  return m_pImpl.get();
}
