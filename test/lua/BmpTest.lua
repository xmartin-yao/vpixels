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
  local bmp = vpixels.bmp( 4, 5, 6 )

  -- import non-existing file
  lu.assertError( bmp.import, bmp, 'not-exits.bmp' ) -- not exit
  -- bmp unchanged
  lu.assertEquals( 4, bmp:bpp() )
  lu.assertEquals( 5, bmp:width() )
  lu.assertEquals( 6, bmp:height() )
  lu.assertEquals( 16, bmp:colortablesize() )

  -- import() expects a string argument
  lu.assertError( bmp.import, bmp, 2 )
  lu.assertError( bmp.import, bmp, {} )
  -- bmp unchanged
  lu.assertEquals( 4, bmp:bpp() )
  lu.assertEquals( 5, bmp:width() )
  lu.assertEquals( 6, bmp:height() )
  lu.assertEquals( 16, bmp:colortablesize() )

  -- import non-bmp file or corrupted bmp file
  lu.assertError( bmp.import, bmp, 'BmpTest.lua' )   -- not a BMP file
  -- bmp becomes invalid, results of calling its methods are undefined
end

function TestBmp:testExport()
  local bmp = vpixels.bmp()
  bmp:export( 'temp.bmp', true )
  lu.assertError( bmp.export, bmp, 'temp.bmp' )
  lu.assertError( bmp.export, bmp, 'temp.bmp', false )
end

-- test LuaUtil::NewIndex()
-- b/c __newindex of LuaBmp delegates to LuaUtil::NewIndex()
function TestBmp:testNewIndex()
  local bmp = vpixels.bmp( 4, 5, 6 )
  local newindex = getmetatable( bmp ).__newindex

  -- bmp has uservalue
  local uvalue = debug.getuservalue( bmp )
  lu.assertNotEquals( uvalue, nil )
  lu.assertEquals( type(uvalue), 'table' )  -- uservalue is a table
  lu.assertEquals( type(uvalue.base), 'boolean' ) -- field 'base' stores a boolean

  -- key named 'base' is reserved and unmodifiable
  lu.assertError( newindex, bmp, 'base', nil )   -- bmp.base = nil
  lu.assertError( newindex, bmp, 'base', {} )    -- bmp.base = {}
  lu.assertError( newindex, bmp, 'base', 3.14 )  -- bmp.base = 3.14
  lu.assertError( newindex, bmp, 'base', "string" )  -- bmp.base = "string"
  lu.assertError( newindex, bmp, 'base', function() end )  -- bmp.base = function() end

  -- expect three arguments
  lu.assertError( newindex )
  lu.assertError( newindex, bmp )
  lu.assertError( newindex, bmp, 'pi' )
  lu.assertError( newindex, bmp, 'pi', 3.14, 3.14 )

  -- 1st argument must be a userdata that has a uservalue
  debug.setuservalue( bmp, nil )  -- uservalue removed
  lu.assertError( newindex, bmp, 'pi', 3.14 )

  debug.setuservalue( bmp, {} ) -- uservalue (a table) has no 'base' field
  lu.assertError( newindex, bmp, 'pi', 3.14 )

  -- restore uservalue
  debug.setuservalue( bmp, uvalue )

  -- error cases: 1st argument is not userdata
  lu.assertError( newindex, {}, 'pi', 3.14 )
  lu.assertError( newindex, 3.14, 'pi', 3.14 )
  lu.assertError( newindex, function() end, 'pi', 3.14 )

  -- 2nd argument can be any value
  newindex( bmp, 'pi', 3.14 )  -- bmp.pi = 3.14
  lu.assertEquals( bmp.pi, 3.14 )

  newindex( bmp, 0, "number key" )  -- bmp[0] = "number key"
  lu.assertEquals( bmp[0], "number key" )

  newindex( bmp, '0', "string key" )  -- bmp['0'] = "string key"
  lu.assertEquals( bmp['0'], "string key" )

  newindex( bmp, bmp, "bmp for key" )  -- bmp[bmp] = "bmp for key"
  lu.assertEquals( bmp[bmp], "bmp for key" )

  local t = {}
  newindex( bmp, t, "table for key" )  -- bmp[t] = "table for key"
  lu.assertEquals( bmp[t], "table for key" )

  local f = function() end
  newindex( bmp, f, "function for key" )  -- bmp[f] = "function for key"
  lu.assertEquals( bmp[f], "function for key" )

  -- assigned values are stored in uservalue
  lu.assertEquals( uvalue.pi,   3.14 )
  lu.assertEquals( uvalue[0],   "number key" )
  lu.assertEquals( uvalue['0'], "string key" )
  lu.assertEquals( uvalue[bmp], "bmp for key" )
  lu.assertEquals( uvalue[t],   "table for key" )
  lu.assertEquals( uvalue[f],   "function for key" )

  -- 3rd argument can be any value
  newindex( bmp, 'var', bmp )  -- bmp.var = bmp
  lu.assertEquals( bmp.var, bmp )
  lu.assertEquals( uvalue.var, bmp )

  newindex( bmp, 'var', t )  -- bmp.var = t
  lu.assertEquals( bmp.var, t )
  lu.assertEquals( uvalue.var, t )

  newindex( bmp, 'var', f )  -- bmp.var = f
  lu.assertEquals( bmp.var, f )
  lu.assertEquals( uvalue.var, f )

  -- do NOT directly refer 'base' when doing assignment operation.
  -- although the following example is syntactically correct and runs,
  -- it doesn't add a field 'pi' to 'base' and leaves bmp in an abnormal status.
  bmp.base.pi = 3.1415
  -- to accomplish this task, both __index and __newindex are called.
  -- 1) __index gets called to resolve bmp.base and it returns bmp,
  --    but it sets 'base' of bmp's uservalue to true.
  lu.assertEquals( uvalue.base, true )
  -- 2) __newindex gets called with bmp, pi and 3.1415. it adds
  --    a new field 'pi' to uservalue.
  lu.assertEquals( uvalue.pi, 3.1415 )
  -- when pi is retrieved,
  --    e.g  x = bmp.pi  or  x = bmp.base.pi
  -- __index gets called. __index won't search uservalue but metatable of bmp,
  -- b/c 'base' was set to true during the last round. so it fails
  lu.assertError( getmetatable(bmp).__index, bmp, 'pi' )
  -- before __index errors out, it sets 'base' back to false
  lu.assertEquals( uvalue.base, false )
end

-- test LuaUtil::testIndexing()
-- b/c __index of LuaBmp delegates to LuaUtil::testIndexing()
function TestBmp:testIndexing()
  local bmp = vpixels.bmp( 4, 5, 6 )
  local index = getmetatable( bmp ).__index
  local uvalue = debug.getuservalue( bmp )
  local ret = nil

  -- key named 'base' is reserved, calling __index with 'base' returns
  -- the bmp object itself. two consecutive calls to __index with key 'base'
  --    e.g. bmp.base.base
  -- trigger an error (see override a method in testExtendBmp() )
  ret = index( bmp, 'base' )   -- bmp.base
  lu.assertEquals( ret, bmp )  -- bmp.base returns bmp
  lu.assertError( index, bmp, 'base' )  -- calling __index again triggers an error

  -- expect two arguments
  lu.assertError( index )
  lu.assertError( index, bmp )
  lu.assertError( index, bmp, 'var', 2 )

  -- 1st argument must be a userdata that has a uservalue
  debug.setuservalue( bmp, nil )  -- uservalue removed
  lu.assertError( index, bmp, 'pi' )

  debug.setuservalue( bmp, {} ) -- uservalue (a table) has no 'base' field
  lu.assertError( index, bmp, 'pi' )

  -- restore uservalue
  debug.setuservalue( bmp, uvalue )

  -- error cases: 1st argument is not userdata
  lu.assertError( index, {}, 'pi' )
  lu.assertError( index, 3.14, 'pi' )
  lu.assertError( index, function() end, 'pi' )

  -- 2nd argument can be any value
  bmp[1] = 0.01
  lu.assertEquals( index(bmp, 1), 0.01 )

  bmp.pi = 3.14
  lu.assertEquals( index(bmp, 'pi'), 3.14 )

  local t = {}
  bmp[t] = t
  lu.assertEquals( index(bmp, t), t )

  local f = function() end
  bmp[f] = f
  lu.assertEquals( index(bmp, f), f )
end

-- test that LuaUtil::testIndexing() and LuaUtil::testNewIndex()
-- work correctly to extend vp.bmp object
function TestBmp:testExtendBmp()
  local bmp1 = vpixels.bmp( 4, 5, 6 )
  local bmp2 = vpixels.bmp( 8, 9, 10 )

  -- add variables to bmp1
  bmp1.number = 0  -- number
  lu.assertEquals( bmp1.number, 0 )

  bmp1.name = "a name"  -- string
  lu.assertEquals( bmp1.name, "a name" )

  bmp1.consts = { 0.0, 0.0 }  -- table
  lu.assertEquals( bmp1.consts[1], 0.0 )
  lu.assertEquals( bmp1.consts[2], 0.0 )

  -- change values of variables
  bmp1.number = 1
  lu.assertEquals( bmp1.number, 1 )

  bmp1.name = "a vp.bmp object"
  lu.assertEquals( bmp1.name, "a vp.bmp object" )

  bmp1.consts[1] = 0.01
  bmp1.consts[2] = 0.02
  lu.assertEquals( bmp1.consts[1], 0.01 )
  lu.assertEquals( bmp1.consts[2], 0.02 )

  -- add methods to bmp1
  function bmp1:getnumber() return bmp1.number end
  lu.assertEquals( bmp1:getnumber(), 1 )

  function bmp1:getname() return bmp1.name end
  lu.assertEquals( bmp1:getname(), "a vp.bmp object" )

  function bmp1:getconst(i) return bmp1.consts[i] end
  lu.assertEquals( bmp1:getconst(1), 0.01 )
  lu.assertEquals( bmp1:getconst(2), 0.02 )
  lu.assertEquals( bmp1:getconst(3), nil )

  function bmp1:getbpp() return bmp1:bpp() end  -- call method defined in metatable
  lu.assertEquals( bmp1:getbpp(), 4 )

  -- redefine methods
  function bmp1:getnumber() return bmp1.name end
  lu.assertEquals( bmp1:getnumber(), "a vp.bmp object" )

  function bmp1:getname() return bmp1.number end
  lu.assertEquals( bmp1:getname(), 1 )

  function bmp1:getbpp() return bmp1:width() end
  lu.assertEquals( bmp1:getbpp(), 5 )

  -- override a method defined in metatable of vp.bmp
  function bmp1:bpp() return 0 end
  lu.assertEquals( bmp1:bpp(), 0 )

  -- using 'base' key to access the method defined in metatable of vp.bmp
  lu.assertEquals( bmp1.base:bpp(), 4 )
  lu.assertEquals( bmp1.base.bpp(bmp1), 4 )

  -- these are syntactically correct, but treated as error
  --   bmp1.base.base:bpp()  or  bmp1.base.base.bpp(bmp1)
  -- (see two consecutive calls to __index with key 'base' in testIndexing() )

  -- all modifications affect bmp1 only
  -- bmp2 doesn't get the overridden method
  lu.assertNotEquals( bmp2:bpp(), bmp1:bpp() )
  lu.assertEquals( bmp2:bpp(), 8 )
  -- newly added data are not available for bmp2
  local index = getmetatable(bmp2).__index
  lu.assertError( index, bmp2, 'number' )  -- bmp2.number
  lu.assertError( index, bmp2, 'name' )    -- bmp2.name
  lu.assertError( index, bmp2, 'consts' )  -- bmp2.consts
  lu.assertError( index, bmp2, 'getnumber' )  -- bmp2.getnumber
  lu.assertError( index, bmp2, 'getname' )  -- bmp2.getname
  lu.assertError( index, bmp2, 'getbpp' )   -- bmp2.getbpp
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
