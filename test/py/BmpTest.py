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

# vp.bmp unit tests

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


class TestBmp( unittest.TestCase ):
  # vpixels.bmp() with no arg
  def testNoArg( self ):
    bmp = vpixels.bmp()
    self.assertEqual( 1, bmp.bitsperpixel() )
    self.assertEqual( 1, bmp.bpp() )
    self.assertEqual( 1, bmp.width() )
    self.assertEqual( 1, bmp.height() )
    self.assertEqual( (1, 1), bmp.dimension() )
    self.assertEqual( 2, bmp.colortablesize() )
    # default value 0
    self.assertEqual( (0, 0, 0), bmp.getcolortable( 0 ) )
    self.assertEqual( (0, 0, 0), bmp.getcolortable( 1 ) )
    bmp.setcolortable(0, 25, 26, 27)
    self.assertEqual( (25, 26, 27), bmp.getcolortable( 0 ) )
    # index exceeds limit
    self.assertRaises( ValueError, bmp.setcolortable, 2, 25, 26, 27 )
    self.assertRaises( ValueError, bmp.getcolortable, 2 )
    self.assertRaises( OverflowError, bmp.setcolortable, -1, 25, 26, 27 )
    self.assertRaises( OverflowError, bmp.getcolortable, -1 )


  # vpixels.bmp() with keywords
  def testKwords(self):
    bmp = vpixels.bmp( height = 7, bpp = 1, width = 9 )
    self.assertEqual( 1, bmp.bitsperpixel() )
    self.assertEqual( 1, bmp.bpp() )
    self.assertEqual( 9, bmp.width() )
    self.assertEqual( 7, bmp.height() )
    self.assertEqual( (9, 7), bmp.dimension() )
    self.assertEqual( 2, bmp.colortablesize() )

    bmp = vpixels.bmp( width = 7, bpp = 24, height = 9 )
    self.assertEqual( 24, bmp.bitsperpixel() )
    self.assertEqual( 24, bmp.bpp() )
    self.assertEqual( 7, bmp.width() )
    self.assertEqual( 9, bmp.height() )
    self.assertEqual( (7, 9), bmp.dimension() )
    self.assertEqual( 0, bmp.colortablesize() )


  # vpixels.bmp() with bad args
  def testBadArgs( self ):
    self.assertRaises( TypeError, vpixels.bmp, 2 )
    self.assertRaises( TypeError, vpixels.bmp, 2.5, 3, 4 )
    self.assertRaises( ValueError, vpixels.bmp, 2, 3, 4 )
    self.assertRaises( ValueError, vpixels.bmp, 9, 3, 4 )
    self.assertRaises( OverflowError, vpixels.bmp, -2, 3, 4 )
    self.assertRaises( ValueError, vpixels.bmp, 2, -3, 4 )
    self.assertRaises( ValueError, vpixels.bmp, 2, 3, -4 )
    self.assertRaises( TypeError, vpixels.bmp, 1, 3, 4, 5  )
    self.assertRaises( TypeError, vpixels.bmp, height = 5, width = 6 )
  

  # 1-bit
  def test1Bit( self ):
    bmp = vpixels.bmp( 1, 5, 6 )
    self.assertEqual( 1, bmp.bitsperpixel() )
    self.assertEqual( 1, bmp.bpp() )
    self.assertEqual( 5, bmp.width() )
    self.assertEqual( 6, bmp.height() )
    self.assertEqual( (5, 6), bmp.dimension() )

    self.assertEqual( 2, bmp.colortablesize() )
    bmp.setcolortable( 1, 2, 3, 4 )
    self.assertEqual( (2, 3, 4), bmp.getcolortable( 1 ) )
    self.assertRaises( ValueError, bmp.setcolortable, 2, 2, 3, 4 )
    self.assertRaises( OverflowError, bmp.setcolortable, -1, 2, 3, 4 )

    self.assertEqual( 0, bmp.getpixel( 0, 0 ) )
    bmp.setallpixels( 1 )
    self.assertEqual( 1, bmp.getpixel( 0, 0 ) )
    bmp.setpixel( 0, 0, 0 )
    self.assertEqual( 0, bmp.getpixel( 0, 0 ) )
    self.assertRaises( ValueError, bmp.getpixel, 5, 0 )
    self.assertRaises( ValueError, bmp.getpixel, 0, 6 )
    self.assertRaises( ValueError, bmp.setallpixels, 2 )
    self.assertRaises( ValueError, bmp.setpixel, 5, 0, 1 )
    self.assertRaises( ValueError, bmp.setpixel, 0, 6, 1 )
    self.assertRaises( ValueError, bmp.setpixel, 0, 0, 2 )

    bmp2 = bmp.clone()
    self.assertEqual( 1, bmp2.bitsperpixel() )
    self.assertEqual( 1, bmp2.bpp() )
    self.assertEqual( 5, bmp2.width() )
    self.assertEqual( 6, bmp2.height() )
    self.assertEqual( (5, 6), bmp2.dimension() )
    self.assertEqual( 2, bmp2.colortablesize() )
    self.assertEqual( (2, 3, 4), bmp2.getcolortable( 1 ) )
    self.assertEqual( 0, bmp2.getpixel( 0, 0 ) )
    self.assertEqual( 1, bmp2.getpixel( 1, 0 ) )
    

  # 4-bit
  def test4Bit( self ):
    bmp = vpixels.bmp( 4, 5, 6 )
    self.assertEqual( 4, bmp.bitsperpixel() )
    self.assertEqual( 4, bmp.bpp() )
    self.assertEqual( 5, bmp.width() )
    self.assertEqual( 6, bmp.height() )
    self.assertEqual( (5, 6), bmp.dimension() )

    self.assertEqual( 16, bmp.colortablesize() )
    bmp.setcolortable( 1, 2, 3, 4 )
    self.assertEqual( (2, 3, 4), bmp.getcolortable( 1 ) )
    self.assertRaises( ValueError, bmp.setcolortable, 16, 2, 3, 4 )
    self.assertRaises( OverflowError, bmp.setcolortable, -1, 2, 3, 4 )

    self.assertEqual( 0, bmp.getpixel( 0, 0 ) )
    bmp.setallpixels( 5 )
    self.assertEqual( 5, bmp.getpixel( 0, 0 ) )
    bmp.setpixel( 0, 0, 0 )
    self.assertEqual( 0, bmp.getpixel( 0, 0 ) )
    self.assertRaises( ValueError, bmp.getpixel, 5, 0 )
    self.assertRaises( ValueError, bmp.getpixel, 0, 6 )
    self.assertRaises( ValueError, bmp.setallpixels, 16 )
    self.assertRaises( ValueError, bmp.setpixel, 5, 0, 1 )
    self.assertRaises( ValueError, bmp.setpixel, 0, 6, 1 )
    self.assertRaises( ValueError, bmp.setpixel, 0, 0, 16 )

    bmp2 = bmp.clone()
    self.assertEqual( 4, bmp2.bitsperpixel() )
    self.assertEqual( 4, bmp2.bpp() )
    self.assertEqual( 5, bmp2.width() )
    self.assertEqual( 6, bmp2.height() )
    self.assertEqual( (5, 6), bmp2.dimension() )
    self.assertEqual( 16, bmp2.colortablesize() )
    self.assertEqual( (2, 3, 4), bmp2.getcolortable( 1 ) )
    self.assertEqual( 0, bmp2.getpixel( 0, 0 ) )
    self.assertEqual( 5, bmp2.getpixel( 1, 0 ) )


  # 8-bit
  def test8Bit( self ):
    bmp = vpixels.bmp( 8, 5, 6 )
    self.assertEqual( 8, bmp.bitsperpixel() )
    self.assertEqual( 8, bmp.bpp() )
    self.assertEqual( 5, bmp.width() )
    self.assertEqual( 6, bmp.height() )
    self.assertEqual( (5, 6), bmp.dimension() )

    self.assertEqual( 256, bmp.colortablesize() )
    bmp.setcolortable( 1, 2, 3, 4 )
    self.assertEqual( (2, 3, 4), bmp.getcolortable( 1 ) )
    self.assertRaises( OverflowError, bmp.setcolortable, 256, 2, 3, 4 )
    self.assertRaises( OverflowError, bmp.setcolortable, -1, 2, 3, 4 )

    self.assertEqual( 0, bmp.getpixel( 0, 0 ) )
    bmp.setallpixels( 205 )
    self.assertEqual( 205, bmp.getpixel( 0, 0 ) )
    bmp.setpixel( 0, 0, 0 )
    self.assertEqual( 0, bmp.getpixel( 0, 0 ) )
    self.assertRaises( ValueError, bmp.getpixel, 5, 0 )
    self.assertRaises( ValueError, bmp.getpixel, 0, 6 )
    self.assertRaises( OverflowError, bmp.setallpixels, 256 )
    self.assertRaises( ValueError, bmp.setpixel, 5, 0, 1 )
    self.assertRaises( ValueError, bmp.setpixel, 0, 6, 1 )
    self.assertRaises( OverflowError, bmp.setpixel, 0, 0, 256 )

    bmp2 = bmp.clone()
    self.assertEqual( 8, bmp2.bitsperpixel() )
    self.assertEqual( 8, bmp2.bpp() )
    self.assertEqual( 5, bmp2.width() )
    self.assertEqual( 6, bmp2.height() )
    self.assertEqual( (5, 6), bmp2.dimension() )
    self.assertEqual( 256, bmp2.colortablesize() )
    self.assertEqual( (2, 3, 4), bmp2.getcolortable( 1 ) )
    self.assertEqual( 0, bmp2.getpixel( 0, 0 ) )
    self.assertEqual( 205, bmp2.getpixel( 1, 0 ) )


  # 24-bit
  def test24Bit( self ):
    bmp = vpixels.bmp( 24, 5, 6 )
    self.assertEqual( 24, bmp.bitsperpixel() )
    self.assertEqual( 24, bmp.bpp() )
    self.assertEqual( 5, bmp.width() )
    self.assertEqual( 6, bmp.height() )
    self.assertEqual( (5, 6), bmp.dimension() )

    self.assertEqual( 0, bmp.colortablesize() )
    self.assertRaises( Exception, bmp.setcolortable, 1, 2, 3, 4 )
    self.assertRaises( Exception, bmp.getcolortable, 1 )

    self.assertEqual( (0, 0, 0), bmp.getpixel( 0, 0 ) )
    bmp.setallpixels( 205, 206, 207 )
    self.assertEqual( (205, 206, 207), bmp.getpixel( 0, 0 ) )
    bmp.setpixel( 0, 0, 25, 26, 27 )
    self.assertEqual( (25, 26, 27), bmp.getpixel( 0, 0 ) )
    self.assertRaises( ValueError, bmp.getpixel, 5, 0 )
    self.assertRaises( ValueError, bmp.getpixel, 0, 6 )
    self.assertRaises( TypeError, bmp.setallpixels, 25 )
    self.assertRaises( OverflowError, bmp.setallpixels, 256, 27, 28 )
    self.assertRaises( ValueError, bmp.setpixel, 5, 0, 25, 26, 27 )
    self.assertRaises( ValueError, bmp.setpixel, 0, 6, 25, 26, 27 )
    self.assertRaises( OverflowError, bmp.setpixel, 0, 0, 256, 27, 28 )
    self.assertRaises( OverflowError, bmp.setpixel, 0, 0, 26, 257, 28 )
    self.assertRaises( OverflowError, bmp.setpixel, 0, 0, 26, 27, 258 )

    bmp2 = bmp.clone()
    self.assertEqual( 24, bmp2.bitsperpixel() )
    self.assertEqual( 24, bmp2.bpp() )
    self.assertEqual( 5, bmp2.width() )
    self.assertEqual( 6, bmp2.height() )
    self.assertEqual( (5, 6), bmp2.dimension() )
    self.assertEqual( 0, bmp2.colortablesize() )
    self.assertEqual( (25, 26, 27), bmp2.getpixel( 0, 0 ) )
    self.assertEqual( (205, 206, 207), bmp2.getpixel( 1, 0 ) )


  def testImportf( self ):
    bmp = vpixels.bmp( 4, 5, 6 )

    # import non-existing file
    self.assertRaises( IOError, bmp.importf, 'not-exits.bmp' ) # not exit
    # bmp unchanged
    self.assertEqual( 4, bmp.bpp() )
    self.assertEqual( (5, 6), bmp.dimension() )
    self.assertEqual( 16, bmp.colortablesize() )

    # importf() expects a string argument
    self.assertRaises( TypeError, bmp.importf, 2 ) # not a file name
    # bmp unchanged
    self.assertEqual( 4, bmp.bpp() )
    self.assertEqual( (5, 6), bmp.dimension() )
    self.assertEqual( 16, bmp.colortablesize() )

    # import non-bmp file or corrupted bmp file
    self.assertRaises( Exception, bmp.importf, 'BmpTest.py' ) # not a bmp file
    # bmp becomes invalid, results of calling its methods are undefined


  def testExport( self ):
    bmp = vpixels.bmp()
    bmp.export( 'temp.bmp', True ) # overwrite = True
    self.assertRaises( IOError, bmp.export, 'temp.bmp' ) # overwrite = False
    self.assertRaises( IOError, bmp.export, 'temp.bmp', False ) # overwrite = False


  def testInheritance( self ):
    class subbmp( vpixels.bmp ):
      def __init__( self ):
        super(subbmp, self).__init__( 4, 3, 4 )

      def setcolor( self ):
        super(subbmp, self).setcolor( 0, 2, 3, 4 )

    bmp = subbmp()
    bmp.setcolor()
    self.assertEqual( (2, 3, 4), bmp.getcolor(0) )
    bmp.setpixel( 1, 2, 3)
    self.assertEqual( 3, bmp.getpixel( 1, 2 ) )


if __name__ == '__main__':
  unittest.main()
