------------------------------------------------------------------------
-- Copyright (C) 2019 Xueyi Yao
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
-- along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
------------------------------------------------------------------------

-- Generate a GIF file of animated hourglass

local vpixels = require( "vpixels" )

-- create a vp.gif object
-- 2 bits/pixel, 99x99, 93 images
local gif = vpixels.gif( 2, 99, 99, 93 )

-- derive hourglass object from previously created vp.gif object
local hourglass = vpixels.derive( gif )

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

-- set color table entries
function hourglass:setcolor()
  self.super:setcolor( 0, 255, 255, 255 )  -- white
  self.super:setcolor( 1, 255, 0, 0 )      -- red
  self.super:setcolor( 2, 0, 255, 0 )      -- green
  self.super:setcolor( 3, 0, 0, 255 )      -- blue
end

-- draw images
function hourglass:draw()
  -- layout
  self:layout()

  -- image #0 ~ #46
  self:paint0_46()

  -- image #47 ~ #49
  self:reflect()
end

-- draw two triangles for each image
function hourglass:layout()
  -- image #0
  local img0 = self[0]

  -- draw image #0: two horizontal lines
  for x = 2, 96 do
    img0:setpixel( x, 2, 3 )
    img0:setpixel( x, 96, 3 )
  end

  -- draw image #0: two diagonal lines
  for x = 3, 95 do
    img0:setpixel( x, x, 3 )
    img0:setpixel( 98 - x, x, 3 )
  end

  -- clone image #0
  for i = 1, #self - 1 do
    self[i]:clone( img0 )
  end
end

-- image #0 ~ #46
function hourglass:paint0_46()
  -- top triangle
  local offset = 3
  local img = nil
  for i = 0, 46 do
    img = self[i]
    for y = offset, 48 do
      for x = y + 1, 98 - y - 1 do
        img:setpixel( x, y, 2 )
      end
    end
    offset = offset + 1
  end

  -- bottom triangle
  offset = 95
  for i = 1, 46 do
    img = self[i]
    for y = offset, 95 do
      for x = 98 - y + 1, y - 1 do
        img:setpixel( x, y, 2 )
      end
    end
    offset = offset - 1
  end
end

-- image #47 ~ #49
-- clone image #45 ~ image #0, symmetric about image #46
function hourglass:reflect()
  local j = 45
  for i = 47, 92 do
    self[i]:clone( self[j] )
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
