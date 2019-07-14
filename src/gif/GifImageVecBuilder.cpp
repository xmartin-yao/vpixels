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

#include "GifImageVecBuilder.h"
#include "GifImage.h"
#include "GifImageImpl.h"
#include "GifGraphicsControlExt.h"
#include "GifImageDescriptor.h"


///////////////////////////////////
GifImageVecBuilder::GifImageVecBuilder()
 : m_State( BuilderState::NEW_IMAGE ),
   m_pImageImpl( nullptr )
{
}

///////////////////////////////
GifImageVecBuilder::~GifImageVecBuilder() = default;

//////////////////////////////////////////////////////
// Parse the GifComponentVec and build a GifImageVec. 
// Parsing is based on GIF89a specification, i.e. an image consists of
//     GifGraphicsControlExt
//     GifImageDescriptor
// GifGraphicsControlExt is optional.
//////////////////////////////////////////////////////////////////////
GifImageVec GifImageVecBuilder::operator()( const GifImpl& rGifImpl, 
                                            const GifComponentVec& ComponentVec )
{
  GifImageVec ImageVec;
  m_State = BuilderState::NEW_IMAGE;
  for( auto& pComponent : ComponentVec )
  {
    switch( m_State )
    {
      case BuilderState::NEW_IMAGE :
        OnNewImage( ImageVec, rGifImpl, pComponent );
        break;

      case BuilderState::GRAPH_CONTROL :
        OnGraphControl( ImageVec, pComponent );
        break;
    }
  }

  // incomplete data, ignore
  if( m_pImageImpl )
    m_pImageImpl.reset();

  return ImageVec;
}

////////////////////////////
// create a new GifImageImpl
// expect either GifGraphicsControlExt or GifImageDescriptor
//////////////////////////////////////////////////////////
void GifImageVecBuilder::OnNewImage( GifImageVec& ImageVec, const GifImpl& rGifImpl,
                                     const GifComponentSP& pComponent )
{
  // a new GifImageImpl
  m_pImageImpl = std::make_unique<GifImageImpl>(rGifImpl);

  if( pComponent->Label() == 0xF9 )  // GifGraphicsControlExt
  {
    m_pImageImpl->m_pGraphicsControlExt = std::dynamic_pointer_cast<GifGraphicsControlExt>(pComponent);  
    m_State = BuilderState::GRAPH_CONTROL;
  }
  else if( pComponent->ID() == 0x2C )  // GifImageDescriptor
  {
    m_pImageImpl->m_pImageDescriptor = std::dynamic_pointer_cast<GifImageDescriptor>(pComponent);
    ImageVec.push_back( std::make_unique<vp::GifImage>(std::move(m_pImageImpl)) );
    m_State = BuilderState::NEW_IMAGE;
  }
  else
    // this is not expected, could be ApplicationExt at the beginning of 
    // GifComponentVec or, if being read from a gif file, something else.
    m_pImageImpl.reset();
}

////////////////////////////
// expect GifImageDescriptor
/////////////////////////////////////////////////////////////
void GifImageVecBuilder::OnGraphControl( GifImageVec& ImageVec, const GifComponentSP& pComponent )
{
  if( pComponent->ID() == 0x2C )  // GifImageDescriptor
  {
    m_pImageImpl->m_pImageDescriptor = std::dynamic_pointer_cast<GifImageDescriptor>(pComponent);
    ImageVec.push_back( std::make_unique<vp::GifImage>(std::move(m_pImageImpl)) );
    m_State = BuilderState::NEW_IMAGE;
  }
  else if( pComponent->Label() == 0xF9 )  // GifGraphicsControlExt
  {
    // encounter GifGraphicsControlExt again, 
    // error recovery: ignore previous GifGraphicsControlExt, 
    //                 connect the current, keep state
    m_pImageImpl->m_pGraphicsControlExt = std::dynamic_pointer_cast<GifGraphicsControlExt>(pComponent);   
  }
}
