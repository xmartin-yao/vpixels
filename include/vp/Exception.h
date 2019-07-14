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

#ifndef VP_EXCEPTION_H
#define VP_EXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

namespace vp
{
  // Use this macro throwing exceptions.
  // Error message will include information of the C++ source file (file
  // name, line number and function name) which threw the exception.

  #define VP_THROW( msg ) \
  {                       \
    std::string path = __FILE__;   \
    std::stringstream msgstream;   \
    msgstream << msg << std::endl; \
    msgstream << "  File '" << path.substr(path.rfind("/") + 1) \
              << "', line " << __LINE__      \
              << ", in '" << __func__ << "'" \
              << std::endl;                  \
    throw vp::Exception( msgstream.str() );  \
  }


  //////////////////////////////////////
  class Exception : public std::exception
  {
  public:
    // ctors
    explicit Exception( const std::string& );
    Exception( const Exception& );
    Exception( Exception&& ) noexcept;

    virtual ~Exception() {}

    // not implemented
    Exception() = delete;
    Exception& operator=( const Exception& ) = delete;
    Exception& operator=( Exception&& ) = delete;

    // override 
    virtual const char* what() const noexcept override;

  private:
    std::string m_Message;
  };

} //namespace vp

#endif //VP_EXCEPTION_H
