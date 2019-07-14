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

#include "GifComponent.h"
#include "GifApplicationExt.h"
#include "GifCommentExt.h"
#include "GifGraphicsControlExt.h"
#include "GifImageDescriptor.h"
#include "GifPlainTextExt.h"

//////////////////////////////////////////////////
std::shared_ptr<GifComponent> GifComponent::Create( const uint8_t Label )
{
  std::shared_ptr<GifComponent> pComponent;
  switch( Label )
  {
    case 0x00 :
      pComponent.reset( new GifImageDescriptor() );
      break;

    case 0xFF :
      pComponent.reset( new GifApplicationExt() );
      break;

    case 0xF9 :
      pComponent.reset( new GifGraphicsControlExt() );
      break;

    case 0xFE :
      pComponent.reset( new GifCommentExt() );
      break;

    case 0x01 :
      pComponent.reset( new GifPlainTextExt() );
  }

  return pComponent;
}

///////////////////
// create a GifImageDescriptor
////////////////////////////////////////////////////////
std::shared_ptr<GifComponent>
GifComponent::Create( const uint8_t BitsPerPixel,
                      const uint16_t Width, const uint16_t Height,
                      const uint16_t Left, const uint16_t Top,
                      const bool LocalColorTable )
{
  return std::shared_ptr<GifComponent>( new GifImageDescriptor(BitsPerPixel, Width, Height,
                                                               Left, Top, LocalColorTable) );
}
