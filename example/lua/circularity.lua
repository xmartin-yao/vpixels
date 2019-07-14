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

-- Generate a BMP file consisting of circular shapes

local vpixels = require( "vpixels" )

-- define circularity object
local circularity = {}

-- generate BMP file
function circularity:generate()
  print( "Set color table" )
  self:setcolor()

  print( "Drawing ..." )
  self:draw()

  print( "Export to 'circularity.bmp'" )
  self:export( "circularity.bmp", true )
end

-- set color table entries
--   0 ~ height/2 - 1  : dark -> bright
--   height/2 ~ height : bright -> dark
function circularity:setcolor()
  local height = self:height()
  local j = height - 1
  local color = nil
  for i = 0, height/2 - 1 do
    color = 4*i
    self.super:setcolor( i, color, color, color )
    self.super:setcolor( j, color, color, color )
    j = j - 1
  end
end

-- draw image
function circularity:draw()
  -- dimension
  local width, height = self:dimension()

  -- center
  local x0 = width/2 - 1
  local y0 = height/2 -1

  -- set every pixel
  local radius = nil
  for x = 0, width - 1 do
    for y = 0, height - 1 do
      radius = math.sqrt( (x - x0)^2 + (y - y0)^2 )
      self:setpixel( x, y, math.ceil(radius) )
    end
  end
end

-- create a vp.bmp object: 8 bits/pixel, 162x100
local bmp = vpixels.bmp( 8, 162, 100 )

-- derive circularity object from vp.bmp object
vpixels.derive( bmp, circularity )
print(circularity)

-- run generate()
circularity:generate()
io.write( "Elapsed: ", os.clock(), " sec.\n" )
