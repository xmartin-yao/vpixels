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

-- vp.bmp unit tests

local lu = require( "luaunit" )

-- load vpixels from current directory
if package.config:sub( 1, 1 ) == "\\" then
  package.cpath = '.\\?.dll'
else
  package.cpath = './?.so'
end
local vpixels = require( "vpixels" )


TestBmp = {}

-- vpixels.bmp() with no arg
function TestBmp:testNoArg()
  local bmp = vpixels.bmp()

  lu.assertEquals( 1, bmp:bitsperpixel() )
  lu.assertEquals( 1, bmp:bpp() )

  lu.assertEquals( 1, bmp:width() )
  lu.assertEquals( 1, bmp:height() )
  local w, h = bmp:dimension()
  lu.assertEquals( 1, w )
  lu.assertEquals( 1, h )

  lu.assertEquals( 2, bmp:colortablesize() )

  -- default value 0
  local b, g, r = bmp:getcolortable( 0 )
  lu.assertEquals( 0, b )
  lu.assertEquals( 0, g )
  lu.assertEquals( 0, r )
  b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 0, b )
  lu.assertEquals( 0, g )
  lu.assertEquals( 0, r )

  bmp:setcolortable( 0, 25, 26, 27 )
  b, g, r = bmp:getcolortable( 0 )
  lu.assertEquals( 25, b )
  lu.assertEquals( 26, g )
  lu.assertEquals( 27, r )

  -- index exceeds limit
  lu.assertError( bmp.setcolortable, bmp, 2, 25, 26, 27 )
  lu.assertError( bmp.getcolortable, bmp, 2 )
  lu.assertError( bmp.setcolortable, bmp, -1, 25, 26, 27 )
  lu.assertError( bmp.getcolortable, bmp, -1 )
end

-- vpixels.bmp() with bad args
function TestBmp:testBadArgs()
  lu.assertError( vpixels.bmp, 2 )
  lu.assertError( vpixels.bmp, 2.5, 3, 4 )
  lu.assertError( vpixels.bmp, 2, 3, 4 )
  lu.assertError( vpixels.bmp, 9, 3, 4 )
  lu.assertError( vpixels.bmp, -2, 3, 4 )
  lu.assertError( vpixels.bmp, 2, -3, 4 )
  lu.assertError( vpixels.bmp, 2, 3, -4 )
  lu.assertError( vpixels.bmp, 1, 3, 4, 5  )
end

-- 1-bit
function TestBmp:test1Bit()
  local bmp = vpixels.bmp( 1, 5, 6 )

  lu.assertEquals( 1, bmp:bitsperpixel() )
  lu.assertEquals( 1, bmp:bpp() )

  lu.assertEquals( 5, bmp:width() )
  lu.assertEquals( 6, bmp:height() )
  local w, h = bmp:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )

  lu.assertEquals( 2, bmp:colortablesize() )
  bmp:setcolortable( 1, 2, 3, 4 )
  local b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 2, b )
  lu.assertEquals( 3, g )
  lu.assertEquals( 4, r )

  lu.assertError( bmp.setcolortable, bmp, 2, 2, 3, 4 )
  lu.assertError(  bmp.setcolortable, bmp, -1, 2, 3, 4 )

  lu.assertEquals( 0, bmp:getpixel( 0, 0 ) )
  bmp:setallpixels( 1 )
  lu.assertEquals( 1, bmp:getpixel( 0, 0 ) )
  bmp:setpixel( 0, 0, 0 )
  lu.assertEquals( 0, bmp:getpixel( 0, 0 ) )

  lu.assertError( bmp.getpixel, bmp, 5, 0 )
  lu.assertError( bmp.getpixel, bmp, 0, 6 )
  lu.assertError( bmp.setallpixels, bmp, 2 )
  lu.assertError( bmp.setpixel, bmp, 5, 0, 1 )
  lu.assertError( bmp.setpixel, bmp, 0, 6, 1 )
  lu.assertError( bmp.setpixel, bmp, 0, 0, 2 )

  local bmp2 = bmp:clone()

  lu.assertEquals( 1, bmp2:bitsperpixel() )
  lu.assertEquals( 1, bmp2:bpp() )
  lu.assertEquals( 5, bmp2:width() )
  lu.assertEquals( 6, bmp2:height() )
  local w, h = bmp2:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )
  lu.assertEquals( 2, bmp2:colortablesize() )
  b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 2, b )
  lu.assertEquals( 3, g )
  lu.assertEquals( 4, r )
  lu.assertEquals( 0, bmp2:getpixel( 0, 0 ) )
  lu.assertEquals( 1, bmp2:getpixel( 1, 0 ) )
end

-- 4-bit
function TestBmp:test4Bit()
  local bmp = vpixels.bmp( 4, 5, 6 )

  lu.assertEquals( 4, bmp:bitsperpixel() )
  lu.assertEquals( 4, bmp:bpp() )

  lu.assertEquals( 5, bmp:width() )
  lu.assertEquals( 6, bmp:height() )
  local w, h = bmp:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )

  lu.assertEquals( 16, bmp:colortablesize() )
  bmp:setcolortable( 1, 2, 3, 4 )
  local b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 2, b )
  lu.assertEquals( 3, g )
  lu.assertEquals( 4, r )

  lu.assertError( bmp.setcolortable, bmp, 16, 2, 3, 4 )
  lu.assertError( bmp.setcolortable, bmp, -1, 2, 3, 4 )

  lu.assertEquals( 0, bmp:getpixel( 0, 0 ) )
  bmp:setallpixels( 5 )
  lu.assertEquals( 5, bmp:getpixel( 0, 0 ) )
  bmp:setpixel( 0, 0, 0 )
  lu.assertEquals( 0, bmp:getpixel( 0, 0 ) )

  lu.assertError( bmp.getpixel, bmp, 5, 0 )
  lu.assertError( bmp.getpixel, bmp, 0, 6 )
  lu.assertError( bmp.setallpixels, bmp, 16 )
  lu.assertError( bmp.setpixel, bmp, 5, 0, 1 )
  lu.assertError( bmp.setpixel, bmp, 0, 6, 1 )
  lu.assertError( bmp.setpixel, bmp, 0, 0, 16 )

  local bmp2 = bmp:clone()

  lu.assertEquals( 4, bmp2:bitsperpixel() )
  lu.assertEquals( 4, bmp2:bpp() )
  lu.assertEquals( 5, bmp2:width() )
  lu.assertEquals( 6, bmp2:height() )
  w, h = bmp2:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )
  b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 2, b )
  lu.assertEquals( 3, g )
  lu.assertEquals( 4, r )
  lu.assertEquals( 0, bmp2:getpixel( 0, 0 ) )
  lu.assertEquals( 5, bmp2:getpixel( 1, 0 ) )
end

-- 8-bit
function TestBmp:test8Bit()
  local bmp = vpixels.bmp( 8, 5, 6 )

  lu.assertEquals( 8, bmp:bitsperpixel() )
  lu.assertEquals( 8, bmp:bpp() )
  lu.assertEquals( 5, bmp:width() )
  lu.assertEquals( 6, bmp:height() )
  local w, h = bmp:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )

  lu.assertEquals( 256, bmp:colortablesize() )
  bmp:setcolortable( 1, 2, 3, 4 )
  local b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 2, b )
  lu.assertEquals( 3, g )
  lu.assertEquals( 4, r )

  lu.assertError( bmp.setcolortable, bmp, 256, 2, 3, 4 )
  lu.assertError( bmp.setcolortable, bmp, -1, 2, 3, 4 )

  lu.assertEquals( 0, bmp:getpixel( 0, 0 ) )
  bmp:setallpixels( 205 )
  lu.assertEquals( 205, bmp:getpixel( 0, 0 ) )
  bmp:setpixel( 0, 0, 0 )
  lu.assertEquals( 0, bmp:getpixel( 0, 0 ) )
  lu.assertError( bmp.getpixel, bmp, 5, 0 )
  lu.assertError( bmp.getpixel, bmp, 0, 6 )
  lu.assertError( bmp.setallpixels, bmp, 256 )
  lu.assertError( bmp.setpixel, bmp, 5, 0, 1 )
  lu.assertError( bmp.setpixel, bmp, 0, 6, 1 )
  lu.assertError( bmp.setpixel, bmp, 0, 0, 256 )

  local bmp2 = bmp:clone()

  lu.assertEquals( 8, bmp2:bitsperpixel() )
  lu.assertEquals( 8, bmp2:bpp() )
  lu.assertEquals( 5, bmp2:width() )
  lu.assertEquals( 6, bmp2:height() )
  w, h = bmp2:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )
  lu.assertEquals( 256, bmp2:colortablesize() )
  b, g, r = bmp:getcolortable( 1 )
  lu.assertEquals( 2, b )
  lu.assertEquals( 3, g )
  lu.assertEquals( 4, r )
  lu.assertEquals( 0, bmp2:getpixel( 0, 0 ) )
  lu.assertEquals( 205, bmp2:getpixel( 1, 0 ) )
end

-- 24-bit
function TestBmp:test24Bit()
  local bmp = vpixels.bmp( 24, 5, 6 )

  lu.assertEquals( 24, bmp:bitsperpixel() )
  lu.assertEquals( 24, bmp:bpp() )
  lu.assertEquals( 5, bmp:width() )
  lu.assertEquals( 6, bmp:height() )
  local w, h = bmp:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )

  lu.assertEquals( 0, bmp:colortablesize() )

  lu.assertError( bmp.setcolortable, bmp, 1, 2, 3, 4 )
  lu.assertError( bmp.getcolortable, bmp, 1 )

  local b, g, r = bmp:getpixel( 0, 0 )
  lu.assertEquals( 0, b )
  lu.assertEquals( 0, g )
  lu.assertEquals( 0, r )
  bmp:setallpixels( 205, 206, 207 )
  b, g, r = bmp:getpixel( 0, 0 )
  lu.assertEquals( 205, b )
  lu.assertEquals( 206, g )
  lu.assertEquals( 207, r )
  bmp:setpixel( 0, 0, 25, 26, 27 )
  b, g, r = bmp:getpixel( 0, 0 )
  lu.assertEquals( 25, b )
  lu.assertEquals( 26, g )
  lu.assertEquals( 27, r )

  lu.assertError( bmp.getpixel, bmp, 5, 0 )
  lu.assertError( bmp.getpixel, bmp, 0, 6 )
  lu.assertError( bmp.setallpixels, bmp, 25 )
  lu.assertError( bmp.setallpixels, bmp, 256, 27, 28 )
  lu.assertError( bmp.setpixel, bmp, 5, 0, 25, 26, 27 )
  lu.assertError( bmp.setpixel, bmp, 0, 6, 25, 26, 27 )
  lu.assertError( bmp.setpixel, bmp, 0, 0, 256, 27, 28 )
  lu.assertError( bmp.setpixel, bmp, 0, 0, 26, 257, 28 )
  lu.assertError( bmp.setpixel, bmp, 0, 0, 26, 27, 258 )

  local bmp2 = bmp:clone()

  lu.assertEquals( 24, bmp2:bitsperpixel() )
  lu.assertEquals( 24, bmp2:bpp() )
  lu.assertEquals( 5, bmp2:width() )
  lu.assertEquals( 6, bmp2:height() )
  w, h = bmp2:dimension()
  lu.assertEquals( 5, w )
  lu.assertEquals( 6, h )
  lu.assertEquals( 0, bmp2:colortablesize() )
  b, g, r = bmp:getpixel( 0, 0 )
  lu.assertEquals( 25, b )
  lu.assertEquals( 26, g )
  lu.assertEquals( 27, r )
  b, g, r = bmp:getpixel( 1, 0 )
  lu.assertEquals( 205, b )
  lu.assertEquals( 206, g )
  lu.assertEquals( 207, r )
end

function TestBmp:testImport()
  local bmp = vpixels.bmp()
  lu.assertError( bmp.import, bmp, 'not-exits.bmp' ) -- not exit
  lu.assertError( bmp.import, bmp, 'BmpTest.lua' )   -- not a BMP file
  lu.assertError( bmp.import, bmp, 2 )
  lu.assertError( bmp.import, bmp, {} )
end

function TestBmp:testExport()
  local bmp = vpixels.bmp()
  bmp:export( 'temp.bmp', true )
  lu.assertError( bmp.export, bmp, 'temp.bmp' )
  lu.assertError( bmp.export, bmp, 'temp.bmp', false )
end

function TestBmp:testGC()
  local bmp = vpixels.bmp( 1, 3, 4 )
  lu.assertEquals( bmp:bpp(), 1 )

  bmp:__gc()
  lu.assertError( bmp.bpp, bmp ) -- test LuaBmp::CheckBmp()

  -- calling __gc() again is harmless
  bmp:__gc()
end

function TestBmp:testMetatable()
  local bmp = vpixels.bmp()

  -- allow modifying metatable
  local mt = getmetatable(bmp)
  lu.assertEquals( "table", type(mt) )
  mt.getzero = function() return 0 end
  lu.assertEquals( 0, bmp:getzero() )

  -- metatable has its string representation
  lu.assertEquals( "string", type(tostring(mt)) )

  -- changing metatable is not allowed
  lu.assertError( setmetatable, bmp, {} )
end


local runner = lu.LuaUnit.new()
--runner:setOutputType("tap")
os.exit( runner:runSuite() )
