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

// List information of a GIF file. 

#include <iostream>
#include <string> 
#include <vp/Gif.h>
#include <vp/GifImage.h>
#include <vp/Exception.h>

///////////////////
// rip off path and get program name
//////////////////////////////////////
std::string ProgName( char* argv[] )
{
  std::string FullPath = argv[0];
  size_t pos;
  if( (pos = FullPath.rfind("\\")) != std::string::npos || 
      (pos = FullPath.rfind("/")) != std::string::npos )
    return FullPath.substr(pos+1);
  else
    return FullPath;
}

///////////////////////
int Usage( char *argv[] )
{
  std::cout << vp::Gif::PackageVersion() << std::endl
            << "Copyright (C) 2019 Xueyi Yao" << std::endl
            << "License: GNU GPL version 3 or later" << std::endl << std::endl
            << "Usage: " << ProgName( argv ) << " FILE" << std::endl;
  return 1;
}

///////////////////////////////
int main( int argc, char *argv[] )
{
  if( argc < 2 )
    return Usage( argv );

  std::cout << "===================================" << std::endl;

  // import GIF file
  std::string FileName( argv[1] );
  vp::Gif gif;
  try
  {
    if( !gif.Import( FileName ) )
    {
      std::cout << "Failed to open file: " << FileName << std::endl;
      return 1;
    }
  }
  catch( const vp::Exception& e )
  {
    std::cout << "Failed to import file: " << FileName << std::endl;
    std::cout << e.what() << std::endl;
    return 1;
  }

  // info
  std::cout << "Version: " << gif.Version() << std::endl
            << "Bits/pixel: " << static_cast<uint16_t>(gif.BitsPerPixel()) << std::endl
            << "Dimension: " << gif.Width() << " x " << gif.Height() << std::endl
            << "Number of images: " << gif.Images() << std::endl
            << "Color table size: " << gif.ColorTableSize() << std::endl;

  // info of images
  for( size_t i = 0; i < gif.Images(); ++i )
  {
    std::cout << "------------------------" << std::endl;

    vp::GifImage& image = gif[i];
    std::cout << "Image: " << i << std::endl
              << "Bits/pixel: " << static_cast<uint16_t>(image.BitsPerPixel()) << std::endl
              << "Origin: " << "(" << image.Left() << ", " << image.Top() << ")" << std::endl
              << "Dimension: " << image.Width() << " x " << image.Height() << std::endl
              << "Delay = " << image.Delay() << std::endl
              << "Color table size: " << image.ColorTableSize() << std::endl;
  }

  return 0;
}
