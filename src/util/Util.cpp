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

#include "Util.h"
#include <regex>

/////////////////////////
// Compare ReqVersion with Version.
// if Exact == true, check whether they are exactly the same.
// otherwise, check if Version is at least ReqVersion or a later version.
/////////////////////////////////////////////////////////////////
bool Util::CheckVersion( const char* ReqVersion, const char* Version,
                         const bool& Exact )
{
  // exactly 3 digital fields separated by a dot
  auto Regex = std::regex( "^(\\d+)\\.(\\d+)\\.(\\d+)$" );

  // it returns false, if ReqVersion does not match Regex.
  // 4 matches expected, ReqDigits[0] is the whole string,
  // others are the 3 fields.
  std::cmatch ReqDigits;
  if( !std::regex_search( ReqVersion, ReqDigits, Regex ) )
    return false;

  std::cmatch Digits;
  if( !std::regex_search( Version, Digits, Regex ) )
    return false;

  if( Exact )
  {
    for( size_t i = 1; i < 4; ++i )
      if( std::stoi(ReqDigits[i], nullptr) != std::stoi(Digits[i], nullptr) )
        return false;
  }
  else
  {
    for( size_t i = 1; i < 4; ++i )
      if( std::stoi(ReqDigits[i], nullptr) > std::stoi(Digits[i], nullptr) )
        return false;
  }

  return true;
}
