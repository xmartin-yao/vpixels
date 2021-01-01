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
  local bmp = vpixels.bmp(1, 2, 3)
  local derived_bmp = vpixels.derive( bmp )

  -- derived_bmp.width is a closure
  -- (because derived_bmp has no field 'width', __index() of its metatable
  --  gets called and returns a closure.)

  -- a closure is of type 'function'
  lu.assertEquals( "function", type(derived_bmp.width) )

  -- upvalue of the closure is bmp.width
  local _, up = debug.getupvalue( derived_bmp.width, 1 )
  lu.assertEquals( up, bmp.width )

  -- the closure calls bmp.width()
  lu.assertEquals( derived_bmp.width(derived_bmp), bmp:width() )

  -- a closure is a return value of __index(), so c1 and c2
  -- are different, though they return the same result
  local c1 = derived_bmp.width
  local c2 = derived_bmp.width
  lu.assertNotEquals( c1, c2 )
  lu.assertEquals( c1(derived_bmp), c2(derived_bmp) )
  lu.assertEquals( c1(derived_bmp), bmp:width() )

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
  local index = getmetatable(derived_bmp).__index

  -- expect 2 arguments
  lu.assertError( index, derived_bmp )
  lu.assertError( index, derived_bmp, 'bpp', 2 )

  -- 1st argument must be vp.derive
  lu.assertError( index, 'bpp', 3 )
  lu.assertError( index, bmp, 'bpp' )
  lu.assertError( index, {}, 'bpp' )

  -- if the key is the name of a function of bmp, __index returns
  -- a closure of the function, not the function itself
  local bpp1 = index( derived_bmp, 'bpp' )  -- bpp1 = derived_bmp.bpp
  local bpp2 = derived_bmp.bpp
  -- bpp1 and bpp2 are closures
  lu.assertEquals( type(bpp1), "function" ) -- they are functions
  lu.assertEquals( type(bpp2), "function" )
  lu.assertNotEquals( bpp1, bpp2 )     -- they are not the same
  lu.assertNotEquals( bpp1, bmp.bpp )  -- they are not the function they enclose
  lu.assertNotEquals( bpp2, bmp.bpp )
  -- bpp1 and bpp2 have the same upvalue
  local _, up1 = debug.getupvalue( bpp1, 1 )
  local _, up2 = debug.getupvalue( bpp2, 1 )
  lu.assertEquals( up1, up2 )
  lu.assertEquals( up1, bmp.bpp )  -- upvalue is function bmp.bpp
  -- bpp1 and bpp2 return same result, b/c they call the same function bmp.bpp()
  lu.assertEquals( bpp1(derived_bmp), bpp2(derived_bmp) )
  lu.assertEquals( bpp1(derived_bmp), bmp:bpp() )

  -- access methods of bmp
  lu.assertEquals( 4, derived_bmp:bpp() )
  lu.assertEquals( 5, derived_bmp:width() )
  lu.assertEquals( 6, derived_bmp:height() )

  -- access meta fields of bmp
  -- '__tostring' of bmp: a function
  -- derived_bmp.__tostring is a closure that encloses __tostring of bmp
  lu.assertEquals( type(derived_bmp.__tostring), "function" )
  lu.assertNotEquals( derived_bmp.__tostring, bmp.__tostring )  -- they are not the same
  lu.assertEquals( derived_bmp:__tostring(), bmp:__tostring() ) -- their results are the same

  local gif = vpixels.gif(2, 3, 4, 5)
  local derived_gif = vpixels.derive( gif )

  -- access functions of gif
  lu.assertEquals( 2, derived_gif:bpp() )
  lu.assertEquals( 3, derived_gif:width() )
  lu.assertEquals( 4, derived_gif:height() )
  lu.assertEquals( 5, derived_gif:images() )
  lu.assertTrue( derived_gif:colortable() )

  -- access images of gif
  lu.assertEquals( type(derived_gif[0]), "userdata" )
  lu.assertEquals( type(derived_gif[4]), "userdata" )
  lu.assertStrContains( tostring(derived_gif[0]), "gifimage" )
  lu.assertStrContains( tostring(derived_gif[4]), "gifimage" )

  -- access meta fields of vp.gif
  -- '__index' of gif: a function
  -- derived_gif.__index actually is a closure that encloses __index of gif
  lu.assertEquals( "function", type(derived_gif.__index) )
  lu.assertNotEquals( derived_gif.__index, gif.__index )  -- they are not the same
  lu.assertEquals( derived_gif:__index('bpp'), gif:__index('bpp') )  -- their results are the same
  lu.assertEquals( derived_gif:__index('bpp'), gif.bpp )             -- i.e. bpp() of gif

  local img = derived_gif[0]
  local derived_img = vpixels.derive( img )

  -- access functions of img
  lu.assertEquals( 2, derived_img:bpp() )
  lu.assertEquals( 0, derived_img:left() )
  lu.assertEquals( 0, derived_img:top() )
  lu.assertEquals( 3, derived_img:width() )
  lu.assertEquals( 4, derived_img:height() )
  lu.assertFalse( derived_img:colortable() )

  -- access meta fields of img
  -- '__index' of img: a table (the metatable of vp.gifimage)
  -- derived_img.__index returns __index of img
  lu.assertEquals( type(derived_img.__index), "table" )
  lu.assertEquals( derived_img.__index, img.__index )

  -- access methods of super objects
  local a = {}
  function a:foo() return "a" end

  local b = vpixels.derive(a)
  function b:bar() return "b" end

  local c = vpixels.derive(b)
  lu.assertEquals( "a", c:foo() )
  lu.assertEquals( "b", c:bar() )
end

function TestDerive:testOverride()
  local bmp = vpixels.bmp(4, 2, 2)
  local derived_bmp = vpixels.derive( bmp )

  -- before overriding: bpp() of bmp gets called
  lu.assertEquals( 4, derived_bmp:bpp() )

  -- override bpp()
  function derived_bmp:bpp()
    return 2
  end

  function bmp:bpp()
    return 6
  end

  -- call overridden bpp() of derived_bmp
  lu.assertEquals( 2, derived_bmp:bpp() )
  --[[
  derived_bmp:bpp()
    1) resolve derived_bmp.bpp: don't need to call __index, b/c derived_bmp
       is a table and newly added bpp() is stored in it.
    2) call bpp(derived_bmp) and yield 2.
  ]]

  -- call overridden bpp() of bmp
  lu.assertEquals( 6, derived_bmp.super:bpp() )
  --[[
  derived_bmp.super:bpp()
    1) resolve derived_bmp.super: yield bmp. don't need to call __index,
       b/c derived_bmp is a table and 'super' is a field that stores bmp.
    2) resolve bmp.bpp: call __index of bmp with bmp and 'bpp' as arguments,
       which returns bpp() from uservalue of bmp.
    3) call bpp(bmp) and yield 6.
  ]]

  -- call original bpp() of bmp
  lu.assertEquals( 4, derived_bmp.base:bpp() )
  lu.assertEquals( 4, derived_bmp.super.base:bpp() )
  --[[
  derived_bmp.base:bpp()
   1) resolve derived_bmp.base: call __index of derived_bmp with derived_bmp
      and 'base' as arguments, which in turn calls __index of bmp with bmp and
      'bpp' as arguments, which sets metatable-only and returns bmp.
   2) resolve bmp.bpp: call __index of bmp with bmp and 'bpp' as arguments,
      which returns bpp() defined in bmp's metatable.
   3) call bpp(bmp) and yield 4.

  derived_bmp.super.base:bpp()
   1) resolve derived_bmp.super: same as step 1) of derived_bmp.super:bpp().
   2) resolve bmp.base: call call __index of bmp with bmp and 'base'
      as arguments, which sets metatable-only and returns bmp.
   3) resolve bmp.bpp: same as step 2) of derived_bmp.base:bpp().
   4) call bpp(bmp) and yield 4.
  ]]

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
  lu.assertStrContains( tostring(derived_bmp), "derived" )
  lu.assertStrContains( tostring(derived_bmp), "bmp" )

  local derived_gif = vpixels.derive( vpixels.gif() )
  lu.assertStrContains( tostring(derived_gif), "derived" )
  lu.assertStrContains( tostring(derived_gif), "gif" )

  local derived_img = vpixels.derive( derived_gif[0] )
  lu.assertStrContains( tostring(derived_img), "derived" )
  lu.assertStrContains( tostring(derived_img), "gifimage" )
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
