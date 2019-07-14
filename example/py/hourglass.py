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

# Generate a GIF file of animated hourglass
class hourglass( vpixels.gif ):
  def __init__( self ):
    # 2 bits/pixel, 99x99, 93 images
    super( hourglass, self ).__init__( 2, 99, 99, 93 )


  # generate GIF file
  def generate( self ):
    print "Set color table"
    self.__setcolor()

    print "Drawing ..."
    self.__draw()

    print "Set delay time"
    self.__setpause()

    print "Export to 'hourglass.gif'"
    self.export( "hourglass.gif", True )


  # set color table entries
  def __setcolor( self ):
    self.setcolor( 0, 255, 255, 255 )  # white
    self.setcolor( 1, 255, 0, 0 )      # red
    self.setcolor( 2, 0, 255, 0 )      # green
    self.setcolor( 3, 0, 0, 255 )      # blue


  # draw
  def __draw( self ):
    # layout
    self.__layout()

    # image #0 ~ #46
    self.__paint0_46()

    # image #47 ~ #49
    self.__reflect()

    # set delay time of image #46 and #92
    self[46].delay( 200 )
    self[92].delay( 200 )


  # draw two triangles for each image
  def __layout( self ):
    # image #0
    img0 = self.getimage( 0 )

    # draw image #0: two horizontal lines
    for x in range( 2, 97 ):
      img0.setpixel( x, 2, 3 )  # entry 3 is blue color
      img0.setpixel( x, 96, 3 )

    # draw image #0: two diagonal lines
    for x in range( 3, 96 ):
      y = x
      img0.setpixel( x, y, 3 )
      img0.setpixel( 98 - x, y, 3 )

    # clone image #0
    for i in range( 1, 93 ):
      self[i].clone( img0 )


  # image #0 ~ #46
  def __paint0_46( self ):
    # top triangle
    offset = 3
    for i in range( 0, 47 ):
      img = self[i]
      for y in range( offset, 49 ):
        for x in range( y + 1, 98 - y ):
          img.setpixel( x, y, 2 )
      offset += 1

    # bottom triangle
    offset = 95
    for i in range( 1, 47 ):
      img = self[i]
      for y in range( offset, 96 ):
        for x in range( 98 - y + 1, y):
          img.setpixel( x, y, 2 )
      offset -= 1


  # image #47 ~ #49
  # clone image #45 ~ image #0, symmetric about image #46
  def __reflect( self ):
    j = 45
    for i in range( 47, 93 ):
      self[i].clone( self[j] )
      j -= 1


  # set delay time of images
  def __setpause( self ):
    self[46].delay( 200 )  # image #46 
    self[92].delay( 200 )  # image #92 



if __name__ == '__main__':
  start = time.clock()

  gif = hourglass()
  print gif
  gif.generate()

  end = time.clock()
  print "Elapsed:", end - start, "sec."
