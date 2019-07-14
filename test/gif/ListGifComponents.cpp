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

#include <iostream>
#include "GifImpl.h"
#include "GifImage.h"
#include "Exception.h"
#include "GifApplicationExt.h"
#include "GifCommentExt.h"
#include "GifGraphicsControlExt.h"
#include "GifImageDescriptor.h"
#include "GifPlainTextExt.h"

int main( int argc, char *argv[] )
{
  if( argc < 2 )
  {
    std::cout << "Need file name." << std::endl;
    return 1;
  }

  std::string FileName( argv[1] );

  // import GIF file
  GifImpl gifimpl;
  try
  {
    if( !gifimpl.Import( FileName ) )
    {
      std::cout << "Failed to open: " << FileName << std::endl;
      return 1;
    }
  }
  catch( const vp::Exception& e )
  {
    std::cout << e.what() << std::endl;
    return 1;
  }

  std::cout << "=======================" << std::endl;
  std::cout << "File: " << FileName << std::endl;
  std::cout << "Images: " << gifimpl.Images() << std::endl;

  // list components
  for( auto& pComponent : gifimpl.Components() )
  {
    switch( pComponent->Label() )
    {
      case 0x00 :
        if( pComponent->ID() == 0x2C )
          std::cout << "ImageDescriptor" << std::endl;
        else
          std::cout << "Unknown component (ID: "
                    << static_cast<uint16_t>(pComponent->ID()) << ")" << std::endl;  
        break;

      case 0xFF :
        std::cout << "ApplicationExt: "
                  << dynamic_cast<GifApplicationExt*>(pComponent.get())->AppID() << " "
                  << dynamic_cast<GifApplicationExt*>(pComponent.get())->AuthCode() << std::endl;
        break;

      case 0xF9 :
        std::cout << "GraphicsControlExt" << std::endl;
        break;

      case 0xFE :
        std::cout << "CommentExt: "
                  << dynamic_cast<GifCommentExt*>(pComponent.get())->Comment() << std::endl;
        break;

      case 0x01 :
        std::cout << "PlainTextExt: "
                  << dynamic_cast<GifPlainTextExt*>(pComponent.get())->Text() << std::endl;
        break;

      default :
        std::cout << "Unknown component (Label: "
                  << static_cast<uint16_t>(pComponent->Label()) << ")" << std::endl;
    }
  }

  return 0;
}
