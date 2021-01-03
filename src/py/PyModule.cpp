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
#include "Util.h"
#include "config.h"
#include <string>
#include <iostream>

////////
// docstrings
//////////////////////
PyDoc_STRVAR( Module_doc,
"This module provides a tool to generate and manipulate BMP and GIF images.\n\
\n\
There are three classes, " PACKAGE_NAME ".bmp, " PACKAGE_NAME ".gif, and " PACKAGE_NAME ".gifimage in\n\
the module. A " PACKAGE_NAME ".bmp class represents a BMP image. A " PACKAGE_NAME ".gif class\n\
represents a GIF image, and a " PACKAGE_NAME ".gifimage class an image (a.k.a. frame)\n\
in a GIF image.\n\
\n\
A " PACKAGE_NAME ".bmp object can be exported to a BMP file. A BMP file can be imported\n\
into a " PACKAGE_NAME ".bmp object for manipulating.\n\
\n\
A " PACKAGE_NAME ".gif object can be exported to a GIF file. A GIF file can be imported\n\
into a " PACKAGE_NAME ".gif object, so pixels in very image can be manipulated." );

PyDoc_STRVAR( about_doc,
"about()\n\n\
Display information about the module." );

PyDoc_STRVAR( version_doc,
"version() -> str\n\n\
Return the version of the module.\n\n\
version(v [, exact]) -> bool\n\n\
   v: required version\n\
   exact: if True, check whether the version of the module is exactly the same as\n\
          the required; otherwise, check whether it is at least the required or\n\
          a later version. False by default.\n\n\
Verify the version of the module." );

////////
// define module
//////////////////////
namespace
{
  #define COPYRIGHT "Copyright (C) 2019 Xueyi Yao"
  #define LICENSE   "GNU GPL version 3 or later"

  // verify the module's version
  PyObject* CheckVersion( PyObject* args )
  {
    const char* ReqVersion = nullptr;
    auto pyBool = Py_False;  // False by default
    if( !PyArg_ParseTuple(args, "s|O!", &ReqVersion, &PyBool_Type, &pyBool) )
      return nullptr;

    bool Exact = PyObject_IsTrue( pyBool );
    if( Util::CheckVersion( ReqVersion, PACKAGE_VERSION, Exact ) )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }

  // return or verify the module's version
  PyObject* Version( PyObject*, PyObject* args )
  {
    if( PyTuple_Size( args ) == 0 )
      return PyString_FromString( PACKAGE_VERSION );
    else
      return CheckVersion( args );
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
    MDef( version, Version, METH_VARARGS, version_doc )
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
    if( PyModule_AddObject( M, "bmp", reinterpret_cast<PyObject*>(&PyBmp::Bmp_Type) ) != 0 )
    {
      Py_DECREF( M );
      Py_DECREF( &PyBmp::Bmp_Type );
      return nullptr;
    }

    if( PyModule_AddObject( M, "gif", reinterpret_cast<PyObject*>(&PyGif::Gif_Type) ) != 0 )
    {
      Py_DECREF( M );
      Py_DECREF( &PyGif::Gif_Type );
      return nullptr;
    }

    // do not need to add GifImage_Type. doing so is to allow the use of help()
    // to display docstrings without having to have a gifimage instance.
    if( PyModule_AddObject( M, "gifimage", reinterpret_cast<PyObject*>(&PyGifImage::GifImage_Type) ) != 0 )
    {
      Py_DECREF( M );
      Py_DECREF( &PyGifImage::GifImage_Type );
      return nullptr;
    }

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
