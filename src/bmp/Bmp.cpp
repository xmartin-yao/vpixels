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

#include "Bmp.h"
#include "BmpImpl.h"
#include "BmpInfo.h"
#include "config.h"

using namespace vp;

/////////////////////////////
std::string Bmp::PackageVersion()
{
  return PACKAGE_STRING;
}

/////////////////////////////
bool Bmp::Supported( const uint8_t BitsPerPixel )
{
  return BmpInfo::Supported( BitsPerPixel );
}

///////////////////////////////////////////////////////////////
Bmp::Bmp( const uint8_t BitsPerPixel, const int32_t Width, const int32_t Height )
 : m_pImpl( std::make_unique<BmpImpl>(BitsPerPixel, Width, Height) )
{
}

/////////////////////////////////
Bmp::Bmp( const Bmp& other )
 : m_pImpl( std::make_unique<BmpImpl>(*other.m_pImpl) )
{
}

///////////////////////////////////
Bmp& Bmp::operator=( const Bmp& other )
{
  if( this != &other )
    m_pImpl.reset( new BmpImpl(*other.m_pImpl) );

  return *this;
}

////////////////////
Bmp::Bmp(Bmp&&) = default;
Bmp& Bmp::operator=( Bmp&& ) = default;
Bmp::~Bmp() = default;

//////////////////////////////////////////////
bool Bmp::Import( const std::string& FileName )
{
  return GetImpl()->Import( FileName );
}

///////////////////////////////////////////////////////////////
bool Bmp::Export( const std::string& FileName, const bool OverWrite ) const
{
  return GetImpl()->Export( FileName, OverWrite );
}

///////////////////////////////
uint8_t Bmp::BitsPerPixel() const
{
  return GetImpl()->BitsPerPixel();
}

///////////////////////////////
int32_t Bmp::Width() const
{
  return GetImpl()->Width();
}

///////////////////////////////
int32_t Bmp::Height() const
{
  return GetImpl()->Height();
}

///////////////////////////////
uint16_t Bmp::ColorTableSize() const
{
  return GetImpl()->ColorTableSize();
}

///////////////////////////////////////////////////////////////////
void Bmp::SetAllPixels( const uint8_t Blue, const uint8_t Green, const uint8_t Red )
{
  GetImpl()->SetAllPixels( Blue, Green, Red );
}

//////////////////////////////////////////////////////////////
void Bmp::SetPixel( const int32_t X, const int32_t Y,
                    const uint8_t Blue, const uint8_t Green, const uint8_t Red )
{
  GetImpl()->SetPixel( X, Y, Blue, Green, Red );
}

//////////////////////////////////////////////////////////
void Bmp::GetPixel( const int32_t X, const int32_t Y,
                    uint8_t& Blue, uint8_t& Green, uint8_t& Red ) const
{
  GetImpl()->GetPixel( X, Y, Blue, Green, Red );
}

////////////////////////////////////////////////
void Bmp::SetAllPixels( const uint8_t ColorIndex )
{
  GetImpl()->SetAllPixels( ColorIndex );
}

//////////////////////////////////////////////////////////////
void Bmp::SetPixel( const int32_t X, const int32_t Y, const uint8_t ColorIndex )
{
  GetImpl()->SetPixel( X, Y, ColorIndex );
}

////////////////////////////////////////////////////////
uint8_t Bmp::GetPixel( const int32_t X, const int32_t Y ) const
{
  return GetImpl()->GetPixel( X, Y );
}

//////////////////////////////////////////////////////////////
void Bmp::SetColorTable( const uint8_t ColorIndex,
                         const uint8_t Blue, const uint8_t Green, const uint8_t Red )
{
  GetImpl()->SetColorTable( ColorIndex, Blue, Green, Red );
}

//////////////////////////////////////////////////////////////
void Bmp::GetColorTable( const uint8_t ColorIndex,
                         uint8_t& Blue, uint8_t& Green, uint8_t& Red ) const
{
  GetImpl()->GetColorTable( ColorIndex, Blue, Green, Red );
}

/////////////////////////
Bmp::operator bool() const
{
  return m_pImpl.get() != nullptr;
}

///////////////////////////////
const BmpImpl* Bmp::GetImpl() const
{
  return m_pImpl.get();
}

///////////////////////////////
BmpImpl* Bmp::GetImpl()
{
  return m_pImpl.get();
}
