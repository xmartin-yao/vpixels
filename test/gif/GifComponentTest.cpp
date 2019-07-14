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

#include "GifComponentTest.h"
#include "GifComponent.h"
#include "GifApplicationExt.h"
#include "GifCommentExt.h"
#include "GifGraphicsControlExt.h"
#include "GifImageDescriptor.h"
#include "GifPlainTextExt.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GifComponentTest );

void GifComponentTest::testCreate()
{
  std::shared_ptr<GifComponent> pComponent;

  // GifImageDescriptor
  pComponent = GifComponent::Create( 0x00 );
  CPPUNIT_ASSERT( typeid( *pComponent ) == typeid( GifImageDescriptor ) );
  CPPUNIT_ASSERT( pComponent->ID() == 0x2C );
  CPPUNIT_ASSERT( pComponent->Label() == 0x00 );

  pComponent = GifComponent::Create( 2, 5, 5 );
  CPPUNIT_ASSERT( typeid( *pComponent ) == typeid( GifImageDescriptor ) );
  CPPUNIT_ASSERT( pComponent->ID() == 0x2C );
  CPPUNIT_ASSERT( pComponent->Label() == 0x00 );


  // GifApplicationExt
  pComponent = GifComponent::Create( 0xFF );
  CPPUNIT_ASSERT( typeid( *pComponent ) == typeid( GifApplicationExt ) );
  CPPUNIT_ASSERT( pComponent->ID() == 0x21 );
  CPPUNIT_ASSERT( pComponent->Label() == 0xFF );

  // GifCommentExt
  pComponent = GifComponent::Create( 0xFE );
  CPPUNIT_ASSERT( typeid( *pComponent ) == typeid( GifCommentExt ) );
  CPPUNIT_ASSERT( pComponent->ID() == 0x21 );
  CPPUNIT_ASSERT( pComponent->Label() == 0xFE );

  // GifGraphicsControlExt
  pComponent = GifComponent::Create( 0xF9 );
  CPPUNIT_ASSERT( typeid( *pComponent ) == typeid( GifGraphicsControlExt ) );
  CPPUNIT_ASSERT( pComponent->ID() == 0x21 );
  CPPUNIT_ASSERT( pComponent->Label() == 0xF9 );

  // GifPlainTextExt
  pComponent = GifComponent::Create( 0x01 );
  CPPUNIT_ASSERT( typeid( *pComponent ) == typeid( GifPlainTextExt ) );
  CPPUNIT_ASSERT( pComponent->ID() == 0x21 );
  CPPUNIT_ASSERT( pComponent->Label() == 0x01 );

  // not a GifComponent
  pComponent = GifComponent::Create( 0x05 );
  CPPUNIT_ASSERT( pComponent.get() == nullptr );
}
