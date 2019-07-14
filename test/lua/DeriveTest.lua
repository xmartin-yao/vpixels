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

-- vp.derived unit tests

local lu = require( "luaunit" )

-- load vpixels from current directory
if package.config:sub( 1, 1 ) == "\\" then
  package.cpath = '.\\?.dll'
else
  package.cpath = './?.so'
end
local vpixels = require( "vpixels" )

TestDerive = {}

function TestDerive:testDerive()
  -- no arg: error
  lu.assertError( vpixels.derive )

  -- 1 arg
  local derived_bmp = vpixels.derive( vpixels.bmp(1, 2, 3) )
  lu.assertEquals( "table", type(derived_bmp) )
  lu.assertEquals( "userdata", type(derived_bmp.super) )

  local derived_gif = vpixels.derive( vpixels.gif(2, 3, 4, 5) )
  lu.assertEquals( "table", type(derived_gif) )
  lu.assertEquals( "userdata", type(derived_gif.super) )

  local derived_img = vpixels.derive( vpixels.gif()[0] )
  lu.assertEquals( "table", type(derived_img) )
  lu.assertEquals( "userdata", type(derived_img.super) )

  local derived_table = vpixels.derive( {} )
  lu.assertEquals( "table", type(derived_table) )
  lu.assertEquals( "table", type(derived_table.super) )

  -- 2 args
  derived_bmp = {}
  vpixels.derive( vpixels.bmp(4, 5, 6), derived_bmp )
  lu.assertEquals( "table", type(derived_bmp) )
  lu.assertEquals( "userdata", type(derived_bmp.super) )

  derived_gif = {}
  vpixels.derive( vpixels.gif(3, 4, 5, 6), derived_gif )
  lu.assertEquals( "table", type(derived_gif) )
  lu.assertEquals( "userdata", type(derived_gif.super) )

  derived_img = {}
  vpixels.derive( vpixels.gif(3, 4, 5, 6)[5], derived_img )
  lu.assertEquals( "table", type(derived_img) )
  lu.assertEquals( "userdata", type(derived_img.super) )

  derived_table = {}
  vpixels.derive( {}, derived_table )
  lu.assertEquals( "table", type(derived_table) )
  lu.assertEquals( "table", type(derived_table.super) )

  -- 2 args: error
  lu.assertError( vpixels.derive, vpixels.bmp(), 3 )
  lu.assertError( vpixels.derive, vpixels.gif(), "" )
  lu.assertError( vpixels.derive, derived_table, function() end )

  -- 3 args: error
  lu.assertError( vpixels.derive, vpixels.bmp(), {}, 3 )
  lu.assertError( vpixels.derive, vpixels.gif(), {}, "" )
  lu.assertError( vpixels.derive, derived_table, {}, function() end )
end

function TestDerive:testClosure()
  local derived_bmp = vpixels.derive( vpixels.bmp(1, 2, 3) )

  -- derived_bmp.width is a closure
  -- (because derived_bmp has no field 'width', __index() of its metatable
  --  gets called and returns a closure.)

  -- a closure is of type 'function'
  lu.assertEquals( "function", type(derived_bmp.width) )

  -- the closure calls super.width()
  lu.assertEquals( 2, derived_bmp.width(derived_bmp) )

  -- a closure is a return value of __index(), so c1 and c2
  -- are different, though they produce the same result
  local c1 = derived_bmp.width
  local c2 = derived_bmp.width
  lu.assertNotEquals( c1, c2 )
  lu.assertEquals( c1(derived_bmp), c2(derived_bmp) )

  -- error cases
  -- accept derived_bmp:width() or derived_bmp.width(derived_bmp)
  lu.assertError( derived_bmp.width ) -- derived_bmp.width()
  lu.assertError( derived_bmp.width, derived_bmp.super ) -- derived_bmp.width(derived_bmp.super)
  lu.assertError( derived_bmp.width, 2 )  -- derived_bmp.width( 2 )
  lu.assertError( derived_bmp.width, "" ) -- derived_bmp.width( "" )
  lu.assertError( derived_bmp.width, {} ) -- derived_bmp.width( {} )
end

function TestDerive:testIndexing()
  local derived_bmp = vpixels.derive( vpixels.bmp(4, 5, 6) )

  -- access methods of vp.bmp
  lu.assertEquals( 4, derived_bmp:bpp() )
  lu.assertEquals( 5, derived_bmp:width() )
  lu.assertEquals( 6, derived_bmp:height() )

  -- access meta fields of vp.bmp

  -- '__index' of vp.bmp: the metatable
  lu.assertEquals( "table",  type(derived_bmp.__index) )

  -- '__tostring' of vp.bmp: function
  -- since __tostring of vp.bmp points a function, derived_bmp.__tostring
  -- actually is a closure that calls the function pointed by __tostring of vp.bmp
  lu.assertEquals( "function", type(derived_bmp.__tostring) )

  -- __tostring() of vp.bmp gets called
  lu.assertEquals( derived_bmp:__tostring(), tostring(derived_bmp.super) )

  -- __tostring() of vp.bmp and __tostring() of vp.derived get called, respectively
  lu.assertNotEquals( derived_bmp:__tostring(), tostring(derived_bmp) )


  local derived_gif = vpixels.derive( vpixels.gif(2, 3, 4, 5) )

  -- access functions of vp.gif
  lu.assertEquals( 2, derived_gif:bpp() )
  lu.assertEquals( 3, derived_gif:width() )
  lu.assertEquals( 4, derived_gif:height() )
  lu.assertEquals( 5, derived_gif:images() )
  lu.assertEquals( true, derived_gif:colortable() )

  -- access images of vp.gif
  lu.assertEquals( "userdata", type(derived_gif[0]) )
  lu.assertEquals( "userdata", type(derived_gif[4]) )

  -- access meta fields of vp.gif

  -- '__index' of vp.gif: function
  -- since __index() of vp.gif is a function, derived_gif.__index actually
  -- is a closure that calls __index() of vp.gif
  lu.assertEquals( "function", type(derived_gif.__index) )

  -- derived_gif.__index(derived_gif, "bpp") works
  lu.assertEquals( "function", type(derived_gif.__index(derived_gif, "bpp")) )

  -- derived_gif.__index(derived_gif.super, "bpp") doesn't
  lu.assertError( derived_gif.__index, derived_gif.super, "bpp")

  -- '__tostring' of vp.gif: function
  lu.assertEquals( "function", type(derived_gif.__tostring) )

  -- __tostring() of vp.gif gets called
  lu.assertEquals( derived_gif:__tostring(), tostring(derived_gif.super) )

  -- __tostring() of vp.gif and __tostring() of vp.derived get called, respectively
  lu.assertNotEquals( derived_gif:__tostring(), tostring(derived_gif) )


  local derived_img = vpixels.derive( derived_gif[0] )

  -- access functions of vp.image
  lu.assertEquals( 2, derived_img:bpp() )
  lu.assertEquals( 0, derived_img:left() )
  lu.assertEquals( 0, derived_img:top() )
  lu.assertEquals( 3, derived_img:width() )
  lu.assertEquals( 4, derived_img:height() )
  lu.assertEquals( false, derived_img:colortable() )

  -- access meta fields of vp.gifimage

  -- '__index' of vp.gifimage: metatable
  lu.assertEquals( "table",  type(derived_img.__index) )

  -- '__tostring' of vp.gifimage: function
  -- since __tostring() of vp.gifimage is a function, derived_bmp.__tostring
  -- actually is a closure that calls __tostring() of vp.gifimage
  lu.assertEquals( "function", type(derived_img.__tostring) )

  -- __tostring() of vp.gifimage gets called
  lu.assertEquals( derived_img:__tostring(), tostring(derived_img.super) )

  -- __tostring() of vp.gifimage and __tostring() of vp.derived get called, respectively
  lu.assertNotEquals( derived_img:__tostring(), tostring(derived_img) )

  -- access methods of super objects
  local a = {}
  function a:foo() return "a" end
  local b = {}
  function b:bar() return "b" end
  vpixels.derive(a, b)
  local c = vpixels.derive(b)
  lu.assertEquals( "a", c:foo() )
  lu.assertEquals( "b", c:bar() )
end

function TestDerive:testOverride()
  local derived_bmp = vpixels.derive( vpixels.bmp(4, 2, 2) )

  -- before overriding: bpp() of vp.bmp gets called 
  lu.assertEquals( 4, derived_bmp:bpp() )

  -- override bpp()
  function derived_bmp:bpp()
    return 2
  end

  -- bpp() of derived_bmp gets called 
  lu.assertEquals( 2, derived_bmp:bpp() )

  -- call bpp() of vp.bmp
  lu.assertEquals( 4, derived_bmp.super:bpp() )

  -- override setcolor()
  function derived_bmp:setcolor()
    self.super:setcolor(0, 100, 110, 120)
    self.super:setcolor(1, 200, 210, 220)
  end

  -- setcolor() of derived_bmp gets called 
  derived_bmp:setcolor()
  r, b, g = derived_bmp:getcolor(0)
  lu.assertEquals( 100, r )
  lu.assertEquals( 110, b )
  lu.assertEquals( 120, g )
  r, b, g = derived_bmp:getcolor(1)
  lu.assertEquals( 200, r )
  lu.assertEquals( 210, b )
  lu.assertEquals( 220, g )
end

function TestDerive:testTostring()
  local derived_bmp = vpixels.derive( vpixels.bmp() )
  lu.assertNotEquals( nil, string.find( tostring(derived_bmp), "vp.derived", 1, true ) )
  lu.assertNotEquals( nil, string.find( tostring(derived_bmp), "vp.bmp", 1, true ) )

  local derived_gif = vpixels.derive( vpixels.gif() )
  lu.assertNotEquals( nil, string.find( tostring(derived_gif), "vp.derived", 1, true ) )
  lu.assertNotEquals( nil, string.find( tostring(derived_gif), "vp.gif", 1, true ) )

  local derived_img = vpixels.derive( derived_gif[0] )
  lu.assertNotEquals( nil, string.find( tostring(derived_img), "vp.derived", 1, true ) )
  lu.assertNotEquals( nil, string.find( tostring(derived_img), "vp.gifimage", 1, true ) )
end

function TestDerive:testLengthOp()
  local derived_bmp = vpixels.derive( vpixels.bmp(4, 2, 2) )
  lu.assertEquals( 0, #derived_bmp )

  local derived_gif = vpixels.derive( vpixels.gif(2, 3, 4, 2) )
  lu.assertEquals( 2, #derived_gif )

  local derived_img = vpixels.derive( derived_gif[0] )
  lu.assertEquals( 0, #derived_img )
end

function TestDerive:testMetatable()
  local derived_bmp = vpixels.derive( vpixels.bmp() )

  -- accessing metatable is not allowed
  local mt = getmetatable(derived_bmp)
  lu.assertEquals( "string", type(mt) )

  -- changing metatable is not allowed
  lu.assertError( setmetatable, derived_bmp, {} )
end


local runner = lu.LuaUnit.new()
--runner:setOutputType("tap")
os.exit( runner:runSuite() )
