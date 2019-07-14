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

#include "GifComponentVecUtil.h"
#include "GifComponent.h"
#include "GifCommentExt.h"
#include "config.h"
#include <istream>


////////////////////////////////////
// multi-image
//   +- ApplicationExt 
//   +- (image 0)
//   |    + GraphicsControlExt
//   |    + ImageDescriptor
//   +- (image 1)
//   |    + GraphicsControlExt
//   |    + ImageDescriptor
//   :
//    
// single-image
//   +- ImageDescriptor
///////////////////////////////////////////////////////////////////
GifComponentVec GifComponentVecUtil::Create( const uint8_t BitsPerPixel,
                                             const uint16_t Width, const uint16_t Height,
                                             const size_t Images, 
                                             const bool GlobalColorTable )
{
  GifComponentVec ComponentVec;

  if( Images > 1 ) // multiple images
  {
    ComponentVec.reserve( 1 + 2*Images );

    // GifApplicationExt
    ComponentVec.push_back( GifComponent::Create(0xFF) );
    for( size_t i = 0; i < Images; ++i )
    {
      // GifGraphicsControlExt
      ComponentVec.push_back( GifComponent::Create(0xF9) );
      // GifImageDescriptor
      ComponentVec.push_back( GifComponent::Create( BitsPerPixel, Width, Height,
                                                    0, 0, !GlobalColorTable) );
    }
  }
  else // single image 
    // GifImageDescriptor
    ComponentVec.push_back( GifComponent::Create( BitsPerPixel, Width, Height,
                                                  0, 0, !GlobalColorTable ) );

  return ComponentVec;
}

/////////////////////////////////////////////////////////
GifComponentVec GifComponentVecUtil::Create( const GifComponentVec& other )
{
  GifComponentVec ComponentVec;
  ComponentVec.reserve( other.size() );
  for( auto& pComponent : other ) 
    ComponentVec.push_back( std::shared_ptr<GifComponent>(pComponent->Clone()) );

  return ComponentVec;
}

///////////////////////////////////////////////////////
uint8_t GifComponentVecUtil::Read( std::istream& is, GifComponentVec& ComponentVec )
{
  // empty it, before reading
  ComponentVec.clear();

  // read from istream
  uint8_t ID = is.get();
  std::shared_ptr<GifComponent> pComponent;
  while( ID == 0x2C || ID == 0x21 )
  {
    if( ID == 0x2C )  // ImageDescriptor
    {
      pComponent = GifComponent::Create( 0x00 );
    }
    else if( ID == 0x21 )  // Extension
    {
      uint8_t Label = is.get();
      pComponent = GifComponent::Create( Label );
    }

    if( pComponent )
    {
      pComponent->Read( is );
	    ComponentVec.push_back( std::move(pComponent) );
    }

    ID = is.get();
  }

  return ID;  // expect Gif Trailer
}

namespace
{
  /////////////////////////////////////////////
  void AddComment( GifComponentVec& ComponentVec )
  {
    // comment: package name and version
    std::string Comment{"Built with "};
    Comment += PACKAGE_STRING;

    auto pComponent = ComponentVec.back(); // the last component
    if( pComponent->Label() == 0xFE ) // a GifCommentExt
    {
      // modify
      auto pCommentExt = dynamic_cast<GifCommentExt*>(pComponent.get());
      if( pCommentExt != nullptr && pCommentExt->Comment() != Comment )
        pCommentExt->Comment( Comment );
    }
    else // add a new GifCommentExt
    {
      GifCommentExt* pCommentExt = new GifCommentExt();
      pCommentExt->Comment( Comment );
      ComponentVec.push_back( std::shared_ptr<GifComponent>(pCommentExt) );
    }
  }
}

////////////////////////////////////////////////////////////
void GifComponentVecUtil::Write( std::ostream& os, GifComponentVec& ComponentVec )
{
  AddComment( ComponentVec );

  for( auto& pComponent : ComponentVec )
    pComponent->Write( os );
}
