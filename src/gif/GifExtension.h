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

#ifndef GifExtension_h
#define GifExtension_h

#include "GifComponent.h"

/////////////////////
// interface for all GIF extensions
////////////////////////////////////////////
class GifExtension : public GifComponent
{
public:
  explicit GifExtension( const uint8_t& Label )
   : GifComponent(0x21), m_Label(Label) {}

  virtual ~GifExtension() = default;

  // not implemented
  GifExtension( const GifExtension& other ) = delete;
  GifExtension( GifExtension&& ) = delete;
  GifExtension& operator=( const GifExtension& ) = delete;
  GifExtension& operator=( GifExtension&& ) = delete;

  virtual uint8_t Label() const override { return m_Label; }

protected:
  uint8_t  m_Label;  // label of GIF extension
};

#endif //GifExtension_h


