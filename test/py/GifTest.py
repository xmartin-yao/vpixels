#######################################################################
# Copyright (C) 2019 Xueyi Yao
#
# This file is part of VPixels.
#
# VPixels is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# VPixels is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
#######################################################################

# vp.gif unit tests

import unittest
import sys

# import vpixels from current directory
sys_path = sys.path
if sys.platform == 'win32':
  sys.path = ['.\\']
else:
  sys.path = ['./']
import vpixels
sys.path = sys_path # restore default sys.path


class TestGif( unittest.TestCase ):
  # vpixels.gif() with no arg
  def testNoArg( self ):
    gif = vpixels.gif()
    self.assertEqual( 1, len(gif) )

    self.assertEqual( 2, gif.bitsperpixel() )
    self.assertEqual( 2, gif.bpp() )

    self.assertEqual( 1, gif.width() )
    self.assertEqual( 1, gif.height() )
    self.assertEqual( (1, 1), gif.dimension() )

    self.assertEqual( True, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 4, gif.colortablesize() )

    self.assertEqual( 1, gif.images() )
    self.assertRaises( Exception, gif.getimage, 1 )
    self.assertRaises( Exception, gif.getimage, -1 )


  # vpixels.gif() with 3 args
  def test3Args( self ):
    gif = vpixels.gif( 3, 5, 6 )  # 3 args
    self.assertEqual( 1, len(gif) )

    self.assertEqual( 3, gif.bitsperpixel() )
    self.assertEqual( 3, gif.bpp() )

    self.assertEqual( 5, gif.width() )
    self.assertEqual( 6, gif.height() )
    self.assertEqual( (5, 6), gif.dimension() )

    self.assertEqual( True, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 8, gif.colortablesize() )

    self.assertEqual( 1, gif.images() )
    self.assertRaises( ValueError, gif.getimage, 1 )


  # vpixels.gif() with 4 args
  def test4Args( self ):
    gif = vpixels.gif( 4, 5, 6, 7 )
    self.assertEqual( 7, len(gif) )

    self.assertEqual( 4, gif.bitsperpixel() )
    self.assertEqual( 4, gif.bpp() )

    self.assertEqual( 5, gif.width() )
    self.assertEqual( 6, gif.height() )
    self.assertEqual( (5, 6), gif.dimension() )

    self.assertEqual( True, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 16, gif.colortablesize() )

    self.assertEqual( 7, gif.images() )
    self.assertRaises( ValueError, gif.getimage, 8 )
    self.assertRaises( ValueError, gif.getimage, -1 )


  # vpixels.gif() with 5 args
  def test5Args( self ):
    # no global color table
    gif = vpixels.gif( 4, 5, 6, 7, False )
    self.assertEqual( 7, len(gif) )
    self.assertEqual( False, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 0, gif.colortablesize() )

    self.assertEqual( 4, gif.bitsperpixel() )
    self.assertEqual( 4, gif.bpp() )
    self.assertEqual( 5, gif.width() )
    self.assertEqual( 6, gif.height() )
    self.assertEqual( (5, 6), gif.dimension() )
    self.assertEqual( 7, gif.images() )

    # has global color table
    gif = vpixels.gif( 4, 5, 6, 7, True )
    self.assertEqual( True, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 16, gif.colortablesize() )

    self.assertEqual( 4, gif.bitsperpixel() )
    self.assertEqual( 4, gif.bpp() )
    self.assertEqual( 5, gif.width() )
    self.assertEqual( 6, gif.height() )
    self.assertEqual( (5, 6), gif.dimension() )
    self.assertEqual( 7, gif.images() )


  # vpixels.gif() with keywords
  def testKwords(self):
    gif = vpixels.gif( height = 7, bpp = 3, width = 9 )
    self.assertEqual( 1, len(gif) )
    self.assertEqual( 3, gif.bitsperpixel() )
    self.assertEqual( 3, gif.bpp() )
    self.assertEqual( 9, gif.width() )
    self.assertEqual( 7, gif.height() )
    self.assertEqual( (9, 7), gif.dimension() )
    self.assertEqual( True, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 8, gif.colortablesize() )
    self.assertEqual( 1, gif.images() )

    gif = vpixels.gif( 4, images = 3, height = 7, width = 9 )
    self.assertEqual( 3, len(gif) )
    self.assertEqual( 4, gif.bitsperpixel() )
    self.assertEqual( 4, gif.bpp() )
    self.assertEqual( 9, gif.width() )
    self.assertEqual( 7, gif.height() )
    self.assertEqual( (9, 7), gif.dimension() )
    self.assertEqual( True, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 16, gif.colortablesize() )
    self.assertEqual( 3, gif.images() )

    gif = vpixels.gif( 2, 7, 9, colortable = False )  # no global color
    self.assertEqual( 1, len(gif) )
    self.assertEqual( 2, gif.bitsperpixel() )
    self.assertEqual( 2, gif.bpp() )
    self.assertEqual( 7, gif.width() )
    self.assertEqual( 9, gif.height() )
    self.assertEqual( (7, 9), gif.dimension() )
    self.assertEqual( False, gif.colortable() )
    self.assertEqual( False, gif.colortablesorted() )
    self.assertEqual( 0, gif.colortablesize() )
    self.assertEqual( 1, gif.images() )
    self.assertEqual( True, gif[0].colortable() )   # has local color
    self.assertEqual( 4, gif[0].colortablesize() )


  # vpixels.gif() with bad args
  def testBadArgs( self ):
    self.assertRaises( TypeError, vpixels.gif, 2 )
    self.assertRaises( TypeError, vpixels.gif, 2.5, 3, 4 )
    self.assertRaises( ValueError, vpixels.gif, 1, 3, 4 )
    self.assertRaises( ValueError, vpixels.gif, 9, 3, 4 )
    self.assertRaises( OverflowError, vpixels.gif, -2, 3, 4 )
    self.assertRaises( ValueError, vpixels.gif, 2, -3, 4 )
    self.assertRaises( ValueError, vpixels.gif, 2, 3, -4 )
    self.assertRaises( OverflowError, vpixels.gif, -2, 3, 4, 5 )
    self.assertRaises( ValueError, vpixels.gif, 2, -3, 4, 5 )
    self.assertRaises( ValueError, vpixels.gif, 2, 3, -4, 5 )
    self.assertRaises( ValueError, vpixels.gif, 2, 3, 4, -5 )
    self.assertRaises( OverflowError, vpixels.gif, -2, 3, 4, 5, False )
    self.assertRaises( ValueError, vpixels.gif, 2, -3, 4, 5, False )
    self.assertRaises( ValueError, vpixels.gif, 2, 3, -4, 5, False )
    self.assertRaises( ValueError, vpixels.gif, 2, 3, 4, -5, False )
    self.assertRaises( TypeError, vpixels.gif, 2, 3, 4, 5, 8 )
    self.assertRaises( TypeError, vpixels.gif, height = 5, width = 6 )
    self.assertRaises( TypeError, vpixels.gif, 2, height = 5, width = 6, colortable = 4 )

  def testClone( self ):
    gif = vpixels.gif( 2, 3, 4, 5 )
    other = gif.clone()

    self.assertEqual( 2, other.bpp() )
    self.assertEqual( 3, other.width() )
    self.assertEqual( 4, other.height() )
    self.assertEqual( (3, 4), other.dimension() )

    self.assertEqual( True, other.colortable() )
    self.assertEqual( False, other.colortablesorted() )
    self.assertEqual( 4, other.colortablesize() )

    self.assertEqual( 5, other.images() )


  def testColorTable( self ):
    gif = vpixels.gif()
    # no color table
    self.assertEqual( True, gif.colortable() )
    self.assertEqual( 4, gif.colortablesize() )
    self.assertRaises( Exception, gif.setcolortable, 4, 0, 0, 0 )
    self.assertRaises( Exception, gif.getcolortable, 4 )
    # enable color table
    gif.colortablesize( 2 )
    self.assertEqual( 2, gif.colortablesize() )
    # default value 255
    self.assertEqual( (255, 255, 255), gif.getcolortable( 0 ) )
    gif.setcolortable(0, 25, 26, 27)
    self.assertEqual( (25, 26, 27), gif.getcolortable( 0 ) )
    # index exceeds limit
    self.assertRaises( ValueError, gif.setcolortable, 3, 25, 26, 27 )
    self.assertRaises( ValueError, gif.getcolortable, 3 )
    self.assertRaises( OverflowError,  gif.setcolortable, -1, 25, 26, 27 )
    self.assertRaises( OverflowError,  gif.getcolortable, -1 )
    # increase to maximum size
    gif.colortablesize( 4 )
    self.assertEqual( 4, gif.colortablesize() )
    # size exceeds limit
    self.assertRaises( Exception, gif.colortablesize, 5 )
    # disable color table
    gif.colortablesize( 0 )
    self.assertEqual( False, gif.colortable() )
    self.assertEqual( 0, gif.colortablesize() )
    self.assertRaises( Exception, gif.setcolortable, 0, 0, 0, 0 )
    self.assertRaises( Exception, gif.getcolortable, 0 )

    gif = vpixels.gif(4, 5, 6)
    # has color table
    self.assertEqual( True, gif.colortable() )
    self.assertEqual( 16, gif.colortablesize() )
    # default value 255
    self.assertEqual( (255, 255, 255), gif.getcolortable( 0 ) )
    gif.setcolortable(0, 25, 26, 27)
    self.assertEqual( (25, 26, 27), gif.getcolortable( 0 ) )
    # index exceeds limit
    self.assertRaises( ValueError, gif.setcolortable, 16, 25, 26, 27 )
    self.assertRaises( ValueError, gif.getcolortable, 16 )
    self.assertRaises( OverflowError,  gif.setcolortable, -1, 25, 26, 27 )
    self.assertRaises( OverflowError,  gif.getcolortable, -1 )
    # size exceeds limit
    self.assertRaises( ValueError, gif.colortablesize, 17 )
    # downsize 
    gif.colortablesize( 8 )
    self.assertEqual( True, gif.colortable() )
    self.assertEqual( 8, gif.colortablesize() )
    self.assertRaises( ValueError, gif.setcolortable, 8, 25, 26, 27 )
    self.assertRaises( ValueError, gif.getcolortable, 8 )
    # disable color table
    gif.colortablesize( 0 )
    self.assertEqual( False, gif.colortable() )
    self.assertEqual( 0, gif.colortablesize() )
    self.assertRaises( Exception, gif.setcolortable, 0, 0, 0, 0 )
    self.assertRaises( Exception, gif.getcolortable, 0 )


  def testBackgroundColor( self ):
    gif1 = vpixels.gif( 2, 3, 4, 5 )
    self.assertEqual( gif1.backgroundcolor(), 0 )
    gif1.backgroundcolor(1)
    self.assertEqual( gif1.backgroundcolor(), 1 )
    gif1.backgroundcolor(3)
    self.assertEqual( gif1.backgroundcolor(), 3 )
    self.assertRaises( ValueError, gif1.backgroundcolor, 4 )  # exceeds
    gif1.colortablesize(0)  # disable
    self.assertEqual( gif1.colortable(), False )
    self.assertEqual( gif1.backgroundcolor(), 0 )
    self.assertRaises( Exception, gif1.backgroundcolor, 0 )  # not allowed

    gif2 = vpixels.gif( 3, 4, 5, 3, False )
    self.assertEqual( gif2.colortable(), False )
    self.assertEqual( gif2.background(), 0 )
    gif2.colortablesize(4)  # enable
    self.assertEqual( gif2.colortable(), True )
    self.assertEqual( gif2.background(), 0 )
    gif2.background(2)
    self.assertEqual( gif2.background(), 2 )
    self.assertRaises( ValueError, gif2.background, 4 )  # exceeds


  def testImportf( self ):
    gif = vpixels.gif( 3, 4, 5, 2 )
    img0 = gif[0]
    img1 = gif[1]

    # import non-existing file
    self.assertRaises( IOError, gif.importf, 'not-exits.gif' ) # not exit
    # gif unchanged
    self.assertEqual( 3, gif.bpp() )
    self.assertEqual( (4, 5), gif.dimension() )
    self.assertEqual( 2, gif.images() )
    self.assertEqual( 2, len(gif) )
    # img0 and img1 are still valid
    self.assertEqual( 3, img0.bpp() )
    self.assertEqual( 3, img1.bpp() )

    # importf() expects a string argument
    self.assertRaises( TypeError, gif.importf, 2 ) # not a file name
    # gif unchanged
    self.assertEqual( 3, gif.bpp() )
    self.assertEqual( (4, 5), gif.dimension() )
    self.assertEqual( 2, gif.images() )
    self.assertEqual( 2, len(gif) )
    # img0 and img1 are still valid
    self.assertEqual( 3, img0.bpp() )
    self.assertEqual( 3, img1.bpp() )

    # import non-gif file or corrupted gif file
    self.assertRaises( Exception, gif.importf, 'GifTest.py' ) # not a gif file
    # gif still a valid object, but becomes bpp=2, 1x1, images=1, colors=4
    self.assertEqual( 2, gif.bpp() )
    self.assertEqual( (1, 1), gif.dimension() )
    self.assertEqual( 1, len(gif) )
    self.assertEqual( 4, gif.colortablesize() )
    # img0 and img1 are invalid, b/c the gif object they refer to is out of scope
    self.assertNotEqual( img0, None )
    self.assertNotEqual( img1, None )
    self.assertRaises( Exception, img0.bpp )
    self.assertRaises( Exception, img1.bpp )


  def testExport( self ):
    gif = vpixels.gif()
    gif.export( 'temp.gif', True ) # overwrite = True
    self.assertRaises( IOError, gif.export, 'temp.gif' ) # overwrite = False
    self.assertRaises( IOError, gif.export, 'temp.gif', False ) # overwrite = False


  def testInheritance( self ):
    class subgif( vpixels.gif ):
      def __init__( self ):
        super(subgif, self).__init__( 2, 3, 4 )

      def setcolor( self ):
        super(subgif, self).setcolor( 0, 2, 3, 4 )

    g = subgif()
    g.setcolor()
    self.assertEqual( (2, 3, 4), g.getcolor(0) )


class TestGifImage( unittest.TestCase ):
  def testOutOfScope(self):
    gif = vpixels.gif( 2, 3, 4, 5 )
    img0 = gif[0]
    img4 = gif[4]
    self.assertEqual( 2, img0.bpp() )
    self.assertEqual( 2, img4.bpp() )

    # assign to None, gif out of scope
    gif = None                              
    # img0 and img4 become invalid
    self.assertNotEqual( img0, None )
    self.assertNotEqual( img4, None )
    self.assertRaises( Exception, img0.bpp )
    self.assertRaises( Exception, img4.bpp )

    # assign to a new object
    gif = vpixels.gif( 3, 3, 4, 5 )
    # img0 and img4 are still invalid
    self.assertNotEqual( img0, None )
    self.assertNotEqual( img4, None )
    self.assertRaises( Exception, img0.bpp )
    self.assertRaises( Exception, img4.bpp )

    # assign img0 and img4 again
    img0 = gif[0]
    img4 = gif[4]
    self.assertEqual( 3, img0.bpp() )
    self.assertEqual( 3, img4.bpp() )

    # assign to another Gif object
    gif = vpixels.gif( 4, 5, 6 )
    # img0 and img4 become invalid, b/c the gif object they refer to is out of scope
    self.assertNotEqual( img0, None )
    self.assertNotEqual( img4, None )
    self.assertRaises( Exception, img0.bpp )
    self.assertRaises( Exception, img4.bpp )

    # clone() will fail, b/c img4 is invalid
    img0 = gif[0]
    self.assertRaises( Exception, img0.clone, img4 )
    self.assertRaises( Exception, img4.clone, img0 )


  # directly calling __new__() and __init__() is not allowed
  def testNewAndInit(self):
    gif = vpixels.gif( 2, 3, 4, 5 )
    img = gif[0]
    # type(img) and img.__class__ are same, they are type object of vp.gifimage
    self.assertRaises( Exception, img.__new__, type(img) )
    self.assertRaises( Exception, img.__new__, img.__class__ )
    self.assertRaises( Exception, img.__new__, type(gif) )
    self.assertRaises( Exception, img.__new__, type(3) )
    self.assertRaises( Exception, img.__init__ )


  def testMethodsNoArg(self):
    gif = vpixels.gif( 2, 3, 4, 5 )
    img = gif[0]

    self.assertEqual( 2, img.bitsperpixel() )
    self.assertEqual( 2, img.bpp() )
    self.assertEqual( 0, img.left() )
    self.assertEqual( 0, img.top() )
    self.assertEqual( 3, img.width() )
    self.assertEqual( 4, img.height() )
    self.assertEqual( (3, 4), img.dimension() )
    self.assertEqual( False, img.interlaced() )


  def testClone(self):
    gif = vpixels.gif( 2, 3, 4, 5 )
    img0 = gif[0]
    img0.setall( 3 )
    img1 = gif[1]
    self.assertEqual( 0, img1.getpixel( 1, 1 ) )
    img1.clone( img0 )
    self.assertEqual( 3, img1.getpixel( 1, 1 ) )

    # wrong args
    self.assertRaises( TypeError, img1.clone, 3 )
    self.assertRaises( TypeError, img1.clone, {} )

    # images belong to compatible gif objects
    gif2 = vpixels.gif( 2, 3, 4, 2 )
    img2 = gif2[0]
    img2.clone( img0 )
    self.assertEqual( img2.getpixel( 1, 1 ), 3 )

    # images belong to incompatible gif objects
    #gif3 = vpixels.gif( 3, 3, 4, 2 )  # different bpp
    #img3 = gif3[0]
    #self.assertRaises( Exception, img3.clone, img0 )

    gif4 = vpixels.gif( 2, 4, 4, 2 )  # different width
    img4 = gif4[0]
    self.assertRaises( Exception, img4.clone, img0 )

    gif5 = vpixels.gif( 2, 3, 3, 2 )  # different width
    img5 = gif5[0]
    self.assertRaises( Exception, img5.clone, img0 )


  def testPixels(self):
    gif = vpixels.gif(2, 3, 4, 5)
    img = gif[0]
    self.assertEqual( 0, img.getpixel(0, 0) )
    img.setall( 3 )
    self.assertEqual( 3, img.getpixel(0, 0) )
    img.setpixel( 1, 1, 2 )
    self.assertEqual( 2, img.getpixel(1, 1) )

    # wrong args
    self.assertRaises( OverflowError, img.setallpixels, -2 )
    self.assertRaises( ValueError, img.setallpixels, 4 )
    self.assertRaises( ValueError, img.setpixel, -1, 1, 2 )
    self.assertRaises( ValueError, img.setpixel, 3, 1, 2 )
    self.assertRaises( ValueError, img.getpixel, -1, 1 )
    self.assertRaises( ValueError, img.getpixel, 1, -1 )
    self.assertRaises( ValueError, img.getpixel, 3, 1 )
    self.assertRaises( ValueError, img.getpixel, 1, 4 )


  def testCrop(self):
    gif = vpixels.gif( 2, 8, 9, 2 )

    img = gif[0]
    self.assertEqual( 0, img.left() )
    self.assertEqual( 0, img.top() )
    self.assertEqual( 8, img.width() )
    self.assertEqual( 9, img.height() )

    img.crop( 2, 3, 4, 5 )
    self.assertEqual( 2, img.left() )
    self.assertEqual( 3, img.top() )
    self.assertEqual( 4, img.width() )
    self.assertEqual( 5, img.height() )

    img.crop( 3, 4, 3, 4 )
    self.assertEqual( 3, img.left() )
    self.assertEqual( 4, img.top() )
    self.assertEqual( 3, img.width() )
    self.assertEqual( 4, img.height() )

    # wrong arg value
    self.assertRaises( ValueError, img.crop, 2, 4, 1, 1 ) # 1st arg exceeds lower limit
    self.assertRaises( ValueError, img.crop, 6, 4, 1, 1 ) # 1st arg exceeds upper limit
    self.assertRaises( ValueError, img.crop, 3, 3, 1, 1 ) # 2nd arg exceeds lower limit
    self.assertRaises( ValueError, img.crop, 3, 8, 1, 1 ) # 2nd arg exceeds upper limit
    self.assertRaises( ValueError, img.crop, 4, 5, 0, 1 ) # 3rd arg = 0
    self.assertRaises( ValueError, img.crop, 4, 5, 3, 1 ) # 3rd arg exceeds limit
    self.assertRaises( ValueError, img.crop, 4, 5, 1, 0 ) # 4th arg = 0
    self.assertRaises( ValueError, img.crop, 4, 5, 1, 4 ) # 4th arg exceeds limit

    # wrong number of args
    img = gif[1]
    self.assertRaises( TypeError, img.crop, 2 )
    self.assertRaises( TypeError, img.crop, 2, 3 )
    self.assertRaises( TypeError, img.crop, 2, 3, 4 )

    # negative numbers
    self.assertRaises( ValueError, img.crop, -2, 3, 4, 5 )
    self.assertRaises( ValueError, img.crop, 2, -3, 4, 5 )
    self.assertRaises( ValueError, img.crop, 2, 3, -4, 5 )
    self.assertRaises( ValueError, img.crop, 2, 3, 4, -5 )

    # arg out of range
    self.assertRaises( ValueError, img.crop, 8, 0, 4, 5 )  # arg #1 exceeds 
    self.assertRaises( ValueError, img.crop, 0, 9, 4, 5 )  # arg #2 exceeds 
    self.assertRaises( ValueError, img.crop, 2, 3, 0, 5 )  # arg #3 == 0
    self.assertRaises( ValueError, img.crop, 2, 3, 4, 0 )  # arg #4 == 0
    self.assertRaises( ValueError, img.crop, 2, 3, 7, 5 )  # arg #3 exceeds 
    self.assertRaises( ValueError, img.crop, 2, 3, 4, 7 )  # arg #4 exceeds 


  def testDelay(self):
    gif1 = vpixels.gif( 2, 3, 4, 5 )
    img = gif1[0]
    self.assertEqual( 0, img.delay() )
    img.delay( 100 )
    self.assertEqual( 100, img.delay() )
    self.assertRaises( Exception, img.delay, -100 )

    # gif2 contains only one image, setting delay time not allowed
    gif2 = vpixels.gif( 2, 3, 4, 1 )
    img = gif2[0]
    self.assertRaises( Exception, img.delay )
    self.assertRaises( Exception, img.delay, 100 )


  def testDisposalMethod(self):
    gif1 = vpixels.gif( 2, 3, 4, 5 )
    img = gif1[0]
    self.assertEqual( 1, img.disposalmethod() )
    img.disposalmethod( 2 )
    self.assertEqual( 2, img.disposalmethod() )
    img.disposal( 3 )
    self.assertEqual( 3, img.disposal() )
    img.disposal( 0 )
    self.assertEqual( 0, img.disposal() )
    # value > 3
    self.assertRaises( Exception, img.disposalmethod, 4 )
    # value is not unsigned byte
    self.assertRaises( OverflowError, img.disposalmethod, -1 ) 
    self.assertRaises( OverflowError, img.disposalmethod, 256 )

    # gif2 contains only one image, setting delay time not allowed
    gif2 = vpixels.gif( 2, 3, 4, 1 )
    img = gif2[0]
    self.assertRaises( Exception, img.disposalmethod )
    self.assertRaises( Exception, img.disposalmethod, 2 )


  def testTransparentColor(self):
    gif1 = vpixels.gif( 2, 3, 4, 5 )
    img = gif1[0]
    # default
    self.assertEqual( False, img.hastransparentcolor() )
    self.assertEqual( 0, img.transparentcolor() )
    # turn on
    img.transparentcolor( 3 )
    self.assertEqual( True, img.hastransparentcolor() )
    self.assertEqual( 3, img.transparentcolor() )
    # turn off
    img.hastranscolor( False )
    self.assertEqual( False, img.hastranscolor() )
    self.assertEqual( 0, img.transcolor() )
    # turn on again
    img.hastranscolor( True )
    self.assertEqual( True, img.hastranscolor() )
    self.assertEqual( 0, img.transcolor() )
    # error cases
    self.assertRaises( TypeError, img.hastranscolor, 1 ) # not boolean
    self.assertRaises( ValueError, img.transcolor, 4 ) # exceed color table entry
    self.assertRaises( OverflowError, img.transcolor, -1 ) # exceed unsigned byte
    self.assertRaises( OverflowError, img.transcolor, 256 ) # exceed unsigned byte

    # gif2 contains only one image, transparent color not allowed
    gif2 = vpixels.gif( 2, 3, 4, 1 )
    img = gif2[0]
    self.assertRaises( Exception, img.hastranscolor )
    self.assertRaises( Exception, img.hastranscolor, True )
    self.assertRaises( Exception, img.transcolor )
    self.assertRaises( Exception, img.transcolor, 2 )


  def testColorTable(self):
    gif1 = vpixels.gif( 2, 3, 4 )
    img = gif1[0]

    # no color table by default
    self.assertEqual( False, img.colortable() )
    self.assertEqual( False, img.colortablesorted() )
    self.assertEqual( 0, img.colortablesize() )

    # enable color table
    img.colortablesize( 2 )
    self.assertEqual( True, img.colortable() )
    self.assertEqual( False, img.colortablesorted() )
    self.assertEqual( 2, img.colortablesize() )
    self.assertEqual( (255, 255, 255), img.getcolortable( 0 ) )
    self.assertEqual( (255, 255, 255), img.getcolortable( 1 ) )
    img.setcolortable( 1, 25, 26, 27 )
    self.assertEqual( (25, 26, 27), img.getcolortable( 1 ) )

    # increase to maximum size
    img.colortablesize( 4 )
    self.assertEqual( True, img.colortable() )
    self.assertEqual( 4, img.colortablesize() )

    # bad args
    self.assertRaises( Exception, img.colortablesize, -1 )
    self.assertRaises( Exception, img.colortablesize, 5 )  # max size = 4

    self.assertRaises( Exception, img.getcolortable, -1 )
    self.assertRaises( Exception, img.getcolortable, 4 )

    self.assertRaises( Exception, img.setcolortable, -1, 25, 26, 27 )
    self.assertRaises( Exception, img.setcolortable, 1, -25, 26, 27 )
    self.assertRaises( Exception, img.setcolortable, 1, 25, -26, 27 )
    self.assertRaises( Exception, img.setcolortable, 1, 25, 26, -27 )
    self.assertRaises( Exception, img.setcolortable, 1, 256, 26, 27 )
    self.assertRaises( Exception, img.setcolortable, 1, 25, 256, 27 )
    self.assertRaises( Exception, img.setcolortable, 1, 25, 26, 256 )


if __name__ == '__main__':
  unittest.main()
