#######################################################################
# Copyright (C) 2019-2021 Xueyi Yao
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
# along with VPixels. If not, see <https://www.gnu.org/licenses/>.
#######################################################################
import vpixels as vp
import time

# check version of the module
assert vp.version( '0.7.2' ), "require version 0.7.2 or later"


# Generate a GIF file of animated hourglass
class hourglass( vp.gif ):
  def __init__( self ):
    # 2 bits/pixel, 99x99, 93 images
    super( hourglass, self ).__init__( 2, 99, 99, 93 )


  # generate GIF file
  def generate( self ):
    print( "Set color table" )
    self.__setcolor()

    print( "Drawing ..." )
    self.__draw()

    print( "Set delay time" )
    self.__setpause()

    print( "Export to 'hourglass.gif'" )
    self.export( "hourglass.gif", True )


  # set color table entries
  def __setcolor( self ):
    # set color table entries
    self.setcolor( 0, 255, 255, 255 )  # white
    self.setcolor( 1, 255, 0, 0 )      # red
    self.setcolor( 2, 0, 255, 0 )      # green
    self.setcolor( 3, 0, 0, 255 )      # blue

    # alias for each entry
    self.white = 0
    self.red   = 1
    self.green = 2
    self.blue  = 3


  # draw
  def __draw( self ):
    # layout
    self.__layout()

    # image #0 ~ #46
    self.__paint0to46()

    # image #47 ~ #49
    self.__reflect()


  # draw two blue triangles on each image
  def __layout( self ):
    # draw image #0
    img0 = self.__layoutimage0()

    # copy image #0
    for i in range( 1, 93 ):
      self[i] = img0


  # paint image #0 ~ #46
  def __paint0to46( self ):
    self.__painttoptriangle()
    self.__paintbottomtriangle()


  # copy image #45 ~ #0 to image #47 ~ #92
  # symmetric about image #46
  def __reflect( self ):
    j = 45
    for i in range( 47, 93 ):
      self[i] = self[j]
      j -= 1


  # draw two blue triangles on image #0
  def __layoutimage0( self ):
    # image #0
    img0 = self[0]

    # draw two blue horizontal lines
    # (2,2)->(96,2) and (2,96)->(96,96)
    for x in range( 2, 97 ):
      img0.setpixel( x, 2,  self.blue )
      img0.setpixel( x, 96, self.blue )

    # draw two blue diagonal lines
    # (3,3)->(95,95) and (95,3)->(3,95)
    # intersect at (49,49), the very center of the image
    for x in range( 3, 96 ):
      y = x
      img0.setpixel( x, y, self.blue )
      img0.setpixel( 98 - x, y, self.blue )

    return img0


  # fill top triangle using color green
  #   image #0: (4,3)->(94,3), ... (49,48), 46 lines, triangle fully filled
  #   image #1: (5,4)->(93,4), ... (49,48), 45 lines
  #   ...
  #   image #44: (48,47)->(50,47), ... (49,48), 2 lines
  #   image #45: (49,48), 1 line (just a pixel)
  #   image #46: leave triangle empty
  def __painttoptriangle( self ):
    nlines = 46  # number of lines to draw

    # paint image #0 ~ #45
    for i in range( 46 ):
      img = self[i]  # i-th iamge

      # 1st line is just a pixel at (49,48)
      # one line up the center of the image (49,49)
      xstart, xend, y = 49, 49, 48

      for line in range( nlines ):  # draw lines
        for x in range( xstart, xend + 1):  # draw a line
          img.setpixel( x, y, self.green )  # draw a pixel

        # next line is two pixels wider and located one line up
        xstart -= 1
        xend += 1
        y -= 1

      # next image is one line less than current one
      nlines -= 1


  # fill bottom triangle using color green
  #   image #0: leave triangle empty
  #   image #1: (4,95)->(94,95), 1 line
  #   image #2: (4,95)->(94,95), (5,94)->(93, 94), 2 lines
  #   ...
  #   image #45: (4,95)->(94,95), ... (48,51)->(50,51), 45 lines
  #   image #46: (4,95)->(94,95), ... (49,50), 46 lines, triangle fully filled
  def __paintbottomtriangle( self ):
    nlines = 1  # number of lines to draw

    # paint image #1 ~ #46
    for i in range( 1, 47 ):
      img = self[i]  # i-th iamge

      # 1st horizontal line (4,95)->(94,95)
      # one line up the bottom blue line
      xstart, xend, y = 4, 94, 95

      for line in range( nlines ):  # draw lines
        for x in range( xstart, xend + 1 ):  # draw a line
          img.setpixel( x, y, self.green )   # draw a pixel

        # next line is two pixels shorter and located one line up
        xstart += 1
        xend -= 1
        y -= 1

      # next image has one more line than current one
      nlines += 1


  # set delay time of images
  def __setpause( self ):
    self[46].delay( 200 )  # image #46 
    self[92].delay( 200 )  # image #92 



if __name__ == '__main__':
  start = time.time()

  gif = hourglass()
  print( gif )
  gif.generate()

  end = time.time()
  print( "Elapsed: %f sec." % (end - start) )
