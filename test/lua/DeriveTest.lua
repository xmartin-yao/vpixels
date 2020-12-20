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
  local bmp = vpixels.bmp(4, 5, 6)
  local derived_bmp = vpixels.derive( bmp )

  -- access methods of vp.bmp
  lu.assertEquals( 4, derived_bmp:bpp() )
  lu.assertEquals( 5, derived_bmp:width() )
  lu.assertEquals( 6, derived_bmp:height() )

  -- access meta fields of vp.bmp

  -- '__index' of vp.bmp: a metatable
  lu.assertEquals( "table",  type(derived_bmp.__index) )
  lu.assertEquals( derived_bmp.__index, bmp.__index )

  -- '__tostring' of vp.bmp: function
  -- since __tostring of vp.bmp points to a function, derived_bmp.__tostring
  -- actually is a closure that calls the function pointed by __tostring of vp.bmp
  lu.assertEquals( "function", type(derived_bmp.__tostring) )
  lu.assertNotEquals( derived_bmp.__tostring, bmp.__tostring )

  -- __tostring() of vp.bmp gets called
  lu.assertEquals( derived_bmp:__tostring(), tostring(derived_bmp.super) )

  -- __tostring() of vp.bmp and __tostring() of vp.derived get called, respectively
  lu.assertNotEquals( derived_bmp:__tostring(), tostring(derived_bmp) )


  local gif = vpixels.gif(2, 3, 4, 5)
  local derived_gif = vpixels.derive( gif )

  -- access functions of vp.gif
  lu.assertEquals( 2, derived_gif:bpp() )
  lu.assertEquals( 3, derived_gif:width() )
  lu.assertEquals( 4, derived_gif:height() )
  lu.assertEquals( 5, derived_gif:images() )
  lu.assertTrue( derived_gif:colortable() )

  -- access images of vp.gif
  lu.assertEquals( "userdata", type(derived_gif[0]) )
  lu.assertEquals( "userdata", type(derived_gif[4]) )

  -- access meta fields of vp.gif

  -- '__index' of vp.gif: a function
  -- since __index() of vp.gif is a function, derived_gif.__index actually
  -- is a closure that calls __index() of vp.gif
  lu.assertEquals( "function", type(derived_gif.__index) )
  lu.assertNotEquals( derived_gif.__index, gif.__index )

  -- derived_gif.__index(derived_gif, "bpp") works
  lu.assertEquals( "function", type(derived_gif.__index(derived_gif, "bpp")) )

  -- derived_gif.__index(derived_gif.super, "bpp") doesn't
  lu.assertError( derived_gif.__index, derived_gif.super, "bpp")

  -- '__tostring' of vp.gif: a function
  lu.assertEquals( "function", type(derived_gif.__tostring) )
  lu.assertNotEquals( derived_gif.__tostring, gif.__tostring )

  -- __tostring() of vp.gif gets called
  lu.assertEquals( derived_gif:__tostring(), tostring(derived_gif.super) )

  -- __tostring() of vp.gif and __tostring() of vp.derived get called, respectively
  lu.assertNotEquals( derived_gif:__tostring(), tostring(derived_gif) )


  local img = derived_gif[0]
  local derived_img = vpixels.derive( img )

  -- access functions of vp.image
  lu.assertEquals( 2, derived_img:bpp() )
  lu.assertEquals( 0, derived_img:left() )
  lu.assertEquals( 0, derived_img:top() )
  lu.assertEquals( 3, derived_img:width() )
  lu.assertEquals( 4, derived_img:height() )
  lu.assertFalse( derived_img:colortable() )

  -- access meta fields of vp.gifimage

  -- '__index' of vp.gifimage: a metatable
  lu.assertEquals( "table",  type(derived_img.__index) )
  lu.assertEquals( derived_img.__index, img.__index )

  -- '__tostring' of vp.gifimage: function
  -- since __tostring() of vp.gifimage is a function, derived_bmp.__tostring
  -- actually is a closure that calls __tostring() of vp.gifimage
  lu.assertEquals( "function", type(derived_img.__tostring) )
  lu.assertNotEquals( derived_img.__tostring, img.__tostring )

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
  lu.assertNotEquals( nil, string.find( tostring(derived_bmp), "derived", 1, true ) )
  lu.assertNotEquals( nil, string.find( tostring(derived_bmp), "bmp", 1, true ) )

  local derived_gif = vpixels.derive( vpixels.gif() )
  lu.assertNotEquals( nil, string.find( tostring(derived_gif), "derived", 1, true ) )
  lu.assertNotEquals( nil, string.find( tostring(derived_gif), "gif", 1, true ) )

  local derived_img = vpixels.derive( derived_gif[0] )
  lu.assertNotEquals( nil, string.find( tostring(derived_img), "derived", 1, true ) )
  lu.assertNotEquals( nil, string.find( tostring(derived_img), "gifimage", 1, true ) )
end

function TestDerive:testLengthOp()
  local derived_gif = vpixels.derive( vpixels.gif(2, 3, 4, 5) )
  lu.assertEquals( #derived_gif, 5 )
  local derived_gif2 = vpixels.derive( derived_gif )
  lu.assertEquals( #derived_gif2, 5 )  -- delegated to the very base

  -- vp.bmp does not support # operator
  local derived_bmp = vpixels.derive( vpixels.bmp(4, 2, 2) )
  local mtlen = getmetatable(derived_bmp).__len
  lu.assertEquals( type(mtlen), "function" )
  lu.assertError( mtlen, derived_bmp ) -- #derived_bmp

  -- vp.gifimage does not support # operator
  local derived_img = vpixels.derive( derived_gif[0] )
  mtlen = getmetatable(derived_bmp).__len
  lu.assertEquals( type(mtlen), "function" )
  lu.assertError( mtlen, derived_img ) -- #derived_img
end

function TestDerive:testIPairs()
  local gif = vpixels.gif(3, 4, 5, 3)
  local derived_gif1 = vpixels.derive(gif)
  local derived_gif2 = vpixels.derive(derived_gif1)

  -- all have meta method __ipairs
  local gif_ipairs = getmetatable(gif).__ipairs
  local derived1_ipairs = getmetatable(derived_gif1).__ipairs
  local derived2_ipairs = getmetatable(derived_gif2).__ipairs
  lu.assertEquals( type(gif_ipairs), 'function' )
  lu.assertEquals( type(derived1_ipairs), 'function' )
  lu.assertEquals( type(derived2_ipairs), 'function' )
  lu.assertEquals( derived1_ipairs, derived2_ipairs )  -- same function
  lu.assertNotEquals( derived1_ipairs, gif_ipairs )  -- different

  -- returns of ipairs()
  local iter, state, var = ipairs(gif)
  lu.assertEquals( type(iter), "function" )
  lu.assertEquals( type(state), "userdata" )
  lu.assertStrContains( tostring(state), 'gif' )
  lu.assertEquals( var, -1 )

  local iter1, state1, var = ipairs(derived_gif1)
  lu.assertEquals( type(iter1), "function" )
  lu.assertEquals( type(state1), "userdata" )
  lu.assertStrContains( tostring(state1), 'gif' )
  lu.assertEquals( var, -1 )

  local iter2, state2, var = ipairs(derived_gif2)
  lu.assertEquals( type(iter2), "function" )
  lu.assertEquals( type(state2), "userdata" )
  lu.assertStrContains( tostring(state2), 'gif' )
  lu.assertEquals( var, -1 )

  lu.assertEquals( iter1, iter )  -- same iterator
  lu.assertEquals( iter2, iter )
  lu.assertEquals( state1, state )  -- same state: vp.gif object
  lu.assertEquals( state2, state )

  -- verify correctness of iteration
  for i, img in ipairs(derived_gif1) do
    lu.assertEquals( img:getpixel(i, i), 0 )
    lu.assertEquals( img:getpixel(i + 1, i + 1), 0 )
  end

  for i, img in ipairs(derived_gif2) do
    lu.assertEquals( img:getpixel(i, i), 0 )
    lu.assertEquals( img:getpixel(i + 1, i + 1), 0 )
  end

  for i, img in ipairs(gif) do
    img:setpixel(i + 1, i + 1, i + 1)
  end

  for i, img in ipairs(derived_gif1) do
    lu.assertEquals( img:getpixel(i, i), 0 )
    lu.assertEquals( img:getpixel(i + 1, i + 1), i + 1 )
  end

  for i, img in ipairs(derived_gif2) do
    lu.assertEquals( img:getpixel(i, i), 0 )
    lu.assertEquals( img:getpixel(i + 1, i + 1), i + 1 )
  end

  -- prepare for malfunction and error cases
  local derived_ipairs = getmetatable(derived_gif2).__ipairs
  local table_iter, _, _ = ipairs( {} )

  -- malfunction case: __ipairs of vp.derived removed
  getmetatable(derived_gif2).__ipairs = nil
  -- no problem to call ipairs(), but returns are different
  iter2, state2, var = ipairs(derived_gif2)
  -- a different iterator
  lu.assertEquals( type(iter2), "function" )
  lu.assertEquals( iter2, table_iter ) -- the default iterator for a table
  lu.assertNotEquals( iter2, iter )
  -- different state: vp.derived instead of vp.gif
  lu.assertEquals( type(state2), "table" )
  lu.assertStrContains( tostring(state2), 'derived' )
  -- different var value
  lu.assertEquals( var, 0 )  -- 0 instead of -1

  -- error case: replace __ipairs of vp.derived with a string
  getmetatable(derived_gif2).__ipairs = ""
  lu.assertError( ipairs, derived_gif2 )

  -- restore __ipairs of vp.derived
  getmetatable(derived_gif2).__ipairs = derived_ipairs

  -- error case: replace __ipairs of vp.gif with a string
  getmetatable(gif).__ipairs = ""
  lu.assertError( ipairs, derived_gif )
  lu.assertError( ipairs, derived_gif2 )

  -- error case: __ipairs of vp.gif removed
  getmetatable(gif).__ipairs = nil
  lu.assertError( ipairs, derived_gif )
  lu.assertError( ipairs, derived_gif2 )

  -- error, b/c vp.bmp has no meta method __ipairs
  local bmp = vpixels.bmp()
  local derived_bmp = vpixels.derive( bmp )
  lu.assertError( ipairs, derived_bmp )
  lu.assertEquals( getmetatable(bmp).__ipairs, nil )
  lu.assertEquals( getmetatable(derived_bmp).__ipairs, derived_ipairs )

  -- error, b/c vp.gifimage has no meta method __ipairs
  local img = gif[0]
  local derived_img = vpixels.derive( img )
  lu.assertError( ipairs, derived_img )
  lu.assertEquals( getmetatable(img).__ipairs, nil )
  lu.assertEquals( getmetatable(derived_img).__ipairs, derived_ipairs )
end

function TestDerive:testMetatable()
  local derived_bmp = vpixels.derive( vpixels.bmp() )

  -- allow access to metatable
  local mt = getmetatable(derived_bmp)
  lu.assertEquals( type(mt), "table" )

  -- metatable has its string representation
  lu.assertStrContains( tostring(mt), 'derived' )

  -- metatable has its own metatable
  lu.assertEquals( type(getmetatable(mt)), "table" )

  -- modifying metatable is allowed
  -- modification affects all vp.gifimage objects
  -- (see malfunction and error cases in TestDerive:testIPairs() )

  -- changing metatable is allowed
  setmetatable( derived_bmp, {} )
  lu.assertEquals( #getmetatable(derived_bmp), 0 )
end


local runner = lu.LuaUnit.new()
--runner:setOutputType("tap")
os.exit( runner:runSuite() )
