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

#ifndef GifCommentExt_h
#define GifCommentExt_h

#include "GifExtension.h"
#include <string>

//////////////////////////////////
class GifCommentExt : public GifExtension
{
public:
  GifCommentExt();
  GifCommentExt( const GifCommentExt& other );

  // not implemented
  GifCommentExt( GifCommentExt&& ) = delete;
  GifCommentExt& operator=( const GifCommentExt& ) = delete;
  GifCommentExt& operator=( GifCommentExt&& ) = delete;

  virtual ~GifCommentExt() = default;

  void Comment( const std::string& Comment ) { m_Comment = Comment; }
  std::string Comment() const                { return m_Comment; }

  // overrides
  virtual GifComponent* Clone() const override; 
  virtual void Read( std::istream& ) override;
  virtual void Write( std::ostream& ) const override;

private:
  std::string m_Comment;
};

#endif //GifCommentExt_h
