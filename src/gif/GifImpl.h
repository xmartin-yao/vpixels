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

#ifndef GifImpl_h
#define GifImpl_h

#include <cstdint>
#include <iosfwd>
#include <string>
#include "GifHeader.h"
#include "GifScreenDescriptor.h"
#include "VecDefs.h"

// forward
namespace vp { class GifImage; }


//////////////////////////////////////////
struct GifImpl
{
  // ctor
  GifImpl( const uint8_t BitsPerPixel = 2,
           const uint16_t Width = 1, const uint16_t Height = 1,
           const size_t Images = 1, const bool GlobalColorTable = true );

  GifImpl( const GifImpl& other );

  ~GifImpl() = default;

  // not implemented
  GifImpl( GifImpl&& ) = delete;
  GifImpl& operator=( const GifImpl& other ) = delete;
  GifImpl& operator=( GifImpl&& ) = delete;

  // GIF version
  std::string Version() const;

  // bpp
  uint8_t  BitsPerPixel() const;

  // dimension
  uint16_t Width() const;
  uint16_t Height() const;

  // color table
  bool     ColorTable() const;
  uint16_t ColorTableSize() const;
  void     ColorTableSize( uint16_t& Size );
  bool     ColorTableSorted() const;
  void     SetColorTable( const uint8_t Index, const uint8_t Red,
                          const uint8_t Green, const uint8_t Blue );
  void     GetColorTable( const uint8_t Index, uint8_t& Red,
                          uint8_t& Green, uint8_t& Blue ) const;

  // images
  size_t Images() const;
  vp::GifImage&       operator[]( const size_t Index ) &; 
  const vp::GifImage& operator[]( const size_t Index ) const &; 

  // IO
  bool Import( const std::string& FileName );
  bool Export( const std::string& FileName, const bool OverWrite = false );

  // IO utils
  uint8_t Read( std::istream& );
  void    Write( std::ostream& );

  // for test
  const GifComponentVec& Components() const { return m_ComponentVec; }

  // data members
  GifHeader           m_Header;
  GifScreenDescriptor m_ScreenDescriptor;
  GifComponentVec     m_ComponentVec;
  GifImageVec         m_ImageVec;

  static const uint8_t m_GifTrailer = 0x3B;
};

#endif //GifImpl_h
