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

#ifndef GifPlainTextExt_h
#define GifPlainTextExt_h

#include "GifExtension.h"
#include "U8String.h"

/////////////////////////////////////
class GifPlainTextExt : public GifExtension
{
public:
  GifPlainTextExt();
  GifPlainTextExt( const GifPlainTextExt& other );

  virtual ~GifPlainTextExt() {}

  // not implemented
  GifPlainTextExt( GifPlainTextExt&& ) = delete;
  GifPlainTextExt& operator=( const GifPlainTextExt& ) = delete;
  GifPlainTextExt& operator=( GifPlainTextExt&& ) = delete;

  std::string& Text() { return m_Text; }

  // overrides
  virtual GifComponent* Clone() const override; 
  virtual void Read( std::istream& ) override;
  virtual void Write( std::ostream& ) const override;

private:
  U8String    m_Data;
  std::string m_Text;
};


#endif //GifPlainTextExt_h
