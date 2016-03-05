/*
 -------------------------------------------------------------------------------
    This file is part of scan-tool.
    Copyright (C) 2016  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef SCANTOOL_VT_CACHEOPERATION_HPP
#define SCANTOOL_VT_CACHEOPERATION_HPP

namespace scantool
{

namespace virustotal
{

///enumeration for possible cache operations
enum class CacheOperation { None, //none yet
                            ExistenceCheck, //check existence of cache directory
                            IntegrityCheck, //integrity check for cached files
                            Statistics, //cache statistics
                            Update //update existing files
                          };

} //namespace

} //namespace

#endif // SCANTOOL_VT_CACHEOPERATION_HPP
