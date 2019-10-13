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

// Down size a GIF file.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>     // std::tie
#include <utility>   // std::pair, std::make_pair
#include <algorithm> // std::sort, std::fill_n
#include <vp/Gif.h>
#include <vp/GifImage.h>
#include <vp/Bmp.h>
#include <vp/Exception.h>

////////////////////
// Rip off path and get program name
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
            << "Usage: " << ProgName( argv ) << " FILE [OUTPUT_FILE]" << std::endl;

  return 1;
}

////////////////////
// Generate output file name: filename.gif -> filename-downsized.gif
////////////////////////////////////////////////////////////////
std::string GenOutFileName( const std::string& InFileName )
{
  std::string NameBase;
  size_t pos = InFileName.rfind( "." );
  if( pos == std::string::npos )
    NameBase = InFileName;
  else
    NameBase = InFileName.substr( 0, pos );

  return( NameBase + "-downsized.gif" );
}

/////////////////////////
// To be used with std::set
//////////////////////////////////////
struct RGB
{
  uint8_t R, G, B;

  // this < other, std::set needs this operator
  bool operator<( const RGB& other ) const
  {
    return std::tie(R, G, B) < std::tie(other.R, other.G, other.B);
  }
};

/////////////////////////////////
// Check if set1 contains set2.
// operator>= in C++ STL does lexicographical comparison,
// is not able fulfill the need.
/////////////////////////////////////////////////////////////////
template<typename T> 
static bool contain( const std::set<T>& set1, const std::set<T>& set2 )
{
  // set2 is empty
  if( set2.size() == 0 )
    return false;

  // set1 is smaller than set2
  if( set1.size() < set2.size() )
    return false;

  // verify all elements in set2
  for( auto& element : set2 )
  {
    if( set1.find(element) == set1.end() )
      return false;
  }

  return true;
}

///////////////////////////////
// Simulate a piece of glass.
// It is used to store the transient status of the screen.
// Because a GIF may uses more than 256 colors, using 24-bit BMP
// as the base class simplifies the program.
///////////////////////////////////////////////////////////////////////
class Glass : public vp::Bmp
{
public:
  Glass( const uint16_t Width, const uint16_t Height );
  virtual ~Glass() = default;

  bool Transparent( const uint16_t X, const uint16_t Y ) const;
  bool HasTransPixel() const;
  void Erase();
  void Erase( const uint16_t X, const uint16_t Y );
  void SetPixel( const uint16_t X, const uint16_t Y,
                 const uint8_t Blue, const uint8_t Green, const uint8_t Red );

private:
  // coordinates of a pixel
  struct Pixel
  {
    uint16_t X, Y;

    Pixel( const uint16_t X = 0, const uint16_t Y = 0 );
    bool operator<( const Pixel& other ) const;
  };

  // transparent pixels
  std::set<Pixel> m_TransPixels;
};

//////////////////////////////////////
Glass::Pixel::Pixel( const uint16_t X, const uint16_t Y )
{
  this->X = X;
  this->Y = Y;
}

// this < other, std::set needs this operator
bool Glass::Pixel::operator<( const Pixel& other ) const
{
  return std::tie(X, Y) < std::tie(other.X, other.Y);
}

///////////////////////////////////////////////////
Glass::Glass( const uint16_t Width, const uint16_t Height )
  : vp::Bmp( 24, Width, Height )
{
  // all pixels are transparent by default
  Erase();
}

//////////////////////////
// Check if a pixel is transparent.
////////////////////////////////////////////////////////
bool Glass::Transparent( const uint16_t X, const uint16_t Y ) const
{
  Pixel pixel{X, Y};
  return( m_TransPixels.find( pixel ) != m_TransPixels.end() );
}

///////////////////
// Check if there's any transparent pixel.
////////////////////////////////////////////
bool Glass::HasTransPixel() const
{
  return( m_TransPixels.size() != 0 );
}

/////////////////////
// Set a pixel transparent.
///////////////////////////////////////////////
void Glass::Erase( const uint16_t X, const uint16_t Y )
{
  vp::Bmp::SetPixel( X, Y, 0, 0, 0 );

  Pixel pixel{X, Y};
  m_TransPixels.insert( pixel );
}

///////////////////
// Set all pixels transparent.
///////////////////////////////////////////////
void Glass::Erase()
{
  for( uint16_t Y = 0; Y < Height(); ++Y )
  {
    for( uint16_t X = 0; X < Width(); ++X )
      Erase( X, Y );
  }
}

/////////////////////
// Set a pixel opaque.
//////////////////////////////////////////
void Glass::SetPixel( const uint16_t X, const uint16_t Y,
                      const uint8_t Red, const uint8_t Green, const uint8_t Blue )
{
  vp::Bmp::SetPixel( X, Y, Red, Green, Blue );

  Pixel pixel{X, Y};
  const auto it = m_TransPixels.find( pixel );
  if( it != m_TransPixels.end() )
    m_TransPixels.erase( it );
}

/////////////////
// Callbacks
//////////////////////////////////////////
class Log
{
public:
  Log() = default;
  virtual ~Log() = default;

  virtual void Info( const std::string& info );
  virtual void Message( const std::string& msg );
  virtual void Error( const std::string& err );
  virtual void Progesss( const size_t& current, const size_t& total );
};

////////////////////////////
void Log::Info( const std::string& info )
{
  std::cout << info << std::flush;
}

////////////////////////////
void Log::Message( const std::string& msg )
{
  std::cout << msg << std::flush;
}

////////////////////////////
void Log::Error( const std::string& err )
{
  std::cerr << err << std::flush;
}

/////////////////////////////////////
void Log::Progesss( const size_t& current, const size_t& total )
{
  if( current < total)
    std::cout << "." << std::flush;
  else
    std::cout << "." << std::endl;
}

//////////////////////
// Downsize a GIF file.
/////////////////////////////////////////////////
class GifDownsizer
{
public:
  explicit GifDownsizer( Log* pLog );
  virtual ~GifDownsizer();

  bool operator()( const std::string& InFileName, const std::string& OutFileName );

private:
  void Init();
  bool Import( const std::string& FileName );

  // pre-process
  void CheckResolution();
  void CheckDimension();
  void CheckTransColor();
  void Remove1PixelImages();
  void RemoveDuplicates();

  // downsize single or multiple images
  void DownsizeSingleImage();
  void DownsizeMultiImages();

  // downsize color table(s)
  bool SingleColorTable();
  bool CommonColorTable();
  bool CommonColorTable( size_t& Index, size_t& Count );
  bool DownsizeLocalColorTables();
  bool DownsizeLocalColorTable( const size_t& Index );

  // utils related to color table
  std::set<RGB> ColorsInUse() const;
  std::set<RGB> ColorsInUse( const vp::GifImage& Img ) const;
  bool HasLocalColorTable() const;
  template<typename T>  
  void ResetImage( const size_t& Index, const T& CTableSrc );
  template<typename T>  
  void ResetTransColor( const size_t& Index, T& CTableSrc, const size_t& UsedColors );
  template<typename T>
  static void ResetColorTable( T& CTableSrc, const std::set<RGB>& Colors );
  template<typename T>
  static std::set<RGB> ColorsInTable( const T& CTableSrc );
  template<typename T>  
  static uint8_t RGBtoIndex( const uint8_t& R, const uint8_t& G, const uint8_t& B, const T& CTableSrc );
  static uint16_t Roundup( const size_t& Size );
  static uint8_t  SizeToBpp( uint16_t Size );

  // crop images
  bool CropImages1();
  bool CropImages2();

  // utils related to cropping image
  bool Method1();
  bool Overlay();
  static bool CropImage( const Glass& glass, vp::GifImage& Img );
  static uint16_t DiffTop( const Glass& glass, const vp::GifImage& Img );
  static uint16_t DiffLeft( const Glass& glass, const vp::GifImage& Img );
  static uint16_t DiffRight( const Glass& glass, const vp::GifImage& Img );
  static uint16_t DiffBottum( const Glass& glass, const vp::GifImage& Img );
  static bool Diff( const Glass& glass, const uint16_t Xg, const uint16_t Yg,
                    const vp::GifImage& Img, const uint16_t Xi, const uint16_t Yi );
  static void Blend( Glass& glass, const vp::GifImage& Img );
  static void Update( Glass& glass, vp::GifImage& Img );
  static bool Overlay( Glass& glass, const vp::GifImage& Img );
  static uint8_t AssignTransColor( vp::GifImage& Img );
  static uint32_t TransPixels( const vp::GifImage& Img );

  // reduce bpp
  void ReduceBpp();

  // utils
  static std::string Info( const vp::Gif* pGif );
  static size_t FileSize( const std::string& FileName );

  // member data
  vp::Gif* m_pGifSrc;
  vp::Gif* m_pGifDes;

  // callbacks
  Log* m_pLog;
};

////////////////////////////
GifDownsizer::GifDownsizer( Log* pLog )
 : m_pGifSrc(nullptr), m_pGifDes(nullptr), m_pLog(pLog)
{
}

//////////////////////////
GifDownsizer::~GifDownsizer()
{
  if( m_pGifSrc != nullptr )
  {
    delete m_pGifSrc;
    m_pGifSrc = nullptr;
  }

  if( m_pGifDes != nullptr )
  {
    delete m_pGifDes;
    m_pGifDes = nullptr;
  }
}

///////////////////////////////
// Find an image whose local color table contains colors that
// are most commonly used by other images.
// Return true, if found.
// Index: index of the image
// Count: number of images that can share the color table
//////////////////////////////////////////////////////////////
bool GifDownsizer::CommonColorTable( size_t& Index, size_t& Count )
{
  const auto nImages = m_pGifSrc->Images();
  std::vector<std::set<RGB>> ColorsVec;
  for( size_t i = 0; i < nImages; ++i )
  {
    std::set<RGB> Colors_i = ColorsInTable( (*m_pGifSrc)[i] );
    ColorsVec.push_back( std::move(Colors_i) );
  }

  // pair: Index and Count
  // Index of an image
  // Count of other images that can share color table
  std::vector<std::pair<size_t, size_t>> CommonCount;
  std::vector<bool> Label(nImages, false);
  for( size_t i = 0; i < nImages - 1; ++i )
  {
    // i-th already processed
    if( Label[i] ) continue;

    size_t Count = 0;
    for( size_t j = i + 1; j < nImages; ++j )
    {
      // j-th already processed
      if( Label[j] ) continue;

      // j-th is able to share the color table
      if( contain( ColorsVec[i], ColorsVec[j] ) )
      {
        Count++;
        Label[j] = true;  // label it processed
      }
    }

    CommonCount.push_back( std::make_pair(i, Count) );
  }

  // sort in descending order based on counts
  std::sort( CommonCount.begin(), CommonCount.end(),
             [](const std::pair<size_t,size_t>& Left, const std::pair<size_t,size_t>& Right)
               { return Left.second > Right.second; } );

  Index = CommonCount[0].first;  // index of the image that has common color table
  Count = CommonCount[0].second; // count of other images that can share the color table

  return( Count != 0 );
}

/////////////////////////////////
// Check if a pixel of Img is different from that of glass.
///////////////////////////////////////////////////////////////
bool GifDownsizer::Diff( const Glass& glass, const uint16_t Xg, const uint16_t Yg,
                         const vp::GifImage& Img, const uint16_t Xi, const uint16_t Yi )
{
  // if the pixel of Img is transparent,
  // consider the two pixels are not different
  if( Img.Transparent( Xi, Yi ) )
    return false;

  // if the pixel of Img is not transparent and the pixel of glass is,
  // consider the two pixels are different
  if( glass.Transparent( Xg, Yg ) )
    return true;

  // compare RGB, when both are not transparent
  uint8_t Ri, Gi, Bi, Rg, Gg, Bg;
  Img.GetPixel( Xi, Yi, Ri, Gi, Bi );
  glass.GetPixel( Xg, Yg, Rg, Gg, Bg );
  return( Ri != Rg || Gi != Gg || Bi != Bg );
}

////////////////////////////////////
// Scan from bottom to top and find the row where Img and glass
// differ from each other.
////////////////////////////////////////////////////////////////////////////
uint16_t GifDownsizer::DiffBottum( const Glass& glass, const vp::GifImage& Img )
{
  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t Y = Img.Height(); Y-- != 0; )
  {
    auto Yg = Y + Y0;
    for( uint16_t X = 0; X < Img.Width(); ++X )
    {
      auto Xg = X + X0;
      if( Diff( glass, Xg, Yg, Img, X, Y ) )
        return Yg;
    }
  }

  // Img is the same as the area in glass,
  // return Y of its origin and cropping will result in a 1-pixel image
  return Y0;
}

////////////////////////////////////
// Scan from right to left and find the column where glass and Img
// differ from each other.
/////////////////////////////////////////////////////////////////////////
uint16_t GifDownsizer::DiffRight( const Glass& glass, const vp::GifImage& Img )
{
  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t X = Img.Width(); X-- != 0; )
  {
    auto Xg = X + X0;
    for( uint16_t Y = 0; Y < Img.Height(); ++Y )
    {
      auto Yg = Y + Y0;
      if( Diff( glass, Xg, Yg, Img, X, Y ) )
        return Xg;
    }
  }

  // Img is the same as the area of glass,
  // return X of its origin and cropping will result in a 1-pixel image
  return X0;
}

////////////////////////////////////
// Scan from left to right and find the column where glass and Img
// differ from each other.
////////////////////////////////////////////////////////////////////////
uint16_t GifDownsizer::DiffLeft( const Glass& glass, const vp::GifImage& Img )
{
  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t X = 0; X < Img.Width(); ++X )
  {
    auto Xg = X + X0;
    for( uint16_t Y = 0; Y < Img.Height(); ++Y )
    {
      auto Yg = Y + Y0;
      if( Diff( glass, Xg, Yg, Img, X, Y ) )
        return Xg;
    }
  }

  // Img is the same as the area of glass,
  // return X of its origin and cropping will result in a 1-pixel image
  return X0;
}

////////////////////////////////////
// Scan from top to bottom and find the row where glass and Img
// differ from each other.
/////////////////////////////////////////////////////////////////////
uint16_t GifDownsizer::DiffTop( const Glass& glass, const vp::GifImage& Img )
{
  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t Y = 0; Y < Img.Height(); ++Y )
  {
    auto Yg = Y + Y0;
    for( uint16_t X = 0; X < Img.Width(); ++X )
    {
      auto Xg = X + X0;
      if( Diff( glass, Xg, Yg, Img, X, Y ) )
        return Yg;
    }
  }

  // Img is the same as the area of glass,
  // return Y of its origin and cropping will result in a 1-pixel image
  return Y0;
}

/////////////////////////////////
// Crop images that can use disposal method 1.
/////////////////////////////////////////////////
bool GifDownsizer::CropImages1()
{
  // 1st image
  vp::GifImage& Img0 = (*m_pGifDes)[0];
  Img0.DisposalMethod( 1 );

  // blend 1st image into glass
  Glass glass( m_pGifSrc->Width(), m_pGifSrc->Height() );
  Blend( glass, Img0 );

  // crop other images
  auto Cropped = false;
  for( size_t i = 1; i < m_pGifSrc->Images(); ++i )
  {
    m_pLog->Progesss( i, m_pGifSrc->Images() - 1 );

    // crop Img
    vp::GifImage& Img = (*m_pGifDes)[i];
    Cropped = CropImage( glass, Img ) || Cropped;

    // count transparent pixels, before update
    auto Before = TransPixels( Img );

    // update glass and Img
    Update( glass, Img );
    Img.DisposalMethod( 1 );

    // count transparent pixels, after update
    auto After = TransPixels( Img );

    Cropped = ( After > Before ) || Cropped;
  }

  return Cropped;
}

///////////////////
// Crop images that can use disposal method 2.
/////////////////////////////////////////////////
bool GifDownsizer::CropImages2()
{
  const Glass glass( m_pGifSrc->Width(), m_pGifSrc->Height() );
  auto Cropped = false;
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    m_pLog->Progesss( i, m_pGifSrc->Images() - 1 );

    vp::GifImage& Img = (*m_pGifDes)[i];
    Cropped = CropImage( glass, Img ) || Cropped;
    Img.DisposalMethod( 2 );
  }

  return Cropped;
}

/////////////////////
// Crop an image.
//////////////////////////////////////////////////////////
bool GifDownsizer::CropImage( const Glass& glass, vp::GifImage& Img )
{
  // find edges
  uint16_t Top = DiffTop( glass, Img );
  uint16_t Left = DiffLeft( glass, Img );
  uint16_t Width = DiffRight( glass, Img ) - Left + 1;
  uint16_t Height = DiffBottum( glass, Img ) - Top + 1;

  // crop, if any edge shrinks
  if( Left > Img.Left() || Top > Img.Top() || 
      Width < Img.Width() || Height < Img.Height() )
  {
    Img.Crop( Left, Top, Width, Height );
    return true;
  }
  else
    return false;
}

////////////////
// Blend Img into glass.
//////////////////////////////////////////////////
void GifDownsizer::Blend( Glass& glass, const vp::GifImage& Img )
{
  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t Y = 0; Y < Img.Height(); ++Y )
  {
    auto Yg = Y + Y0;
    for( uint16_t X = 0; X < Img.Width(); ++X )
    {
      if( !Img.Transparent( X, Y ) )
      {
        auto Xg = X + X0;
        uint8_t R, G, B;
        Img.GetPixel( X, Y, R, G, B );
        glass.SetPixel( Xg, Yg, R, G, B );
      }
    }
  }
}

//////////////////
// Count number of transparent pixels.
//////////////////////////////////////////////////
uint32_t GifDownsizer::TransPixels( const vp::GifImage& Img )
{
  uint32_t Count = 0;
  for( uint16_t Y = 0; Y < Img.Height(); ++Y )
  {
    for( uint16_t X = 0; X < Img.Width(); ++X )
    {
      if( Img.Transparent( X, Y ) )
        ++Count;
    }
  }

  return Count;
}

////////////////////////
// Blend Img into glass and set Img's pixels that are same
// as those of glass to transparent.
/////////////////////////////////////////////////////////////
void GifDownsizer::Update( Glass& glass, vp::GifImage& Img )
{
  auto TransColor = AssignTransColor( Img );
  auto UseTransColor = false;

  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t Y = 0; Y < Img.Height(); ++Y )
  {
    auto Yg = Y + Y0;
    for( uint16_t X = 0; X < Img.Width(); ++X )
    {
      auto Xg = X + X0;
      if( Diff( glass, Xg, Yg, Img, X, Y ) )
      {
        // if the two pixels are different, blend to glass
        uint8_t R, G, B;
        Img.GetPixel( X, Y, R, G, B );
        glass.SetPixel( Xg, Yg, R, G, B );
      }
      else if( Img.HasTransColor() )
      {
        // if the two pixels are same and Img has transparent color,
        // set the pixel transparent
        Img.SetPixel( X, Y, TransColor );
        UseTransColor = true;
      }
    }
  }

  // transparent color is not in use, turn it off
  if( !UseTransColor )
    Img.HasTransColor( false );
}

///////////////////
// Check if disposal method 1 is used.
///////////////////////////////////////////////
bool GifDownsizer::Method1()
{
  // 1st and last may be set to 0
  for( size_t i = 1; i < m_pGifSrc->Images() - 1; ++i )
  {
    if( (*m_pGifSrc)[i].DisposalMethod() != 1 )
      return false;
  }

  return true;
}

//////////////////////////
// Check if images overlay one after another.
/////////////////////////////////////////////////
bool GifDownsizer::Overlay()
{
  // if 1st image can fully cover the screen,
  // images that follow will be able to overlay previous ones
  vp::GifImage& Img0 = (*m_pGifSrc)[0];
  if( !Img0.HasTransColor() && Img0.Left() == 0 && Img0.Top() == 0 &&
      Img0.Width() == m_pGifSrc->Width() && Img0.Height() == m_pGifSrc->Height() )
    return true;

  // if glass contains no transparent pixel after 1st image
  // being blended into it, pixels(transparent or not) of images
  // that follow will be able to overlay pixels of glass
  Glass glass( m_pGifSrc->Width(), m_pGifSrc->Height() );
  Blend( glass, Img0 );  // blend 1st image into glass
  if( !glass.HasTransPixel() )
    return true;

  // check images other than 1st one
  for( size_t i = 1; i < m_pGifSrc->Images(); ++i )
  {
    vp::GifImage& Img = (*m_pGifSrc)[i];
    if( !Overlay( glass, Img ) )
      return false;
  }

  return true;
}

//////////////////
// Check if Img just overlays the area of glass.
////////////////////////////////////////////////////////////
bool GifDownsizer::Overlay( Glass& glass, const vp::GifImage& Img )
{
  // if a pixel of Img is not transparent, while the pixel of glass is,
  // Img contributes an extra pixel, so consider it does not right
  // overlay previous images.
  auto X0 = Img.Left();
  auto Y0 = Img.Top();
  for( uint16_t Y = 0; Y < Img.Height(); ++Y )
  {
    auto Yg = Y + Y0;
    for( uint16_t X = 0; X < Img.Width(); ++X )
    {
      auto Xg = X + X0;
      if( !Img.Transparent( X, Y ) )
      {
        if( glass.Transparent( Xg, Yg ) )
          return false;

        // update glass
        uint8_t R, G, B;
        Img.GetPixel( X, Y, R, G, B );
        glass.SetPixel( Xg, Yg, R, G, B );
      }
    }
  }

  return true;
}

/////////////////////////
// Assign transparent color to Img, if it doesn't have one.
///////////////////////////////////////////////////////////
uint8_t GifDownsizer::AssignTransColor( vp::GifImage& Img )
{
  if( Img.HasTransColor() )
    return Img.TransColor();

  // count usage of each color index
  auto TableSize = Img.CheckColorTable();
  std::unique_ptr<uint32_t[]> Hits(new uint32_t[TableSize]);
  std::fill_n( Hits.get(), TableSize, 0 );
  for( uint16_t y = 0; y < Img.Height(); ++y )
  {
    for( uint16_t x = 0; x < Img.Width(); ++x )
      ++Hits[Img.GetPixel( x, y )];
  }

  // find the less used color
  uint8_t  IndexMin = 0;
  uint32_t HitsMin = Hits[0];
  for( uint16_t i = 1; i < TableSize; ++i )
  {
    if( HitsMin > Hits[i] )
    {
      HitsMin = Hits[i];
      IndexMin = static_cast<uint8_t>( i );
    }
  }

  // use it as transparent color, if its usage is zero or
  // less than 1/10000 of the total pixels
  if( HitsMin == 0 || 
      static_cast<double>(HitsMin)/Img.Width()/Img.Height() < 1.0e-4 )
  {
    Img.TransColor( IndexMin );
  }

  return IndexMin;
}

/////////////////////////////
// The goal is to find a local color table that is most commonly
// used by other images and to replace the global color table.
////////////////////////////////////////////////////////////////
bool GifDownsizer::CommonColorTable()
{
  // find most commonly used local color table
  size_t Index, Count;
  if( !CommonColorTable( Index, Count ) )
    return false;

  // number of images that use global color table
  size_t Usage = 0; 
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    if( !(*m_pGifSrc)[i].ColorTable() )
      ++Usage;
  }

  // replacing global color table will result in more space use
  if( m_pGifSrc->ColorTableSize()*Usage > (*m_pGifSrc)[Index].ColorTableSize()*Count )
    return false;

  // replace global color table with the common local color table
  std::set<RGB> ColorsCommon = ColorsInTable( (*m_pGifSrc)[Index] );
  ResetColorTable( *m_pGifDes, ColorsCommon );

  // colors in old global color table
  std::set<RGB> ColorsInOldGTable = ColorsInTable( *m_pGifSrc );

  // process all images
  for( size_t i = 0; i < m_pGifDes->Images(); ++i )
  {
    m_pLog->Progesss( i, m_pGifDes->Images() - 1 );

    vp::GifImage& ImgSrc = (*m_pGifSrc)[i];
    vp::GifImage& ImgDes = (*m_pGifDes)[i];
    if( ImgSrc.ColorTable() )  // image has local color table
    {
      std::set<RGB> ColorsInTable_i = ColorsInTable( ImgSrc );

      // common color table contains the local color table
      if( contain( ColorsCommon, ColorsInTable_i ) ) 
      {
        // disable the local color table
        // reset the image using new global color table
        ImgDes.ColorTableSize( 0 );
        ResetTransColor( i, *m_pGifDes, ColorsCommon.size() );
        ResetImage( i, *m_pGifDes );
      }
      else
        DownsizeLocalColorTable( i );
    }
    else  // image doesn't have local color table
    {
      // enable local color table and copy old global color table
      // reset image using new local color table
      ResetColorTable( ImgDes, ColorsInOldGTable );
      ResetTransColor( i, ImgDes, ColorsInOldGTable.size() );
      ResetImage( i, ImgDes );
    }
  }

  return true;
}

/////////////////////////////
// Collect colors in color table.
// CTableSrc: vp::Gif or vp::GifImage object that contains color table
////////////////////////////////////////////////////////////////////
template<typename T>
std::set<RGB> GifDownsizer::ColorsInTable( const T& CTableSrc )
{
  std::set<RGB> Colors;
  for( uint16_t i = 0; i < CTableSrc.ColorTableSize(); ++i )
  {
    RGB rgb;
    CTableSrc.GetColorTable( static_cast<uint8_t>(i), rgb.R, rgb.G, rgb.B );
    Colors.insert( rgb );
  }

  return Colors;
}

/////////////////////////
// Collect colors used by Img.
//////////////////////////////////////////////////////
std::set<RGB> GifDownsizer::ColorsInUse( const vp::GifImage& Img ) const
{
  RGB rgb;
  std::set<RGB> Colors;
  for( uint16_t y = 0; y < Img.Height(); ++y )
  {
    for( uint16_t x = 0; x < Img.Width(); ++x )
    {
      Img.GetPixel( x, y, rgb.R, rgb.G, rgb.B );
      Colors.insert( rgb );
    }
  }

  if( Img.HasTransColor() )
  {
    Img.TransColor( rgb.R, rgb.G, rgb.B );
    Colors.insert( rgb );
  }

  return Colors;
}

/////////////////////////
// Collect colors used by all images.
/////////////////////////////////////////
std::set<RGB> GifDownsizer::ColorsInUse() const
{
  std::set<RGB> Colors;
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    std::set<RGB> Colors_i = ColorsInUse( (*m_pGifSrc)[i] );
    Colors.insert( Colors_i.begin(), Colors_i.end() );
  }

  return Colors;
}

////////////////////////
// Check if any image has local color table.
///////////////////////////////////////////////
bool GifDownsizer::HasLocalColorTable() const
{
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    if( (*m_pGifSrc)[i].ColorTable() )
      return true;
  }

  return false;
}

/////////////////////////////
// Reduce local color table size of an image.
// Index: index of the image
// pre-condition: the image has local color table
///////////////////////////////////////////////////////
bool GifDownsizer::DownsizeLocalColorTable( const size_t& Index )
{
  vp::GifImage& ImgDes = (*m_pGifDes)[Index];

  // colors in use
  std::set<RGB> Colors = ColorsInUse( ImgDes );

  // color table size will not change
  if( Roundup( Colors.size() ) >= ImgDes.ColorTableSize() )
    return false;

  // reset color table with colors actually in use
  // reset image accordingly
  ResetColorTable( ImgDes, Colors );
  ResetTransColor( Index, ImgDes, Colors.size() );
  ResetImage( Index, ImgDes );

  return true;
}

/////////////////////////////
// The goal is to reduce sizes of all local color tables
// and to disable global color table if it is not in use.
///////////////////////////////////////////////////////
bool GifDownsizer::DownsizeLocalColorTables()
{
  auto GTableInUse = false;
  auto Downsized = false;
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    m_pLog->Progesss( i, m_pGifDes->Images() - 1 );

    if( (*m_pGifSrc)[i].ColorTable() )
      Downsized = DownsizeLocalColorTable( i ) || Downsized;
    else
      GTableInUse = true;
  }

  // disable global color table if it is not in use
  if( m_pGifSrc->ColorTable() && !GTableInUse )
  {
    m_pGifDes->ColorTableSize( 0 );
    Downsized = true;
  }

  return Downsized;
}

//////////////////////////////////
// The goal is to form a global color table that can accommodate
// all colors in use and to disable all local color tables.
/////////////////////////////////////////////////////////////////
bool GifDownsizer::SingleColorTable()
{
  // colors in use
  std::set<RGB> Colors = ColorsInUse();

  // exceed the capacity of a color table
  if( Colors.size() > 256 )
    return false;

  // global color table size is not changed and there's no local color table,
  if( Roundup( Colors.size() ) == m_pGifSrc->ColorTableSize() && 
      !HasLocalColorTable() )
    return false; 

  // reset global color table and images
  ResetColorTable( *m_pGifDes, Colors );
  for( size_t i = 0; i < m_pGifDes->Images(); ++i )
  {
    m_pLog->Progesss( i, m_pGifDes->Images() - 1 );

    // disable local color table
    // reset image using new global color table
    (*m_pGifDes)[i].ColorTableSize( 0 );
    ResetTransColor( i, *m_pGifDes, Colors.size() );
    ResetImage( i, *m_pGifDes );
  }

  return true;
}

////////////////////////////
// Reset color table.
// CTableSrc: vp::Gif or vp::GifImage whose color table to be reset
// Colors: colors used to reset the color table
////////////////////////////////////////////////////////////////////////
template<typename T>
void GifDownsizer::ResetColorTable( T& CTableSrc, const std::set<RGB>& Colors )
{
  CTableSrc.ColorTableSize( static_cast<uint16_t>(Colors.size()) );

  // reset entries
  uint16_t i = 0;
  for( auto& rgb : Colors )
  {
    CTableSrc.SetColorTable( static_cast<uint8_t>(i), rgb.R, rgb.G, rgb.B );
    ++i;
  }

  // fill remaining entries with white color
  while( i < CTableSrc.ColorTableSize() )
  {
    CTableSrc.SetColorTable( static_cast<uint8_t>(i), 255, 255, 255 );
    ++i;
  }
}

/////////////////////////
// Reset an image.
// Index: index of the image
// CTableSrc: vp::Gif or vp::GifImage object whose color table
//            is used to reset the image
////////////////////////////////////////////////////////////////
template<typename T>  
void GifDownsizer::ResetImage( const size_t& Index, const T& CTableSrc )
{
  vp::GifImage& ImgSrc = (*m_pGifSrc)[Index];
  vp::GifImage& ImgDes = (*m_pGifDes)[Index];
  for( uint16_t y = 0; y < ImgSrc.Height(); ++y )
  {
    for( uint16_t x = 0; x < ImgSrc.Width(); ++x )
    {
      if( ImgSrc.Transparent( x, y ) )
      {
        ImgDes.SetPixel( x, y, ImgDes.TransColor() );
      }
      else
      {
        uint8_t R, G, B;
        ImgSrc.GetPixel( x, y, R, G, B );
        ImgDes.SetPixel( x, y, RGBtoIndex( R, G, B, CTableSrc ) );
      }
    }
  }
}

//////////////////////////////////
// Reset transparent color of an image.
// Index: index of the image
// CTableSrc: vp::Gif or vp::GifImage object that contains color table
// UsedColors: number of colors in use
//////////////////////////////////////////////////////////////////
template<typename T>  
void GifDownsizer::ResetTransColor( const size_t& Index, T& CTableSrc,
                                    const size_t& UsedColors )
{
  vp::GifImage& ImgSrc = (*m_pGifSrc)[Index];
  vp::GifImage& ImgDes = (*m_pGifDes)[Index];

  // image has no transparent color
  if( !ImgSrc.HasTransColor() )
    return;

  bool Failed = true;
  uint8_t TransColorIndex = 0;
  uint16_t TableSize = CTableSrc.ColorTableSize();
  if( TableSize > UsedColors )
  {
    // color table has spare entry,
    // pick the last entry as transparent color.
    TransColorIndex = TableSize - 1;
    Failed = false;
  }
  else
  {
    // count usage of each color index
    std::unique_ptr<uint32_t[]> Hits(new uint32_t[TableSize]);
    std::fill_n( Hits.get(), TableSize, 0 );
    for( uint16_t y = 0; y < ImgSrc.Height(); ++y )
    {
      for( uint16_t x = 0; x < ImgSrc.Width(); ++x )
      {
        if( !ImgSrc.Transparent( x, y ) )
        {
          uint8_t R, G, B;
          ImgSrc.GetPixel( x, y, R, G, B );
          ++Hits[RGBtoIndex( R, G, B, CTableSrc )];
        }
      }
    }

    // pick the one that is not in use as transparent color.
    for( uint16_t i = TableSize; i-- != 0; )
    {
      if( Hits[i] == 0 )
      {
        TransColorIndex = static_cast<uint8_t>(i);
        Failed = false;
        break;
      }
    }
  }

  // if failed to set TransColorIndex, increase color table size
  // and pick the last entry as transparent color.
  // this happens, b/c some GIF files do not use a different color as 
  // transparent color, which results in a smaller UsedColors.
  if( Failed )
  {
    CTableSrc.ColorTableSize( TableSize + 1 );
    TransColorIndex = CTableSrc.ColorTableSize() - 1;
  }

  // reset transparent color
  ImgDes.TransColor( TransColorIndex );
}

//////////////////////////
// Downsize GIF that contains multiple images.
///////////////////////////////////////////////
void GifDownsizer::DownsizeMultiImages()
{
  // crop images
  auto Cropped = false;
  if( Method1() || Overlay() )
    Cropped = CropImages1();
  else
    Cropped = CropImages2();

  if( Cropped )
  {
    delete m_pGifSrc;
    m_pGifSrc = new vp::Gif( *m_pGifDes );
  }

  // downsize color tables
  SingleColorTable() || CommonColorTable() || DownsizeLocalColorTables();

  // reduce bits/pixel
  ReduceBpp();
}

////////////////////////////
// Find the index of RGB in color table.
// CTableSrc: vp::Gif or vp::GifImage object whose color table
//            is to be searched for the index
// pre-condition: RGB is in color table
/////////////////////////////////////////////////////////////
template<typename T>
uint8_t GifDownsizer::RGBtoIndex( const uint8_t& R, const uint8_t& G,
                                  const uint8_t& B, const T& CTableSrc  )
{
  uint16_t i = 0;
  while( i < CTableSrc.ColorTableSize() )
  {
    uint8_t Ri, Gi, Bi;
    CTableSrc.GetColorTable( static_cast<uint8_t>(i), Ri, Gi, Bi );
    if( Ri == R && Gi == G && Bi == B )
      break;

    ++i;
  }

  return static_cast<uint8_t>(i);
}

//////////////////////////
// Downsize GIF that contains one image.
//////////////////////////////////////////
void GifDownsizer::DownsizeSingleImage()
{
  vp::GifImage& ImgSrc = (*m_pGifSrc)[0];
  if( ImgSrc.ColorTable() )  // image has local color table
  {
    // disable global color table
    // downsize local color table
    m_pGifDes->ColorTableSize( 0 );
    DownsizeLocalColorTable( 0 );
  }
  else  // image doesn't have local color table
  {
    // colors in use
    std::set<RGB> ColorsUsed = ColorsInUse( ImgSrc );

    // global color table size will not change
    if( Roundup( ColorsUsed.size() ) == m_pGifSrc->ColorTableSize() )
      return;

    // reset global color table and the image
    ResetColorTable( *m_pGifDes, ColorsUsed );
    ResetImage( 0, *m_pGifDes );
  }
}

//////////////
// Initialize member data
///////////////////////////
void GifDownsizer::Init()
{
  if( m_pGifSrc != nullptr )
    delete m_pGifSrc;

  m_pGifSrc = new vp::Gif();

  if( m_pGifDes != nullptr )
  {
    delete m_pGifDes;
    m_pGifDes = nullptr;
  }
}

/////////////////
// Display information of vp::Gif
////////////////////////////////////////////
std::string GifDownsizer::Info( const vp::Gif* pGif )
{
  std::stringstream sstrm;
  sstrm << "-----------------------------------\n"
        << "Bits/pixel: " << static_cast<uint16_t>(pGif->BitsPerPixel()) << "\n"
        << "Dimension: " << pGif->Width() << " x " << pGif->Height() << "\n"
        << "Color table size: " << pGif->ColorTableSize() << "\n"
        << "Number of images: " << pGif->Images() << "\n"
        << "-----------------------------------\n";

 return sstrm.str();
}

/////////////////////
// Roundup Size to power of 2
/////////////////////////////////////////////////
uint16_t GifDownsizer::Roundup( const size_t& Size )
{
  // special cases
  if( Size == 0 ) return 0;
  if( Size == 1 ) return 2;

  uint16_t Ret = 1;
  while( Ret < Size ) Ret *= 2;

  return Ret;
}

///////////////////////////
// Color table size to bits/pixel.
// pre-condition: Size is power of 2
///////////////////////////////////////////////
uint8_t GifDownsizer::SizeToBpp( uint16_t Size )
{
  // special case
  if( Size == 2 ) return 2;

  uint8_t Count = 0;
  while( Size != 1 )
  {
    Size >>= 1;
    ++Count;
  }

  return Count;
}

/////////////////////
// consistency: resolution and color table size
////////////////////////////////////////////////
void GifDownsizer::CheckResolution()
{
  if( m_pGifSrc->ColorTable() )
  {
    auto Size = m_pGifSrc->ColorTableSize();
    if( Size != (1 << m_pGifSrc->BitsPerPixel()) )
      m_pGifSrc->BitsPerPixel( SizeToBpp(Size) );
  }

  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    vp::GifImage& Img = (*m_pGifSrc)[i];
    auto Size = Img.CheckColorTable();
    if( Size != (1 << Img.BitsPerPixel()) )
      Img.BitsPerPixel( SizeToBpp(Size) );
  }
}

//////////////////////
// consistency: origin and dimension of image
////////////////////////////////////////////////
void GifDownsizer::CheckDimension()
{
  auto Width = m_pGifSrc->Width();
  auto Height = m_pGifSrc->Height();
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    vp::GifImage& Img = (*m_pGifSrc)[i];
    auto X0 = Img.Left();
    auto Y0 = Img.Top();
    auto Wi = Img.Width();
    auto Hi = Img.Height();
    if( X0 + Wi > Width || Y0 + Hi > Height )
      Img.Crop( X0, Y0, Width - X0, Height - Y0 );
  }
}

//////////////////////////////
// consistency: transparent color
// if transparent color is not in use, turn it off
//////////////////////////////////////////////////////
void GifDownsizer::CheckTransColor()
{
  for( size_t i = 0; i < m_pGifSrc->Images(); ++i )
  {
    vp::GifImage& Img = (*m_pGifSrc)[i];
    if( Img.HasTransColor() )
    {
      auto TransPixel = false;
      for( uint16_t y = 0; y < Img.Height(); ++y )
      {
        for( uint16_t x = 0; x < Img.Width(); ++x )
        {
          if( Img.Transparent( x, y ) )
          {
            TransPixel = true;
            break;
          }
        }

        if( TransPixel ) break;
      }

      if( !TransPixel )
        Img.HasTransColor( false );
    }
  }
}

////////////////////////////////
// Decrease color resolution of images if possible.
// Some images may use a small portion of the color table, 
// decreasing its color resolution results less storage space.
///////////////////////////////////////////////////////////
void GifDownsizer::ReduceBpp()
{
  for( size_t i = 0; i < m_pGifDes->Images(); ++i )
  {
    // find maximum color index
    uint8_t ColorIndex = 0;
    uint8_t Max = 0;
    vp::GifImage& Img = (*m_pGifDes)[i];
    for( uint16_t y = 0; y < Img.Height(); ++y )
    {
      for( uint16_t x = 0; x < Img.Width(); ++x )
      {
        ColorIndex = Img.GetPixel( x, y );
        if( ColorIndex > Max )
          Max = ColorIndex;
      }
    }

    auto Bpp = SizeToBpp( Roundup( Max + 1 ) );
    if( Bpp != Img.BitsPerPixel() )
      Img.BitsPerPixel( Bpp );
  }
}

//////////////////////
// Remove consecutively duplicated images.
// Some GIF files use duplicated images to prolong delay time.
///////////////////////////////////////////////////////////////
void GifDownsizer::RemoveDuplicates()
{
  // count number of images,
  // excluding consecutively duplicated ones
  auto nImages = m_pGifSrc->Images();
  std::vector<bool> Label(nImages, false);
  size_t i = 0;
  size_t j = 1;
  while( j < m_pGifSrc->Images() )
  {
    m_pLog->Progesss( j, m_pGifSrc->Images() - 1 );

    // j-th image is duplicated
    if( (*m_pGifSrc)[i] == (*m_pGifSrc)[j] )
    {
      --nImages;       // reduce number of images
      Label[j] = true; // label duplicated
      ++j;
    }
    else
    {
      // next consecutive pair
      i = j;
      j = i + 1;
    }
  }

  // no duplicated image
  if( nImages == m_pGifSrc->Images() )
    return;

  // new GIF
  vp::Gif* pGif = new vp::Gif( m_pGifSrc->BitsPerPixel(), m_pGifSrc->Width(),
                               m_pGifSrc->Height(), nImages, m_pGifSrc->ColorTable() );

  // copy color table
  for( uint16_t i = 0; i < m_pGifSrc->ColorTableSize(); ++i )
  {
    uint8_t R, G, B;
    m_pGifSrc->GetColorTable( static_cast<uint8_t>(i), R, G, B );
    pGif->SetColorTable( static_cast<uint8_t>(i), R, G, B );
  }

  // copy images, excluding duplicated ones
  (*pGif)[0] = (*m_pGifSrc)[0];  // copy 1st image
  for( j = 0, i = 1; i < m_pGifSrc->Images(); ++i )
  {
    if( Label[i] )  // duplicated image
    {
      // add delay time
      (*pGif)[j].Delay( (*pGif)[j].Delay() + (*m_pGifSrc)[i].Delay() );
    }
    else
    {
      ++j;
      (*pGif)[j] = (*m_pGifSrc)[i];  // copy image
    }
  }

  delete m_pGifSrc;
  m_pGifSrc = pGif;
}

/////////////////////
// Remove 1-pixel images.
// Some GIF files use 1-pixel images to prolong delay time.
//////////////////////////////////////////////////////////
void GifDownsizer::Remove1PixelImages()
{
  // count number of images, excluding 1-pixel ones
  auto nImages = m_pGifSrc->Images();
  std::vector<bool> Label(nImages, false);
  for( size_t i = 1; i < m_pGifSrc->Images(); ++i )
  {
    // i-th is a 1-pixel image
    vp::GifImage& Img = (*m_pGifSrc)[i] ;
    if( Img.Width() == 1 && Img.Height() == 1 )
    {
      --nImages;       // reduce number of images
      Label[i] = true; // label it 1-pixel image
    }
  }

  // no 1-pixel image
  if( nImages == m_pGifSrc->Images() )
    return;

  // new GIF
  vp::Gif* pGif = new vp::Gif( m_pGifSrc->BitsPerPixel(), m_pGifSrc->Width(),
                               m_pGifSrc->Height(), nImages, m_pGifSrc->ColorTable() );

  // copy color table
  for( uint16_t i = 0; i < m_pGifSrc->ColorTableSize(); ++i )
  {
    uint8_t R, G, B;
    m_pGifSrc->GetColorTable( static_cast<uint8_t>(i), R, G, B );
    pGif->SetColorTable( static_cast<uint8_t>(i), R, G, B );
  }

  // copy images, excluding 1-pixel ones
  (*pGif)[0] = (*m_pGifSrc)[0];  // copy 1st image
  for( size_t j = 0, i = 1; i < m_pGifSrc->Images(); ++i )
  {
    if( Label[i] )  // 1-pixel image
    {
      // add delay time
      (*pGif)[j].Delay( (*pGif)[j].Delay() + (*m_pGifSrc)[i].Delay() );
    }
    else
    {
      ++j;
      (*pGif)[j] = (*m_pGifSrc)[i];  // copy image
    }
  }

  delete m_pGifSrc;
  m_pGifSrc = pGif;
}

/////////////////////
// Import a GIF file
///////////////////////////////
bool GifDownsizer::Import( const std::string& FileName )
{
  try
  {
    if( !m_pGifSrc->Import( FileName ) )
    {
      std::stringstream sstrm;
      sstrm << "Failed to open '" << FileName << "'\n";
      m_pLog->Error( sstrm.str() );

      return false;
    }
  }
  catch( const vp::Exception& e )
  {
    std::stringstream sstrm;
    sstrm << "Failed to import '" << FileName << "' (" << e.what() << ")\n";
    m_pLog->Error( sstrm.str() );

    return false;
  }

  return true;
}

//////////////////
// File size
///////////////////////////////////
size_t GifDownsizer::FileSize( const std::string& FileName )
{
  size_t Size = 0;
  std::ifstream File;
  File.open( FileName, std::ios::ate|std::ios::binary );
  if( File.is_open() )
  {
    Size = static_cast<size_t>( File.tellg() );
    File.close();
  }

  return Size;
}

///////////////////////////////////////
// Down size a GIF file and export to another GIF file
//////////////////////////////////////////////////////////////////////////
bool GifDownsizer::operator()( const std::string& InFileName, const std::string& OutFileName )
{
  // initialize
  Init();

  // import GIF file
  if( !Import( InFileName ) )
    return false;

  auto SizeBefore = FileSize( InFileName );

  // info
  m_pLog->Info( Info( m_pGifSrc ) ); 

  // pre-process
  CheckResolution();
  CheckDimension();
  Remove1PixelImages();
  RemoveDuplicates();

  // down size
  m_pGifDes = new vp::Gif( *m_pGifSrc );
  if( m_pGifSrc->Images() == 1 )
    DownsizeSingleImage();
  else
    DownsizeMultiImages();

  auto SizeAfter = m_pGifDes->Size();
  if( SizeBefore > SizeAfter )
  {
    // export to file
    m_pGifDes->Export( OutFileName, true );

    // info after downsizing
    m_pLog->Info( Info( m_pGifDes ) ); 
    std::stringstream sstrm;
    sstrm << "Size(bytes): " << SizeBefore << " -> "  << SizeAfter << "\n"
          << "Export to: " << OutFileName << "\n";
    m_pLog->Info( sstrm.str() );

    return true;
  }
  else
  {
    std::stringstream sstrm;
    sstrm << "File '" << InFileName << "' cannot be downsized."  << "\n";
    m_pLog->Message( sstrm.str() );

    return false;
  }
}

///////////////////////////////
int main( int argc, char *argv[] )
{
  if( argc < 2 )
    return Usage( argv );

  // input and output file name
  std::string InFileName( argv[1] );
  std::string OutFileName;
  if( argc < 3 )
    OutFileName = GenOutFileName( InFileName );
  else
    OutFileName = argv[2];

  Log callback;
  if( GifDownsizer( &callback )( InFileName, OutFileName ) )
    return 0;
  else
    return 1;
}
