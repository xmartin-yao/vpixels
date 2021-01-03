------------------------------------------------------------------------
-- Copyright (C) 2019-2021 Xueyi Yao
--
-- This file is part of VPixels.
--
-- VPixels is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- VPixels is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with VPixels. If not, see <https://www.gnu.org/licenses/>.
------------------------------------------------------------------------

-- Generate a GIF file of animated hourglass

-- load vpixels module
local vp = require( "vpixels" )

-- check version of the module, need 0.7.2 or later
assert( vp.version('0.7.2') )

-- create a vpixels.gif object
-- 2 bits/pixel, 99x99, 93 images
local hourglass = vp.gif( 2, 99, 99, 93 )

-- generate GIF file
function hourglass:generate()
  print( "Set color table" )
  self:setcolor()

  print( "Drawing ..." )
  self:draw()

  print( "Set delay time" )
  self:setpause()

  print( "Export to 'hourglass.gif'" )
  self:export( "hourglass.gif", true )
end

-- set color table
-- this function overrides the built-in method
-- setcolor(index, red, green, blue) of vpixels.gif
function hourglass:setcolor()
  -- set color table entries
  -- call the built-in setcolor(index, red, green, blue)
  self.base:setcolor( 0, 255, 255, 255 )  -- white
  self.base:setcolor( 1, 255, 0, 0 )      -- red
  self.base:setcolor( 2, 0, 255, 0 )      -- green
  self.base:setcolor( 3, 0, 0, 255 )      -- blue

  -- alias for each entry
  self.white = 0
  self.red   = 1
  self.green = 2
  self.blue  = 3
end

-- draw images
function hourglass:draw()
  -- layout
  self:layout()

  -- image #0 ~ #46
  self:paint0to46()

  -- image #47 ~ #92
  self:reflect()
end

-- draw two blue triangles on each image
function hourglass:layout()
  -- draw image #0
  local img0 = self:layoutimage0()

  -- copy image #0 to others
  for i = 1, #self - 1 do
    self[i] = img0
  end
end

-- draw two blue triangles on image #0
function hourglass:layoutimage0()
  -- image #0
  local img0 = self[0]

  -- draw two blue horizontal lines
  -- (2,2)->(96,2) and (2,96)->(96,96)
  for x = 2, 96 do
    img0:setpixel( x, 2,  self.blue )
    img0:setpixel( x, 96, self.blue )
  end

  -- draw two blue diagonal lines
  -- (3,3)->(95,95) and (95,3)->(3,95)
  -- intersect at (49,49), the very center of the image
  for x = 3, 95 do
    local y = x
    img0:setpixel( x, y, self.blue )
    img0:setpixel( 98 - x, y, self.blue )
  end

  return img0
end

-- paint image #0 ~ #46
function hourglass:paint0to46()
  self:painttoptriangle()
  self:paintbottomtriangle()
end

--[[
fill top triangle using color green
  image #0: (4,3)->(94,3), ... (49,48), 46 lines, triangle fully filled
  image #1: (5,4)->(93,4), ... (49,48), 45 lines
  ...
  image #44: (48,47)->(50,47), ... (49,48), 2 lines
  image #45: (49,48), 1 line (just a pixel)
  image #46: leave triangle empty
]]
function hourglass:painttoptriangle()
  local xstart, xend, y  -- a horizontal line (xstart,y)->(xend,y)
  local nlines = 46      -- number of lines to draw
  local img

  -- paint image #0 ~ #45
  for i = 0, 45 do
    img = self[i]  -- i-th image

    -- 1st line is just a pixel at (49,48)
    -- one line up the center (49,49) of the image
    xstart, xend, y = 49, 49, 48

    for line = 1, nlines do  -- draw lines
      for x = xstart, xend do  -- draw a line
        img:setpixel( x, y, self.green )  -- draw a pixel
      end

      -- next line is two pixels wider and located one line up
      xstart = xstart - 1
      xend = xend + 1
      y = y - 1
    end

    -- next image is one line less than current one
    nlines = nlines - 1
  end
end

--[[
fill bottom triangle using color green
  image #0: leave triangle empty
  image #1: (4,95)->(94,95), 1 line
  image #2: (4,95)->(94,95), (5,94)->(93, 94), 2 lines
  ...
  image #45: (4,95)->(94,95), ... (48,51)->(50,51), 45 lines
  image #46: (4,95)->(94,95), ... (49,50), 46 lines, triangle fully filled
]]
function hourglass:paintbottomtriangle()
  local xstart, xend, y  -- a horizontal line (xstart,y)->(xend,y)
  local nlines = 1       -- number of lines to draw
  local img

  -- paint image #1 ~ #46
  for i = 1, 46 do
    img = self[i]  -- i-th image

    -- 1st line (4,95)->(94,95)
    -- one line up the bottom blue line
    xstart, xend, y = 4, 94, 95

    for line = 1, nlines do  -- draw lines
      for x = xstart, xend do  -- draw a line
        img:setpixel( x, y, self.green )  -- draw a pixel
      end

      -- next line is two pixels shorter and located one line up
      xstart = xstart + 1
      xend = xend - 1
      y = y - 1
    end

    -- next image has one more line than current one
    nlines = nlines + 1
  end
end

-- copy image #45 ~ #0 to image #47 ~ #92
-- symmetric about image #46
function hourglass:reflect()
  local j = 45
  for i = 47, 92 do
    self[i] = self[j]
    j = j - 1
  end
end

-- delay time of images
function hourglass:setpause()
  self[46]:delay( 200 )  -- image #46
  self[92]:delay( 200 )  -- image #92
end

-- run generate()
print(hourglass)
hourglass:generate()
io.write( "Elapsed: ", os.clock(), " sec.\n" )
