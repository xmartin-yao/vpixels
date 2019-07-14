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

#ifndef GifImageVecBuilder_h
#define GifImageVecBuilder_h

#include <cstdint>
#include "VecDefs.h"

// forward
struct GifImpl;
struct GifImageImpl;


///////////////////////////
// functor that builds a GifImageVec from a GifComponentVec
///////////////////////////////////////////////////////////////
class GifImageVecBuilder
{
public:
  GifImageVecBuilder();
  ~GifImageVecBuilder();

  GifImageVec operator()( const GifImpl&, const GifComponentVec& );

  // not implemented
  GifImageVecBuilder( const GifImageVecBuilder& ) = delete;
  GifImageVecBuilder( GifImageVecBuilder&& ) = delete;
  GifImageVecBuilder& operator=( const GifImageVecBuilder& ) = delete;
  GifImageVecBuilder& operator=( GifImageVecBuilder&& ) = delete;

private:
  enum class BuilderState : uint8_t { NEW_IMAGE, GRAPH_CONTROL };

  void OnNewImage( GifImageVec&, const GifImpl&, const GifComponentSP& );
  void OnGraphControl( GifImageVec&, const GifComponentSP& );

  BuilderState m_State;
  std::unique_ptr<GifImageImpl> m_pImageImpl;
};

#endif //GifImageVecBuilder_h
