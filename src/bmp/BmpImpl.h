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

#ifndef BmpImpl_h
#define BmpImpl_h

#include <cstdint>
#include <cstddef>  // size_t
#include <string>

#include "BmpInfo.h"
#include "BmpFileHeader.h"
#include "BmpInfoHeader.h"
#include "BmpColorTable.h"
#include "BmpImageData.h"


struct BmpImpl
{
  // ctors
  BmpImpl( const uint8_t BitsPerPixel = 1, const int32_t Width = 1, const int32_t Height =1 );
  BmpImpl( const BmpImpl& other );

  ~BmpImpl() = default;

  // don't need them
  BmpImpl( BmpImpl&& ) = delete;
  BmpImpl& operator=( const BmpImpl& other ) = delete;
  BmpImpl& operator=( BmpImpl&& ) = delete;

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

  // IO
  void Read( std::istream& );
  void Write( std::ostream& ) const;

  // data members
  std::unique_ptr<BmpInfo> m_pBmpInfo;
  BmpFileHeader m_FileHeader;
  BmpInfoHeader m_InfoHeader;
  BmpColorTable m_ColorTable;  // size=0 for non-indexed bmp
  BmpImageData  m_ImageData;  
};

///////////////////////////////
inline uint8_t BmpImpl::BitsPerPixel() const
{
  return m_pBmpInfo->BitsPerPixel();
}

///////////////////////////////
inline int32_t BmpImpl::Width() const
{
  return m_pBmpInfo->Width();
}

///////////////////////////////
inline int32_t BmpImpl::Height() const
{
  return m_pBmpInfo->Height();
}

///////////////////////////////
inline uint16_t BmpImpl::ColorTableSize() const
{
  return m_ColorTable.Size();
}

#endif //BmpImpl_h
