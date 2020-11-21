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
import vpixels
import time
import math

# Generate a BMP file consisting of circular shapes
class circularity( vpixels.bmp ):
  def __init__( self ):
    # 8 bits/pixel, 162x100
    super( circularity, self ).__init__( 8, 162, 100 )


  # generate BMP file
  def generate( self ):
    print( "Set color table" )
    self.__setcolor()

    print( "Drawing ..." )
    self.__draw()

    print( "Export to 'circularity.bmp'" )
    self.export( "circularity.bmp", True )


  # set color table entries
  #   0 ~ height/2 - 1  : dark -> bright
  #   height/2 ~ height : bright -> dark
  def __setcolor( self ):
    height = self.height()

    j = height - 1
    for i in range( 0, int(height/2) ):
      color = 4*i
      self.setcolor( i, color, color, color )
      self.setcolor( j, color, color, color )
      j -= 1


  # draw
  def __draw( self ):
    # dimension
    width, height = self.dimension()

    # center
    x0 = width/2 - 1
    y0 = height/2 -1

    # set every pixel
    for x in range( 0, width ):
      for y in range( 0, height ):
        radius = math.sqrt( (x - x0)*(x - x0) + (y - y0)*(y - y0) )
        self.setpixel( x, y, int(math.ceil(radius)) )


if __name__ == '__main__':
  start = time.time()

  bmp = circularity()
  print( bmp )
  bmp.generate()

  end = time.time()
  print( "Elapsed: %f sec." % (end - start) )
