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

// Extract images from a GIF file.

#include <iostream>
#include <string> 
#include <cmath>
#include <vp/Gif.h>
#include <vp/GifImage.h>
#include <vp/Exception.h>

//////////////
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

//////////
// generate file name
/////////////////////////////////////////
std::string GenName( const std::string& NameBase, const uint8_t width,
                     const size_t index )
{
  std::stringstream Digits;
  Digits.fill('0');
  Digits.width(width);
  Digits << index;

  return NameBase + "-" + Digits.str() + ".gif";
}

/////////////////
// Correct bpp.
// In some GIF files, the resolution bits (for bpp) and the size bits
// (for global color table size) are inconsistent. if the value of 
// resolution bits are less than that of size bits, and if using bpp
// returned by Gif::BitsPerPixel() to create a new Gif, the newly
// created gif will has less color entries in its global color table.
//////////////////////////////////////////////////////////////////////
uint8_t CorrectBpp( const vp::Gif& gif )
{
  auto Bpp = gif.BitsPerPixel();
  if( gif.ColorTable() && (1 << Bpp) < gif.ColorTableSize() ) // resolution bits < size bits 
  {
    // correct bpp using color table size
    auto Size = gif.ColorTableSize();
    Bpp = 0;
    while( Size != 1 )
    {
      Size >>= 1;
      Bpp++;
    }
  }

  return Bpp;
}

///////////////////
// extract image of the given index i
////////////////////////////////////////////
bool Extract( const vp::Gif& gif, uint8_t Bpp, size_t& i, const std::string& FileName )
{
  const vp::GifImage& img = gif[i];
  bool GlobalColor = gif.ColorTable();
  if( !vp::Gif::IsSupported(Bpp) || !GlobalColor )
    Bpp = img.BitsPerPixel(); // use bpp of the image

  if( img.ColorTable() )
    GlobalColor = false;

  // new GIF
  vp::Gif newgif( Bpp, gif.Width(), gif.Height(), 1, GlobalColor );

  // copy global color table
  if( GlobalColor )
  {
    newgif.ColorTableSize( gif.ColorTableSize() );
    for( uint16_t j = 0; j < gif.ColorTableSize(); ++j )
    {
      auto Index = static_cast<uint8_t>(j);
      uint8_t R, G, B;
      gif.GetColorTable( Index, R, G, B );
      newgif.SetColorTable( Index, R, G, B );
    }
  }

  try
  {
    // copy image
    newgif[0] = img;

    // export to a GIF file
    newgif.Export( FileName, true );

    return true;
  }
  catch( const vp::Exception& e )
  {
    std::cout << e.what() << std::endl;
    return false;
  }
}

///////////////////////////////
int main( int argc, char *argv[] )
{
  if( argc < 2 )
    return Usage( argv );

  std::cout << "===================================" << std::endl;

  std::string FileName = argv[1];
  std::string NameBase;
  size_t pos = FileName.rfind( "." );
  if( pos == std::string::npos )
    NameBase = FileName;
  else
    NameBase = FileName.substr( 0, pos );

  // import GIF file
  vp::Gif gif;
  try
  {
    if( !gif.Import( FileName ) )
    {
      std::cout << "Failed to open file: " << FileName << std::endl;
      return 1;
    }
  }
  catch( vp::Exception e )
  {
    std::cout << "Failed to import file: " << FileName << std::endl
              << e.what() << std::endl;
    return 1;
  }

  // info
  std::cout << "Bits/pixel: " << static_cast<uint16_t>(gif.BitsPerPixel()) << std::endl
            << "Dimension: " << gif.Width() << " x " << gif.Height() << std::endl
            << "Color table size: " << gif.ColorTableSize() << std::endl;

  auto images = gif.Images();
  std::cout << "Number of images: " << images << std::endl << std::flush;

  // only one image
  if( images == 1 )
    return 1;

  // extract images
  int Ret = 0;
  auto Bpp = CorrectBpp( gif );
  auto nDigits = static_cast<uint8_t>(floor(log10(images) + 1));
  for( size_t i = 0; i < images; ++i )
  {
    if( !Extract( gif, Bpp, i, GenName(NameBase, nDigits, i) ) )
      Ret = 1;

    // progressing
    std::cout << "." << std::flush;
  }

  std::cout << std::endl;

  return Ret;
}
