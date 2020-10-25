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

#ifndef GifComponent_h
#define GifComponent_h

#include <cstdint>
#include <iosfwd>
#include <memory> 

/////////////////////////
// Interface for Gif components
////////////////////////////////////////////////
class GifComponent
{
public:
  // factory method to create a concrete component
  static std::shared_ptr<GifComponent> Create( const uint8_t Label );

  // TO DO: put argument LocalColorTable before Left and Top
  // factory method to create ImageDescriptor
  static std::shared_ptr<GifComponent> Create( const uint8_t BitsPerPixel,
                                               const uint16_t Width,
                                               const uint16_t Height,
                                               const uint16_t Left = 0,
                                               const uint16_t Top = 0,
                                               const bool LocalColorTable = false );

  virtual ~GifComponent() = default;

  // not implemented
  GifComponent( const GifComponent& ) = delete;
  GifComponent( GifComponent&& ) = delete;
  GifComponent& operator=( const GifComponent& ) = delete;
  GifComponent& operator=( GifComponent&& ) = delete;

  uint8_t ID() const { return m_ID; }

  virtual uint8_t Label() const { return 0x00; }

  // virtual copy
  virtual GifComponent* Clone() const = 0; 

  // I/O
  virtual void Read( std::istream& ) = 0;
  virtual void Write( std::ostream& ) const = 0;

protected:
  // use factory method to instantiate GifComponent's subclasses
  explicit GifComponent( uint8_t ID ) : m_ID( ID ) {}

protected:
  uint8_t m_ID;  // Introducer of GIF components
};

#endif // GifComponent_h
