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
#include <string>
#include <iostream>

////////
// docstrings
//////////////////////
PyDoc_STRVAR( Module_doc,
"This module provides a tool to generate and manipulate BMP and GIF images.\n\
\n\
There are three classes, vpixels.bmp, vpixels.gif, and vpixels.gifimage in\n\
the module. A vpixels.bmp class represents a BMP image. A vpixels.gif class\n\
represents a GIF image, and a vpixels.gifimage class an image (a.k.a. frame)\n\
in a GIF image.\n\
\n\
A vpixels.bmp object can be exported to a BMP file. A BMP file can be imported\n\
into a vpixels.bmp object for manipulating.\n\
\n\
A vpixels.gif object can be exported to a GIF file. A GIF file can be imported\n\
into a vpixels.gif object, so pixels in very image can be manipulated." );

PyDoc_STRVAR( about_doc,
"about()\n\n\
Display information about the module." );

PyDoc_STRVAR( version_doc,
"version() -> str\n\n\
Return version of the module." );

////////
// define module
//////////////////////
namespace
{
  #define COPYRIGHT "Copyright (C) 2019 Xueyi Yao"
  #define LICENSE   "GNU GPL version 3 or later"

  // version of the module
  PyObject* Version( PyObject*, PyObject* )
  {
    return PyString_FromString( PACKAGE_VERSION );
  }

  // about the module
  PyObject* About( PyObject*, PyObject* )
  {
    const std::string Notice{ PACKAGE_STRING "\n"
                              COPYRIGHT "\n"
                              "License: " LICENSE "\n"
                              "Bug report: " PACKAGE_BUGREPORT "\n"
                              "[Python " PY_VERSION ", " COMPILER_STRING ", " __DATE__ ", " __TIME__ "]" };

    std::cout << Notice << std::endl;
    Py_RETURN_NONE;
  }

  // module methods
  PyMethodDef Methods[] = {
    MDef( version, Version, METH_NOARGS, version_doc )
    MDef( about,   About,   METH_NOARGS, about_doc )
    { nullptr, nullptr, 0, nullptr } 
  };

#if PY_MAJOR_VERSION == 3
  struct PyModuleDef ModuleDef = {
    PyModuleDef_HEAD_INIT,
    PACKAGE_NAME,  // m_name
    Module_doc,    // m_doc
    -1,            // m_size
    Methods,       // m_methods
    nullptr,       // m_reload
    nullptr,       // m_traverse
    nullptr,       // m_clear
    nullptr,       // m_free
  };
#endif

  /////////////
  // module initialization
  /////////////////////////////////
  PyObject* InitModule()
  {
    // initialize types
    if( PyType_Ready( &PyBmp::Bmp_Type ) < 0 )
      return nullptr;

    if( PyType_Ready( &PyGif::Gif_Type ) < 0 )
      return nullptr;

    if( PyType_Ready( &PyGifImage::GifImage_Type ) < 0 )
      return nullptr;

    // new module
#if PY_MAJOR_VERSION == 3
    PyObject* M = PyModule_Create( &ModuleDef );
#else
    PyObject* M = Py_InitModule3( PACKAGE_NAME, Methods, Module_doc );
#endif
    if( M == nullptr )
      return nullptr;

    Py_INCREF( &PyBmp::Bmp_Type );
    Py_INCREF( &PyGif::Gif_Type );
    Py_INCREF( &PyGifImage::GifImage_Type );

    // add types
    PyModule_AddObject( M, "bmp", reinterpret_cast<PyObject*>(&PyBmp::Bmp_Type) );
    PyModule_AddObject( M, "gif", reinterpret_cast<PyObject*>(&PyGif::Gif_Type) );
    // do not need to add GifImage_Type. doing so is to allow the use of help()
    // to display docstrings without having to have a gifimage instance.
    PyModule_AddObject( M, "gifimage", reinterpret_cast<PyObject*>(&PyGifImage::GifImage_Type) );

    // add strings
    PyModule_AddObject( M, "__copyright__",
                        PyString_FromFormat("%s %s", PACKAGE_STRING, COPYRIGHT) );
    PyModule_AddObject( M, "__license__", PyString_FromString(LICENSE) );
    PyModule_AddObject( M, "__version__", PyString_FromString(PACKAGE_VERSION) );

    return M;
  }
}

/////////////
// wrapping module initialization 
///////////////////////////////////////
#if PY_MAJOR_VERSION == 3
PyMODINIT_FUNC PyInit_vpixels()
{
  return InitModule();
}
#else
PyMODINIT_FUNC initvpixels()
{
  InitModule();
}
#endif
