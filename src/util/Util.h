////////////////////////////////////////////////////////////////////////
// Copyright (C) 2021 Xueyi Yao
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
// along with VPixels. If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#ifndef Util_h
#define Util_h

namespace Util 
{
  // Compare ReqVersion with Version.
  bool CheckVersion( const char* ReqVersion, const char* Version, 
                     const bool& Exact = false );
}

#endif //Util_h
