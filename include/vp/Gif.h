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

#ifndef VP_GIF_H
#define VP_GIF_H

#include <cstdint>
#include <iosfwd>
#include <string>
#include <memory>

// forward
struct GifImpl;
namespace vp { class GifImage; }


namespace vp
{
  //////////////////////////////////////////
  class Gif
  {
  public:
    static std::string PackageVersion();

    // Supported: 2, 3, ... 8 bits/pixel
    static bool IsSupported( const uint8_t BitsPerPixel );

    // ctors
    Gif( const uint8_t BitsPerPixel = 2,
         const uint16_t Width = 1, const uint16_t Height = 1,
         const size_t Images = 1, const bool GlobalColorTable = true );
    Gif( const Gif& other );
    Gif( Gif&& );

    virtual ~Gif();

    // assignment
    Gif& operator=( const Gif& );
    Gif& operator=( Gif&& );

    // validity
    explicit operator bool() const;

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
    void     ColorTableSize( uint16_t Size );
    bool     ColorTableSorted() const;
    void     SetColorTable( const uint8_t Index, const uint8_t Red,
                            const uint8_t Green, const uint8_t Blue );
    void     GetColorTable( const uint8_t Index, uint8_t& Red,
                            uint8_t& Green, uint8_t& Blue ) const;

    void     BackgroundColor( const uint8_t ColorIndex );
    uint8_t  BackgroundColor() const;

    uint8_t  AspectRatio() const;

    // images
    size_t Images() const;
    GifImage& operator[]( const size_t Index ) &; 
    const GifImage& operator[]( const size_t Index ) const &; 

    // IO
    bool Import( const std::string& FileName );
    bool Export( const std::string& FileName, const bool OverWrite = false );

  private:
    const GifImpl* GetImpl() const;
    GifImpl*       GetImpl();

    std::unique_ptr<GifImpl> m_pImpl;
  };

} //namespace vp
#endif //VP_GIF_H
