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

#ifndef VP_BMP_H
#define VP_BMP_H

#include <cstdint>
#include <iosfwd>
#include <string>
#include <memory> 

// forward
struct BmpImpl;

///////////////////
namespace vp
{
  class Bmp
  {
  public:
    static std::string PackageVersion();

    //  Supported BitsPerPixel: 1, 4, 8, 24
    static bool Supported( const uint8_t BitsPerPixel );

    // ctors
    Bmp( const uint8_t BitsPerPixel = 1, const int32_t Width = 1, const int32_t Height = 1 );
    Bmp( const Bmp& other );
    Bmp( Bmp&& other );

    // dtor
    virtual ~Bmp();

    // assignment
    Bmp& operator=( const Bmp& other);
    Bmp& operator=( Bmp&& other );

    // validity
    explicit operator bool() const;
    
    // IO
    bool Import( const std::string& FileName );
    bool Export( const std::string& FileName, const bool OverWrite = false ) const;

    // bpp
    uint8_t BitsPerPixel() const;

    // dimension
    int32_t Width() const;
    int32_t Height() const;

    // non-indexed bmp
    void SetAllPixels( const uint8_t Blue, const uint8_t Green, const uint8_t Red );
    void SetPixel( const int32_t X, const int32_t Y,
                   const uint8_t Blue, const uint8_t Green, const uint8_t Red );
    void GetPixel( const int32_t X, const int32_t Y,
                   uint8_t& Blue, uint8_t& Green, uint8_t& Red ) const;

    // indexed bmp
    void SetAllPixels( const uint8_t ColorIndex );
    void SetPixel( const int32_t X, const int32_t Y, const uint8_t ColorIndex );
    uint8_t  GetPixel( const int32_t X, const int32_t Y ) const;

    // color table for indexed bmp
    uint16_t ColorTableSize() const;
    void SetColorTable( const uint8_t ColorIndex,
                        const uint8_t Blue, const uint8_t Green, const uint8_t Red );
    void GetColorTable( const uint8_t ColorIndex,
                        uint8_t& Blue, uint8_t& Green, uint8_t& Red ) const;

  private:
    const BmpImpl* GetImpl() const;
    BmpImpl*       GetImpl();

    std::unique_ptr<BmpImpl> m_pImpl;
  };

} //namespace vp
#endif //VP_BMP_H
