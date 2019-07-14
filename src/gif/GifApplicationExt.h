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

#ifndef GifApplicationExt_h
#define GifApplicationExt_h

#include "GifExtension.h"
#include "U8String.h"

//////////////////////////////////////////////////////////////////////
class GifApplicationExt : public GifExtension
{
public:
  GifApplicationExt();
  GifApplicationExt( const GifApplicationExt& other);

  //not implemented
  GifApplicationExt( const GifApplicationExt&& ) = delete;
  GifApplicationExt& operator=( const GifApplicationExt& ) = delete;
  GifApplicationExt& operator=( const GifApplicationExt&& ) = delete;

  virtual ~GifApplicationExt() = default;

  std::string AppID()    const { return m_AppInfo.substr( 0, 8 ); }
  std::string AuthCode() const { return m_AppInfo.substr( 8, 3 ); }

  uint16_t LoopTimes() const;
  bool     LoopTimes( const uint16_t LoopTimes );

  // overrides
  virtual GifComponent* Clone() const override; 
  virtual void Read ( std::istream& ) override;
  virtual void Write( std::ostream& ) const override;

private:
  std::string m_AppInfo;  // Application ID and Authentication Code
  U8String    m_AppData;  // Application data
};

#endif // GifApplicationExt_h
