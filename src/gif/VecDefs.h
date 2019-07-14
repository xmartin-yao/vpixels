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

#ifndef VecDefs_h
#define VecDefs_h

#include <vector>
#include <memory>

// forward
class GifComponent;
namespace vp { class GifImage; }


// define GifComponentVec
using GifComponentSP = std::shared_ptr<GifComponent>;
using GifComponentVec = std::vector<GifComponentSP>;

// define GifImageVec
using GifImageVec = std::vector<std::unique_ptr<vp::GifImage>>;

#endif  //VecDefs_h

