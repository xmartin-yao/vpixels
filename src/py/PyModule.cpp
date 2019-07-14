////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 Xueyi Yao
//
// This file is part of VPixels.
//
// VPixels is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// VPixels is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#include <Python.h>
#include "PyBmp.h"
#include "PyGif.h"
#include "PyGifImage.h"
#include "PyUtil.h"
#include "config.h"
#include <iostream>

//////////////////////
namespace
{
  // version of the module
  PyObject* Version()
  {
    return PyString_FromString( PACKAGE_VERSION );
  }

  // about the module
  PyObject* About()
  {
    const std::string Notice{ PACKAGE_STRING " Copyright (C) 2019 Xueyi Yao\n"
                              "License: GNU GPL version 3 or later\n"
                              "Bug report: " PACKAGE_BUGREPORT };

    std::cout << Notice << std::endl;
    Py_RETURN_NONE;
  }

  // methods of the module
  PyMethodDef Methods[] = {
    MDef( version, Version, METH_NOARGS, "version of the module." )
    MDef( about,   About,   METH_NOARGS, "about the module." )
    { nullptr, nullptr, 0, nullptr } 
  };
}

/////////////////////////
PyMODINIT_FUNC initvpixels()
{
  // initialize types
  if( PyType_Ready( &PyBmp::Bmp_Type ) < 0 )
    return;

  if( PyType_Ready( &PyGif::Gif_Type ) < 0 )
    return;

  if( PyType_Ready( &PyGifImage::GifImage_Type ) < 0 )
    return;

  // new module
  PyObject* M = Py_InitModule( PACKAGE_NAME, Methods );
  if( M == nullptr ) return;

  Py_INCREF( &PyBmp::Bmp_Type );
  Py_INCREF( &PyGif::Gif_Type );
  Py_INCREF( &PyGifImage::GifImage_Type );

  // add types
  PyModule_AddObject( M, "bmp", reinterpret_cast<PyObject*>(&PyBmp::Bmp_Type) );
  PyModule_AddObject( M, "gif", reinterpret_cast<PyObject*>(&PyGif::Gif_Type) );

  // add strings
  PyModule_AddObject( M, "__copyright__",
                      PyString_FromFormat("%s Copyright (C) 2019 Xueyi Yao", PACKAGE_STRING) );
  PyModule_AddObject( M, "__license__", PyString_FromString("GNU GPL version 3 or later") );
  PyModule_AddObject( M, "__version__", PyString_FromString(PACKAGE_VERSION) );
}
