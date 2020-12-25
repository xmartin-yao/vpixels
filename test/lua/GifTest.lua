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

-- vp.gif unit tests

local lu = require( "luaunit" )

-- load vpixels from current directory
if package.config:sub( 1, 1 ) == "\\" then
  package.cpath = '.\\?.dll'
else
  package.cpath = './?.so'
end
local vpixels = require( "vpixels" )


TestGif = {}

-- no arg
function TestGif:testNew()
  local gif = vpixels.gif()

  lu.assertEquals( gif:bpp(), 2 )
  lu.assertEquals( gif:bitsperpixel(), 2 )

  local w, h = gif:dimension()
  lu.assertEquals( w, 1 )
  lu.assertEquals( h, 1 )
  lu.assertEquals( gif:width(), 1 )
  lu.assertEquals( gif:height(), 1 )

  lu.assertTrue( gif:colortable() )
  lu.assertFalse( gif:colorsorted() )
  lu.assertEquals( gif:colortablesize(), 4 )

  lu.assertEquals( gif:images(), 1 )
  lu.assertEquals( #gif, 1 )
  lu.assertError( gif.getimage, gif, 1 )
  lu.assertError( gif.getimage, gif, -1 )
end

-- 3 args
function TestGif:testNew3()
  local gif = vpixels.gif( 3, 4, 5 )

  lu.assertEquals( gif:bpp(), 3 )
  lu.assertEquals( gif:bitsperpixel(), 3 )

  local w, h = gif:dimension()
  lu.assertEquals( w, 4 )
  lu.assertEquals( h, 5 )
  lu.assertEquals( gif:width(), 4 )
  lu.assertEquals( gif:height(), 5 )

  lu.assertTrue( gif:colortable() )
  lu.assertFalse( gif:colorsorted() )

  lu.assertEquals( gif:images(), 1 )
  lu.assertEquals( #gif, 1 )
  lu.assertError( gif.getimage, gif, 1 )
  lu.assertError( gif.getimage, gif, -1 )
end

-- 4 args
function TestGif:testNew4()
  local gif = vpixels.gif( 3, 4, 5, 2 )

  lu.assertEquals( gif:bpp(), 3 )
  lu.assertEquals( gif:bitsperpixel(), 3 )

  local w, h = gif:dimension()
  lu.assertEquals( w, 4 )
  lu.assertEquals( h, 5 )
  lu.assertEquals( gif:width(), 4 )
  lu.assertEquals( gif:height(), 5 )

  lu.assertTrue( gif:colortable() )
  lu.assertFalse( gif:colorsorted() )

  lu.assertEquals( gif:images(), 2 )
  lu.assertEquals( #gif, 2 )
  lu.assertError( gif.getimage, gif, 2 )
  lu.assertError( gif.getimage, gif, -1 )
end

-- 5 args
function TestGif:testNew5()
  local gif = vpixels.gif( 3, 4, 5, 2, true )

  lu.assertEquals( gif:bpp(), 3 )
  lu.assertEquals( gif:bitsperpixel(), 3 )

  local w, h = gif:dimension()
  lu.assertEquals( w, 4 )
  lu.assertEquals( h, 5 )
  lu.assertEquals( gif:width(), 4 )
  lu.assertEquals( gif:height(), 5 )

  lu.assertTrue( gif:colortable() )     -- has global color table
  lu.assertFalse( gif:colorsorted() )
  lu.assertFalse( gif[0]:colortable() ) -- no local color table

  lu.assertEquals( gif:images(), 2 )
  lu.assertEquals( #gif, 2 )

  gif = vpixels.gif( 3, 4, 5, 2, false )

  lu.assertEquals( gif:bpp(), 3 )
  lu.assertEquals( gif:bitsperpixel(), 3 )

  local w, h = gif:dimension()
  lu.assertEquals( w, 4 )
  lu.assertEquals( h, 5 )
  lu.assertEquals( gif:width(), 4 )
  lu.assertEquals( gif:height(), 5 )

  lu.assertFalse( gif:colortable() )   -- no global color table
  lu.assertFalse( gif:colorsorted() )
  lu.assertTrue( gif[0]:colortable() ) -- has local color table

  lu.assertEquals( gif:images(), 2 )
  lu.assertEquals( #gif, 2 )
end

function TestGif:testNewBadArgs()
  -- number of args is incorrect
  lu.assertError( vpixels.gif, 2 )
  lu.assertError( vpixels.gif, 2, 3 )

  -- negative integer
  lu.assertError( vpixels.gif, -2, 3, 4 )
  lu.assertError( vpixels.gif, 2, -3, 4 )
  lu.assertError( vpixels.gif, 2, 3, -4 )
  lu.assertError( vpixels.gif, 2, 3, 4, -5 )
  lu.assertError( vpixels.gif, 2.1, 3, 4 )
  lu.assertError( vpixels.gif, 2, 3.2, 4 )
  lu.assertError( vpixels.gif, 2, 3, 4.3 )
  lu.assertError( vpixels.gif, 2, 3, 4, 5.4 )

  -- color resolution not supported
  lu.assertError( vpixels.gif, 1, 3, 4 )
  lu.assertError( vpixels.gif, 9, 3, 4 )
  lu.assertError( vpixels.gif, 1, 3, 4, 2 )
  lu.assertError( vpixels.gif, 9, 3, 4, 2 )
  lu.assertError( vpixels.gif, 1, 3, 4, 2, true )
  lu.assertError( vpixels.gif, 9, 3, 4, 2, true )

  -- not boolean
  lu.assertError( vpixels.gif, 2, 3, 4, 2, 1 )
  lu.assertError( vpixels.gif, 2, 3, 4, 2, 'true' )
  lu.assertError( vpixels.gif, 2, 3, 4, 2, 'false' )
end

function TestGif:testClone()
  local gif1 = vpixels.gif( 4, 5, 6, 7 )
  gif1[0]:setall(9)

  local gif2 = gif1:clone()
  lu.assertEquals( gif2:bpp(), 4 )
  lu.assertEquals( gif2:bitsperpixel(), 4 )
  local w, h = gif2:dimension()
  lu.assertEquals( w, 5 )
  lu.assertEquals( h, 6 )
  lu.assertEquals( gif2:width(), 5 )
  lu.assertEquals( gif2:height(), 6 )
  lu.assertTrue( gif2:colortable() )
  lu.assertFalse( gif2:colorsorted() )
  lu.assertEquals( gif2:colortablesize(), 16 )
  lu.assertEquals( gif2:images(), 7 )
  lu.assertEquals( gif2[0]:getpixel( 0, 0 ), 9 )
end

function TestGif:testImport()
  local gif = vpixels.gif( 3, 4, 5, 2 )
  local img0 = gif[0]
  local img1 = gif[1]

  -- import non-existing file
  lu.assertError( gif.import, gif, "not-exist.gif" ) -- not exit
  -- gif unchanged
  lu.assertEquals( gif:bpp(), 3 )
  lu.assertEquals( gif:width(), 4 )
  lu.assertEquals( gif:height(), 5 )
  lu.assertEquals( #gif, 2 )
  lu.assertEquals( gif:colortablesize(), 8 )
  -- img0 and img1 are still valid
  lu.assertEquals( img0:bpp(), 3 )
  lu.assertEquals( img1:bpp(), 3 )

  -- import() expects a string argument
  lu.assertError( gif.import, gif, 2.5 )
  lu.assertError( gif.import, gif, {} )
  -- gif unchanged
  lu.assertEquals( gif:bpp(), 3 )
  lu.assertEquals( gif:width(), 4 )
  lu.assertEquals( gif:height(), 5 )
  lu.assertEquals( #gif, 2 )
  lu.assertEquals( gif:colortablesize(), 8 )
  -- img0 and img1 are still valid
  lu.assertEquals( img0:bpp(), 3 )
  lu.assertEquals( img1:bpp(), 3 )

  -- import non-gif file or corrupted gif file
  lu.assertError( gif.import, gif, "GifTest.lua" )  -- not a GIF file
  -- gif becomes invalid, results of calling its methods are undefined
  -- img0 and img1 become invalid too, calling their methods result in errors
  lu.assertNotIsNil( img0 )
  lu.assertNotIsNil( img1 )
  lu.assertError( img0.bpp, img0 )
  lu.assertError( img1.bpp, img1 )
end

function TestGif:testExport()
  local gif = vpixels.gif()
  gif:export( "temp.gif", true )
  lu.assertError( gif.export, gif, "temp.gif" )
  lu.assertError( gif.export, gif, "temp.gif", false )
end

function TestGif:testColorTableSize()
  local gif = vpixels.gif( 3, 3, 4, 3 )
  lu.assertTrue( gif:colortable() )
  lu.assertFalse( gif:colorsorted() )
  lu.assertEquals( gif:colortablesize(), 8 )
  lu.assertEquals( gif:bpp(), 3 )

  local img0 = gif[0]
  local img1 = gif[1]
  local img2 = gif[2]
  lu.assertEquals( img0:colortablesize(), 0 )
  lu.assertEquals( img1:colortablesize(), 0 )
  lu.assertEquals( img2:colortablesize(), 0 )
  lu.assertEquals( img0:bpp(), 3 )
  lu.assertEquals( img1:bpp(), 3 )
  lu.assertEquals( img2:bpp(), 3 )

  -- img1 enables local color table
  img1:colortablesize(16)
  lu.assertTrue( img1:colortable() )
  lu.assertEquals( img1:colortablesize(), 16 )
  lu.assertEquals( img1:bpp(), 4 )

  -- disable color table
  gif:colortablesize( 0 )
  lu.assertFalse( gif:colortable() )
  lu.assertEquals( gif:colortablesize(), 0 )
  lu.assertEquals( gif:bpp(), 2 )

  lu.assertFalse( img0:colortable() )
  lu.assertTrue( img1:colortable() )
  lu.assertFalse( img2:colortable() )
  lu.assertEquals( img0:colortablesize(), 0 )
  lu.assertEquals( img1:colortablesize(), 16 )
  lu.assertEquals( img2:colortablesize(), 0 )
  lu.assertEquals( img0:bpp(), 2 )
  lu.assertEquals( img1:bpp(), 4 )
  lu.assertEquals( img2:bpp(), 2 )

  lu.assertError( gif.setcolor, gif, 0, 21, 22, 23 )
  lu.assertError( img0.setpixel, img0, 0, 0, 0 )
  img1:setpixel( 0, 0, 0 )
  lu.assertError( img2.setpixel, img0, 0, 0, 0 )

  -- enable color table again
  gif:colortablesize( 2 )
  lu.assertTrue( gif:colortable() )
  lu.assertEquals( gif:colortablesize(), 2 )
  lu.assertEquals( gif:bpp(), 2 )

  lu.assertFalse( img0:colortable() )
  lu.assertTrue( img1:colortable() )
  lu.assertFalse( img2:colortable() )
  lu.assertEquals( img0:colortablesize(), 0 )
  lu.assertEquals( img1:colortablesize(), 16 )
  lu.assertEquals( img2:colortablesize(), 0 )
  lu.assertEquals( img0:bpp(), 2 )
  lu.assertEquals( img1:bpp(), 4 )
  lu.assertEquals( img2:bpp(), 2 )

  -- increase to maximum size
  gif:colortablesize( 256 )
  lu.assertTrue( gif:colortable() )
  lu.assertEquals( gif:colortablesize(), 256 )
  lu.assertEquals( gif:bpp(), 8 )

  lu.assertFalse( img0:colortable() )
  lu.assertTrue( img1:colortable() )
  lu.assertFalse( img2:colortable() )
  lu.assertEquals( img0:colortablesize(), 0 )
  lu.assertEquals( img1:colortablesize(), 16 )
  lu.assertEquals( img2:colortablesize(), 0 )
  lu.assertEquals( img0:bpp(), 8 )
  lu.assertEquals( img1:bpp(), 4 )
  lu.assertEquals( img2:bpp(), 8 )

  -- size out of range
  lu.assertError( gif.colortablesize, gif, 257 )
  lu.assertError( gif.colortablesize, gif, -2 )

  lu.assertFalse( img0:colortable() )
  lu.assertTrue( img1:colortable() )
  lu.assertFalse( img2:colortable() )
  lu.assertEquals( img0:colortablesize(), 0 )
  lu.assertEquals( img1:colortablesize(), 16 )
  lu.assertEquals( img2:colortablesize(), 0 )
  lu.assertEquals( img0:bpp(), 8 )
  lu.assertEquals( img1:bpp(), 4 )
  lu.assertEquals( img2:bpp(), 8 )
end

function TestGif:testColorTable()
  local gif = vpixels.gif( 2, 3, 4 )
  lu.assertTrue( gif:colortable() )
  gif:setcolortable( 0, 255, 26, 27 )
  gif:setcolor( 1, 28, 29, 30 )
  lu.assertEquals( gif:getcolortable( 0 ), 255, 26, 27 )
  lu.assertEquals( gif:getcolor( 1 ), 28, 29, 30 )

  lu.assertError( gif.setcolortable, gif, 4, 25, 26, 27 )
  lu.assertError( gif.setcolortable, gif, -1, 25, 26, 27 )
  lu.assertError( gif.setcolortable, gif, 1, -25, 26, 27 )
  lu.assertError( gif.setcolortable, gif, 1, 25, -26, 27 )
  lu.assertError( gif.setcolortable, gif, 1, 25, 26, -27 )

  lu.assertError( gif.setcolor, gif, 256, 25, 26, 27 )
  lu.assertError( gif.setcolor, gif, 0, 256, 26, 27 )
  lu.assertError( gif.setcolor, gif, 0, 25, 256, 27 )
  lu.assertError( gif.setcolor, gif, 0, 25, 26, 257 )

  lu.assertError( gif.getcolortable, gif, 4 )
  lu.assertError( gif.getcolor, gif, -1 )

  -- no global color table
  local gif2 = vpixels.gif( 2, 3, 4, 1, false )
  lu.assertFalse( gif2:colortable() )
  lu.assertError( gif2.setcolortable, gif2, 0, 25, 26, 27 )
  lu.assertError( gif2.getcolortable, gif2, 0 )
end

function TestGif:testBackgroundColor()
  local gif1 = vpixels.gif( 2, 3, 4, 5 )
  lu.assertEquals( gif1:backgroundcolor(), 0 )
  gif1:backgroundcolor(1)
  lu.assertEquals( gif1:backgroundcolor(), 1 )
  gif1:backgroundcolor(3)
  lu.assertEquals( gif1:backgroundcolor(), 3 )
  lu.assertError( gif1.backgroundcolor, gif1, 4 )  -- exceeds
  gif1:colortablesize(0) -- disable
  lu.assertEquals( gif1:colortable(), false )
  lu.assertEquals( gif1:backgroundcolor(), 0 )
  lu.assertError( gif1.backgroundcolor, gif1, 0 )  -- not allowed

  local gif2 = vpixels.gif( 3, 4, 5, 3, false )
  lu.assertEquals( gif2:background(), 0 )
  lu.assertEquals( gif2:colortable(), false )
  gif2:colortablesize(4) -- enable
  lu.assertEquals( gif2:colortable(), true )
  lu.assertEquals( gif2:background(), 0 )
  gif2:backgroundcolor(3)
  lu.assertEquals( gif2:background(), 3 )
  lu.assertError( gif2.backgroundcolor, gif2, 4 )  -- exceeds
end

function TestGif:testRemoveImage()
  local gif = vpixels.gif( 2, 3, 4, 7 )
  lu.assertEquals( #gif, 7 )

  img0 = gif[0]
  lu.assertEquals( img0:bitsperpixel(), 2 )
  lu.assertIsTrue( gif:removeimage(0) )     -- remove 1st image
  lu.assertError( img0.bitsperpixel, img0 ) -- img0 becomes invalid
  lu.assertEquals( #gif, 6 )

  img5 = gif[5]
  lu.assertEquals( img5:bitsperpixel(), 2 )
  lu.assertIsTrue( gif:removeimage(5) )     -- remove last one
  lu.assertError( img5.bitsperpixel, img5 ) -- img5 becomes invalid
  lu.assertEquals( #gif, 5 )

  img2 = gif[2]
  lu.assertEquals( img2:bitsperpixel(), 2 )
  lu.assertIsTrue( gif:removeimage(2) )     -- remove middle one
  lu.assertError( img2.bitsperpixel, img2 ) -- img2 becomes invalid
  lu.assertEquals( #gif, 4 )

  -- error cases
  lu.assertError( gif.removeimage, gif, -1 )  -- out of range
  lu.assertError( gif.removeimage, gif, 4 )   -- out of range
  lu.assertError( gif.removeimage, gif, 1.5 ) -- not an integer

  -- remove images until only one left
  while #gif > 1 do
    lu.assertIsTrue( gif:remove(0) )
  end

  -- now gif contains only one image
  lu.assertEquals( #gif, 1 )
  lu.assertError( gif.removeimage, gif, 0 ) -- cannot remove the image
  local img = gif[0]
  lu.assertEquals( img:delay(), 0 )
  lu.assertError( img.delay, 100 )
  lu.assertEquals( img:disposalmethod(), 0 )
  lu.assertError( img.disposalmethod, 1 )
  lu.assertIsFalse( img:hastranscolor() )
  lu.assertError( img.hastranscolor, true )
  lu.assertError( img.transcolor )
  lu.assertError( img.transcolor, 1 )

  -- single image object
  local gif2 = vpixels.gif( 2, 3, 4, 1 )
  lu.assertError( gif2.removeimage, gif2, 0 ) -- cannot remove the image
end

function TestGif:testIndexing()
  local gif = vpixels.gif( 2, 3, 4, 5 )
  local findex = getmetatable(gif).__index
  local ret = nil

  -- arg is number
  ret = findex( gif, 0 )
  lu.assertEquals( type(ret), 'userdata' )
  lu.assertStrContains( tostring(ret), 'gifimage' )
  ret = findex( gif, 4 )
  lu.assertEquals( type(ret), 'userdata' )
  lu.assertStrContains( tostring(ret), 'gifimage' )

  lu.assertError( findex, gif, -1 )
  lu.assertError( findex, gif, 5 )

  -- arg is string
  ret = findex( gif, 'bpp' )
  lu.assertEquals( type(ret), 'function' )
  ret = findex( gif, 'dimension' )
  lu.assertEquals( type(ret), 'function' )

  lu.assertError( findex, gif, '0' )
  lu.assertError( findex, gif, 'not-exist' )

  -- arg is neither number nor string
  lu.assertError( findex, gif, ret )
  lu.assertError( findex, gif, gif )
  lu.assertError( findex, gif, {} )
end

function TestGif:testNewIndex()
  local gif1 = vpixels.gif( 2, 3, 4, 5 )
  local fnewindex = getmetatable(gif1).__newindex
  local findex = getmetatable(gif1).__index

  -- "base" is reserved, manipulating it is not allowed
  lu.assertError( fnewindex, gif1, "base", 3.14 )  -- gif1.base = 3.14
  lu.assertError( fnewindex, gif1, "base", "string" ) -- gif1.base = "string"
  lu.assertError( fnewindex, gif1, "base", {} )  -- gif1.base = {}
  lu.assertError( fnewindex, gif1, "base", function() end ) -- gif1.base = function() end

  -- add variables to gif1
  gif1.number = 0  -- number
  lu.assertEquals( gif1.number, 0 )

  gif1.name = "a name"  -- string
  lu.assertEquals( gif1.name, "a name" )

  gif1.consts = { 0.0, 0.0 }  -- table
  lu.assertEquals( gif1.consts[1], 0.0 )
  lu.assertEquals( gif1.consts[2], 0.0 )

  -- change variables
  gif1.number = 1
  lu.assertEquals( gif1.number, 1 )

  gif1.name = "a vp.gif"  -- string
  lu.assertEquals( gif1.name, "a vp.gif" )

  gif1.consts[1] = 0.01
  gif1.consts[2] = 0.02
  lu.assertEquals( gif1.consts[1], 0.01 )
  lu.assertEquals( gif1.consts[2], 0.02 )

  -- add methods to gif1
  function gif1:getnumber() return gif1.number end
  lu.assertEquals( gif1:getnumber(), 1 )

  function gif1:getname() return gif1.name end
  lu.assertEquals( gif1:getname(), "a vp.gif" )

  function gif1:getconst(i) return gif1.consts[i] end
  lu.assertEquals( gif1:getconst(1), 0.01 )
  lu.assertEquals( gif1:getconst(2), 0.02 )
  lu.assertEquals( gif1:getconst(3), nil )

  function gif1:getbpp() return gif1:bpp() end  -- call method in metatable
  lu.assertEquals( gif1:getbpp(), 2 )

  -- redefine methods
  function gif1:getnumber() return gif1.name end
  lu.assertEquals( gif1:getnumber(), "a vp.gif" )

  function gif1:getname() return gif1.number end
  lu.assertEquals( gif1:getname(), 1 )

  function gif1:getbpp() return #gif1 end
  lu.assertEquals( gif1:getbpp(), 5 )

  -- override a method defined in metatable of vp.gif
  function gif1:bpp() return 0 end
  lu.assertEquals( gif1:bpp(), 0 )

  -- using 'base' key to access the method defined in metatable of vp.gif
  lu.assertEquals( gif1.base:bpp(), 2 )
  lu.assertEquals( gif1.base.bpp(gif1), 2 )

  -- the following are syntactically correct, but treated as error
  --   gif1.base.base:bpp() or gif1.base.base.bpp(gif1)
  -- (see two consecutive calls to __index() with key 'base' )

  -- modifications affect gif1 only
  local gif2 = vpixels.gif( 3, 4, 5, 6 )
  lu.assertError( findex, gif2, 'number' )  -- gif2.number
  lu.assertError( findex, gif2, 'name' )    -- gif2.name
  lu.assertError( findex, gif2, 'consts' )  -- gif2.consts
  lu.assertError( findex, gif2, 'getnumber' )  -- gif2.getnumber
  lu.assertError( findex, gif2, 'getname' )    -- gif2.getname
  lu.assertError( findex, gif2, 'getconst' )   -- gif2.getconst
  lu.assertError( findex, gif2, 'getbpp' )     -- gif2.getbpp

  -- delete an image: 3rd arg is nil
  lu.assertEquals( #gif2, 6 )
  img0 = gif2[0]
  lu.assertEquals( img0:bpp(), 3 )
  gif2[0] = nil                    -- delete image #0
  lu.assertError( img0.bpp, img0 ) -- img0 becomes invalid
  lu.assertEquals( #gif2, 5 )      -- 5 images left
  img3 = gif2[3]
  lu.assertEquals( img3:bpp(), 3 )
  gif2[3] = nil                    -- delete image #4
  lu.assertError( img3.bpp, img3 ) -- img0 becomes invalid
  lu.assertEquals( #gif2, 4 )      -- 4 images left
  -- wrong index
  lu.assertError( fnewindex, gif2, -1, nil )
  lu.assertError( fnewindex, gif2, 4, nil )

  -- copy an image: 3rd arg is a vp.gifimage
  gif2[1]:setall(3)
  lu.assertEquals( gif2[0]:getpixel(0, 0), 0 )
  gif2[0] = gif2[1]
  lu.assertEquals( gif2[0]:getpixel(0, 0), 3 )
  -- wrong index
  lu.assertError( fnewindex, gif2, -1, gif2[1] )
  lu.assertError( fnewindex, gif2, 4, gif2[1] )
  -- 3rd arg is a vp.gifimage, but belongs to an incompatible a vp.gif
  lu.assertError( fnewindex, gif2, 0, gif1[1] )
  -- 3rd arg is neither a vp.gifimage nor a nil
  lu.assertError( fnewindex, gif2, 0, 2.718 )
  lu.assertError( fnewindex, gif2, 0, '3.14' )
  lu.assertError( fnewindex, gif2, 0, {} )
end

function TestGif:testIPairs()
  local gif = vpixels.gif( 2, 3, 3, 3 )

  -- vp.gif has meta method __ipairs
  lu.assertEquals( type(getmetatable(gif).__ipairs), 'function' )

  -- ipairs() returns a iterator, a vp.fig and a control variable
  local iter, state, var = ipairs( gif )
  lu.assertEquals( type(iter), "function" )
  lu.assertEquals( type(state), "userdata" )
  lu.assertStrContains( tostring(state), 'gif' )
  lu.assertEquals( var, -1 )

  -- this is what a for-loop does
  local i, img = iter( state, var )
  lu.assertEquals( i, 0 )
  lu.assertEquals( type(img), "userdata" )
  lu.assertStrContains( tostring(img), 'gifimage' )

  i, img = iter( state, i )
  lu.assertEquals( i, 1 )
  lu.assertEquals( type(img), "userdata" )
  lu.assertStrContains( tostring(img), 'gifimage' )

  i, img = iter( state, i )
  lu.assertEquals( i, 2 )
  lu.assertEquals( type(img), "userdata" )
  lu.assertStrContains( tostring(img), 'gifimage' )

  i, img = iter( state, i )
  lu.assertEquals( i, nil )  -- iteration ends
  lu.assertEquals( img, nil )

  -- verify the correctness of iteration
  for i, img in ipairs(gif) do
    lu.assertEquals( img:getpixel(0, 0), 0 )
  end

  for i = 0, #gif - 1 do
    gif[i]:setall(i + 1)
  end

  for i, img in ipairs(gif) do
    lu.assertEquals( img:getpixel(0, 0), i + 1 )
  end

  -- error cases
  getmetatable(gif).__ipairs = ""   -- replace __ipairs with a string
  lu.assertError( ipairs, gif )
  getmetatable(gif).__ipairs = nil  -- __ipairs removed
  lu.assertError( ipairs, gif )
end

function TestGif:testGC()
  local gif = vpixels.gif( 2, 3, 4 )
  local img = gif[0]
  lu.assertEquals( gif:bpp(), 2 )
  lu.assertEquals( img:bpp(), 2 )

  -- if directly call __gc(), the C++ Gif object gets deleted.
  -- test this situation is correctly handled, i.e. error message
  -- notifies this situation.
  gif:__gc()
  lu.assertError( gif.bpp, gif ) -- test LuaGif::CheckGif()
  lu.assertError( img.bpp, img ) -- test LuaGifImage::CheckImage()

  -- calling __gc() again is harmless
  gif:__gc()
end

function TestGif:testMetatable()
  local gif = vpixels.gif()

  -- allow access to metatable
  local mt = getmetatable(gif)
  lu.assertEquals( "table", type(mt) )

  -- metatable has its string representation
  lu.assertStrContains( tostring(mt), 'gif' )

  -- metatable has its own metatable
  lu.assertEquals( type(getmetatable(mt)), "table" )

  -- allow modifying metatable
  mt.getzero = function() return 0 end
  lu.assertEquals( 0, gif:getzero() )

  -- modification affects all vp.gif objects
  local gif2 = vpixels.gif(2, 3, 4, 5)
  lu.assertEquals( 0, gif2:getzero() )

  -- changing metatable is not allowed
  lu.assertError( setmetatable, gif, {} )
end


TestGifImage = {}

function TestGifImage:testOutOfScope()
  local gif = vpixels.gif( 2, 3, 4, 5 )
  local img0 = gif[0]
  local img4 = gif[4]
  lu.assertEquals( img0:bpp(), 2 )
  lu.assertEquals( img4:bpp(), 2 )

  -- assign to nil, gif out of scope
  gif = nil
  collectgarbage( "collect" )
  lu.assertNotIsNil( img0 )
  lu.assertNotIsNil( img4 )
  lu.assertError( img0.bpp, img0 ) -- cannot call img0:bpp()
  lu.assertError( img4.bpp, img4 ) -- cannot call img4:bpp()

  -- assign to a new object
  gif = vpixels.gif( 3, 3, 5, 5 )
  lu.assertError( img0.bpp, img0 ) -- still cannot call img0:bpp()
  lu.assertError( img4.bpp, img4 ) -- still cannot call img4:bpp()

  -- assign img0 and img4
  img0 = gif[0];
  img4 = gif[4];
  lu.assertEquals( img0:bpp(), 3 )
  lu.assertEquals( img4:bpp(), 3 )

  --  assign a new object to gif, previous gif object out of scope
  gif = vpixels.gif( 4, 3, 5 )
  collectgarbage( "collect" )
  lu.assertNotIsNil( img0 )
  lu.assertNotIsNil( img4 )
  lu.assertError( img0.bpp, img0 ) -- cannot call img0:bpp()
  lu.assertError( img4.bpp, img4 ) -- cannot call img4:bpp()

  -- clone() will fail, b/c img4 is invalid
  img0 = gif[0];
  lu.assertError( img0.clone, img0, img4 ) -- img0:clone(img4)
  lu.assertError( img4.clone, img4, img0 ) -- img4:clone(img0)

  -- directly call __gc, gif object is out of scope
  gif:__gc()
  lu.assertError( img0.bpp, img0 ) -- img0:bpp()
end

function TestGifImage:testMethodsNoArg()
  local gif = vpixels.gif( 2, 3, 4, 5 )
  local img = gif[0]
  lu.assertEquals( img:bitsperpixel(), 2 )
  lu.assertEquals( img:bpp(), 2 )
  lu.assertEquals( img:left(), 0 )
  lu.assertEquals( img:top(), 0 )
  lu.assertEquals( img:width(), 3 )
  lu.assertEquals( img:height(), 4 )
  local w, h = img:dimension()
  lu.assertEquals( w, 3 )
  lu.assertEquals( h, 4 )
  lu.assertFalse( img:interlaced() )
  lu.assertFalse( img:colortable() )
  lu.assertFalse( img:colortablesorted() )
end

function TestGifImage:testClone()
  local gif = vpixels.gif( 2, 3, 4, 5 )
  local img0 = gif[0]
  local img1 = gif[1]

  lu.assertEquals( img1:getpixel( 1, 1 ), 0 )
  img0:setall( 3 )
  img1:clone( img0 )
  lu.assertEquals( img1:getpixel( 1, 1 ), 3 )

  -- wrong arg
  lu.assertError( img1.clone, img1, 3 )  -- img1:clone( 3 )
  lu.assertError( img1.clone, img1, {} ) -- img1:clone( {} )

  -- images belong to compatible gif objects
  local gif2 = vpixels.gif( 2, 3, 4, 2 )
  local img2 = gif2[0]
  img2:clone( img0 )
  lu.assertEquals( img2:getpixel( 1, 1 ), 3 )

  -- images belong to incompatible gif objects
  --local gif3 = vpixels.gif( 3, 3, 4, 2 )  -- different bpp
  --local img3 = gif3[0]
  --lu.assertError( img3.clone, img3, img0 )

  local gif4 = vpixels.gif( 2, 4, 4, 2 )  -- different width
  local img4 = gif4[0]
  lu.assertError( img4.clone, img4, img0 )

  local gif5 = vpixels.gif( 2, 3, 3, 2 )  -- different width
  local img5 = gif5[0]
  lu.assertError( img5.clone, img5, img0 )
end

function TestGifImage:testBitsPerpixel()
  local gif = vpixels.gif( 2, 3, 4, 3 )
  local img0 = gif[0]
  local img1 = gif[1]
  local img2 = gif[2]
  lu.assertEquals( gif:bitsperpixel(), 2 )
  lu.assertEquals( gif:colortablesize(), 4 )
  lu.assertEquals( img0:bitsperpixel(), 2 )
  lu.assertEquals( img1:bitsperpixel(), 2 )
  lu.assertEquals( img2:bitsperpixel(), 2 )

  -- change bpp
  gif:bitsperpixel( 7 )
  lu.assertEquals( gif:bitsperpixel(), 7 )
  lu.assertEquals( gif:colortablesize(), 128 )
  lu.assertEquals( img0:bitsperpixel(), 7 )
  lu.assertEquals( img1:bitsperpixel(), 7 )
  lu.assertEquals( img2:bitsperpixel(), 7 )

  -- change bpp of images to smaller value
  img0:bitsperpixel( 3 )
  img1:bitsperpixel( 4 )
  img2:bitsperpixel( 5 )
  lu.assertEquals( gif:bitsperpixel(), 7 )
  lu.assertEquals( gif:colortablesize(), 128 )
  lu.assertEquals( img0:bitsperpixel(), 3 )
  lu.assertEquals( img1:bitsperpixel(), 4 )
  lu.assertEquals( img2:bitsperpixel(), 5 )

  -- change bpp of images to larger value
  lu.assertError( img0.bitsperpixel, img0, 8 )
  lu.assertError( img1.bitsperpixel, img1, 8 )
  lu.assertError( img1.bitsperpixel, img1, 8 )

  -- enable local color table of img1
  img1:colortablesize( 32 )
  lu.assertEquals( img1:colortablesize(), 32 )
  lu.assertEquals( img1:bitsperpixel(), 5 )
  lu.assertEquals( gif:bitsperpixel(), 7 )
  lu.assertEquals( gif:colortablesize(), 128 )
  lu.assertEquals( img0:bitsperpixel(), 3 )
  lu.assertEquals( img2:bitsperpixel(), 5 )

  -- change bpp of img1
  img1:bitsperpixel( 8 )
  lu.assertEquals( img1:bitsperpixel(), 8 )
  lu.assertEquals( img1:colortablesize(), 256 )
  lu.assertEquals( gif:bitsperpixel(), 7 )
  lu.assertEquals( gif:colortablesize(), 128 )
  lu.assertEquals( img0:bitsperpixel(), 3 )
  lu.assertEquals( img2:bitsperpixel(), 5 )

  -- bpp out of range
  lu.assertError( gif.bitsperpixel, gif, 1 )
  lu.assertError( gif.bitsperpixel, gif, 9 )
  lu.assertError( img0.bitsperpixel, img0, 1 )
  lu.assertError( img0.bitsperpixel, img0, 9 )
  lu.assertError( img1.bitsperpixel, img1, 1 )
  lu.assertError( img1.bitsperpixel, img1, 9 )
  lu.assertError( img2.bitsperpixel, img2, 1 )
  lu.assertError( img2.bitsperpixel, img2, 9 )
end

function TestGifImage:testPixels()
  local gif = vpixels.gif(2, 3, 4, 5)

  local img = gif[0]
  lu.assertEquals( img:getpixel(0, 0), 0 )
  img:setallpixels( 3 )
  lu.assertEquals( img:getpixel(0, 0), 3 )
  img:setpixel( 1, 1, 2 )
  lu.assertEquals( img:getpixel(1, 1), 2 )
  lu.assertFalse( img:transparent(1, 1) )
  img:transcolor( 2 )
  lu.assertTrue( img:transparent(1, 1) )
  lu.assertFalse( img:transparent(0, 0) )
  img:transcolor( 3 )
  lu.assertFalse( img:transparent(1, 1) )
  lu.assertTrue( img:transparent(0, 0) )

  -- wrong args
  lu.assertError( img.setall, img, -1 )
  lu.assertError( img.setall, img, 4 )
  lu.assertError( img.setpixel, img, 1, 1, -1 )
  lu.assertError( img.setpixel, img, 1, 1, 4 )
  lu.assertError( img.setpixel, img, -1, 1, 2 )
  lu.assertError( img.setpixel, img, 3, 1, 2 )

  lu.assertError( img.getpixel, img, -1, 1 )
  lu.assertError( img.getpixel, img, 1, -1 )
  lu.assertError( img.getpixel, img, 3, 1 )
  lu.assertError( img.getpixel, img, 1, 4 )

  lu.assertError( img.transparent, img, -1, 1 )
  lu.assertError( img.transparent, img, 1, -1 )
  lu.assertError( img.trans, img, 3, 1 )
  lu.assertError( img.trans, img, 1, 4 )
end

function TestGifImage:testCrop()
  local gif = vpixels.gif( 2, 8, 9, 2 )

  local img = gif[0]
  lu.assertEquals( 0, img:left() )
  lu.assertEquals( 0, img:top() )
  lu.assertEquals( 8, img:width() )
  lu.assertEquals( 9, img:height() )

  img:crop( 2, 3, 4, 5 )
  lu.assertEquals( 2, img:left() )
  lu.assertEquals( 3, img:top() )
  lu.assertEquals( 4, img:width() )
  lu.assertEquals( 5, img:height() )

  img:crop( 3, 4, 3, 4 )
  lu.assertEquals( 3, img:left() )
  lu.assertEquals( 4, img:top() )
  lu.assertEquals( 3, img:width() )
  lu.assertEquals( 4, img:height() )

  -- wrong number of args
  img = gif[1]
  lu.assertError( img.crop, img, 2 )
  lu.assertError( img.crop, img, 2, 3 )
  lu.assertError( img.crop, img, 2, 3, 4 )

  -- negative numbers
  lu.assertError( img.crop, img, -2, 3, 4, 5 )
  lu.assertError( img.crop, img, 2, -3, 4, 5 )
  lu.assertError( img.crop, img, 2, 3, -4, 5 )
  lu.assertError( img.crop, img, 2, 3, 4, -5 )

  -- arg exceeds limit
  lu.assertError( img.crop, img, 8, 0, 4, 5 ) -- arg #1 exceeds 
  lu.assertError( img.crop, img, 0, 9, 4, 5 ) -- arg #2 exceeds
  lu.assertError( img.crop, img, 2, 3, 0, 5 ) -- arg #3 == 0
  lu.assertError( img.crop, img, 2, 3, 4, 0 ) -- arg #4 == 0
  lu.assertError( img.crop, img, 2, 3, 7, 5 ) -- arg #3 exceeds 
  lu.assertError( img.crop, img, 2, 3, 4, 7 ) -- arg #4 exceeds 
end

function TestGifImage:testDelay()
  local gif = vpixels.gif( 2, 3, 4, 5 )
  local img = gif[0]
  lu.assertEquals( img:delay(), 0 )
  img:delay( 100 )
  lu.assertEquals( img:delay(), 100 )

  -- negative number
  lu.assertError( img.delay, img, -100 )

  -- gif2 contains only one image, setting delay time is not allowed
  local gif2 = vpixels.gif( 2, 3, 4, 1 )
  img = gif2[0]
  lu.assertEquals( img:delay(), 0 )
  lu.assertError( img.delay, img, 100 )
end

function TestGifImage:testDisposalMethod()
  local gif1 = vpixels.gif( 2, 3, 4, 5 )
  local img = gif1[0]
  lu.assertEquals( img:disposalmethod(), 1 )
  img:disposalmethod(2)
  lu.assertEquals( img:disposalmethod(), 2 )
  img:disposal(3)
  lu.assertEquals( img:disposal(), 3 )
  img:disposal(0)
  lu.assertEquals( img:disposal(), 0 )
  -- value > 3
  lu.assertError( img.disposalmethod, img, 4 )
  -- value is not unsigned byte
  lu.assertError( img.disposalmethod, img, -1 )
  lu.assertError( img.disposalmethod, 256 )

  -- gif2 contains only one image, setting disposal method is not allowed
  local gif2 = vpixels.gif( 2, 3, 4, 1 )
  img = gif2[0]
  lu.assertEquals( img:disposalmethod(), 0 )
  lu.assertError( img.disposalmethod, img, 256 )  
end

function TestGifImage:testTransparentColor()
  local gif1 = vpixels.gif( 2, 3, 4, 5 )
  local img = gif1[0]
  -- default
  lu.assertEquals( img:hastransparentcolor(), false )
  lu.assertEquals( img:transparentcolor(), 0 )
  -- turn on
  img:transparentcolor( 3 )
  lu.assertEquals( img:hastransparentcolor(), true )
  lu.assertEquals( img:transparentcolor(), 3 )
  -- turn off
  img:hastransparentcolor( false )
  lu.assertEquals( img:hastransparentcolor(), false )
  lu.assertEquals( img:transparentcolor(), 0 )
  -- turn on again
  img:hastransparentcolor( true )
  lu.assertEquals( img:hastranscolor(), true )
  lu.assertEquals( img:transcolor(), 0 )
  -- error cases
  lu.assertError( img.hastransparentcolor, img, 1 ) -- not boolean
  lu.assertError( img.transparentcolor, img, 4 )   -- exceed color table entry
  lu.assertError( img.transparentcolor, img, -1 )  -- exceed unsigned byte
  lu.assertError( img.transparentcolor, img, 256 ) -- exceed unsigned byte

  --gif2 contains only one image, setting transparent color is not allowed
  local gif2 = vpixels.gif( 2, 3, 4, 1 )
  img = gif2[0]
  lu.assertFalse( img:hastransparentcolor() )
  lu.assertError( img.hastransparentcolor, img, true )
  lu.assertError( img.transparentcolor, img )
  lu.assertError( img.transparentcolor, img, 2 )
end

function TestGifImage:testColorTable()
  local gif = vpixels.gif( 3, 4, 5 )
  local img = gif[0]

  -- no color table by default
  lu.assertEquals( img:bpp(), gif:bpp() )
  lu.assertFalse( img:colortable() )
  lu.assertFalse( img:colortablesorted() )
  lu.assertEquals( img:colortablesize(), 0 )

  -- enable color table
  img:colortablesize( 2 )
  lu.assertEquals( img:bpp(), 2 )  -- changed
  lu.assertTrue( img:colortable() )
  lu.assertFalse( img:colortablesorted() )
  lu.assertEquals( img:colortablesize(), 2 )
  local r, g, b = img:getcolortable( 0 )
  lu.assertEquals( r, 255 )
  lu.assertEquals( g, 255 )
  lu.assertEquals( b, 255 )
  r, g, b = img:getcolortable( 1 )
  lu.assertEquals( r, 255 )
  lu.assertEquals( g, 255 )
  lu.assertEquals( b, 255 )
  img:setcolortable( 1, 25, 26, 27 )
  r, g, b = img:getcolortable( 1 )
  lu.assertEquals( r, 25 )
  lu.assertEquals( g, 26 )
  lu.assertEquals( b, 27 )

  -- increase to maximum size for bpp = 2
  img:colortablesize( 4 )
  lu.assertEquals( img:bpp(), 2 )  -- not changed
  lu.assertTrue( img:colortable() )
  lu.assertFalse( img:colortablesorted() )
  lu.assertEquals( img:colortablesize(), 4 )

  -- increase to maximum size 256
  img:colortablesize( 256 )
  lu.assertEquals( img:bpp(), 8 )  -- changed
  lu.assertTrue( img:colortable() )
  lu.assertFalse( img:colortablesorted() )
  lu.assertEquals( img:colortablesize(), 256 )

  -- disable color table
  img:colortablesize( 0 )
  lu.assertEquals( img:bpp(), gif:bpp() )  -- changed back
  lu.assertFalse( img:colortable() )
  lu.assertFalse( img:colortablesorted() )
  lu.assertEquals( img:colortablesize(), 0 )

  -- bad args
  lu.assertError( img.colortablesize, img, -1 )
  lu.assertError( img.colortablesize, img, 257 )  -- exceed max 256

  lu.assertError( img.getcolortable, img, -1 )
  lu.assertError( img.getcolortable, img, 4 )

  lu.assertError( img.setcolortable, img, -1, 25, 26, 27 )
  lu.assertError( img.setcolortable, img, 1, -25, 26, 27 )
  lu.assertError( img.setcolortable, img, 1, 25, -26, 27 )
  lu.assertError( img.setcolortable, img, 1, 25, 26, -27 )
  lu.assertError( img.setcolortable, img, 1, 256, 26, 27 )
  lu.assertError( img.setcolortable, img, 1, 25, 256, 27 )
  lu.assertError( img.setcolortable, img, 1, 25, 26, 256 )
end

function TestGifImage:testCompare()
  local gif = vpixels.gif( 2, 8, 5, 2 )
  for i = 0, 3 do
    gif:setcolor( i, i, i, i )
  end

  local img0 = gif[0]
  local img1 = gif[1]

  -- same object
  lu.assertTrue( img0 == img0 )

  -- different objects
  lu.assertTrue( img0 == img1 )
  lu.assertTrue( img1 == img0 )

  -- set to two different colors
  img0:setpixel( 3, 2, 1 )
  img1:setpixel( 3, 2, 2 )
  lu.assertTrue( img0 ~= img1 )
  lu.assertTrue( img1 ~= img0 )

  -- set the two color transparent
  img0:transcolor( 1 )
  img1:transcolor( 2 )
  lu.assertTrue( img0 == img1 )
  lu.assertTrue( img1 == img0 )

  -- crop img0
  img0:crop( 0, 1, 8, 4 )
  lu.assertTrue( img0 ~= img1 )
  lu.assertTrue( img1 ~= img0 )

  -- crop img1
  img1:crop( 1, 0, 7, 5 )
  lu.assertTrue( img0 ~= img1 )
  lu.assertTrue( img1 ~= img0 )

  -- crop both
  img0:crop( 1, 1, 7, 4 )
  img1:crop( 1, 1, 7, 4 )
  lu.assertTrue( img0 == img1 )
  lu.assertTrue( img1 == img0 )

  -- compare with objects other than vp.image
  lu.assertTrue( img0 ~= nil )
  lu.assertTrue( nil ~= img0 )
  lu.assertTrue( img0 ~= 0 )
  lu.assertTrue( 0 ~= img0 )
  lu.assertTrue( img0 ~= "img0" )
  lu.assertTrue( "img0" ~= img0 )
  lu.assertTrue( img0 ~= {} )
  lu.assertTrue( {} ~= img0 )

  -- operator < not supported
  lu.assertError( img0.__lt, img0, img0 )
  lu.assertError( img0.__lt, img0, img1 )
  lu.assertError( img0.__lt, img1, img0 )
  lu.assertError( img0.__lt, img0, nil )
  lu.assertError( img0.__lt, nil, img0 )
  lu.assertError( img0.__lt, img0, 0 )
  lu.assertError( img0.__lt, 0, img0 )
  lu.assertError( img0.__lt, img0, "img0" )
  lu.assertError( img0.__lt, "img0", img0 )
  lu.assertError( img0.__lt, img0, {} )
  lu.assertError( img0.__lt, {}, img0 )
end

function TestGifImage:testGC()
  local gif = vpixels.gif( 2, 3, 4 )
  local img = gif[0]
  lu.assertEquals( img:bpp(), 2 )

  -- if directly call __gc(), pointer to the C++ GifImage object
  -- is set to nullptr. test this situation is correctly handled,
  -- i.e. error message notifies this situation.
  img:__gc()
  lu.assertError( img.bpp, img ) -- test LuaGifImage::CheckImage()

  -- calling __gc() again is harmless
  img:__gc()
end

function TestGifImage:testMetatable()
  local gif = vpixels.gif( 2, 3, 4, 5 )
  local img = gif[0]

  -- allow access to metatable
  local mt = getmetatable(img)
  lu.assertEquals( "table", type(mt) )

  -- metatable has its string representation
  lu.assertStrContains( tostring(mt), 'gifimage' )

  -- metatable has its own metatable
  lu.assertEquals( type(getmetatable(mt)), "table" )

  -- allow modifying metatable
  mt.getzero = function() return 0 end
  lu.assertEquals( 0, img:getzero() )

  -- modification affects all vp.gifimage objects
  local img1 = gif[1]
  lu.assertEquals( 0, img1:getzero() )

  -- changing metatable is not allowed
  lu.assertError( setmetatable, img, {} )
end


local runner = lu.LuaUnit.new()
--runner:setOutputType("tap")
os.exit( runner:runSuite() )
