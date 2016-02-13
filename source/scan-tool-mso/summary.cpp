/*
 -------------------------------------------------------------------------------
    This file is part of scan-tool.
    Copyright (C) 2015, 2016  Thoronador

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

#include "summary.hpp"
#include <algorithm>
#include <iostream>
#include "../../libthoro/filesystem/file.hpp"

namespace scantool
{

namespace metascan
{

void showSummary(const std::map<std::string, std::string>& mapFileToHash,
                 std::map<std::string, Report>& mapHashToReport,
                 std::vector<std::pair<std::string, int64_t> >& largeFiles)
{
  //list possibly infected files
  if (!mapFileToHash.empty())
  {
    std::clog << "Possibly infected files: " << mapFileToHash.size() << std::endl;
    for (const auto& i : mapFileToHash)
    {
      std::clog << i.first << " may be infected!" << std::endl;
      const Report& repMSO = mapHashToReport[i.second];
      for (const auto& engine : repMSO.scan_details)
      {
        if (engine.detected)
          std::clog << "    " << engine.engine << " detected " << engine.result << std::endl;
      } //for engine
      std::clog << std::endl;
    } //for i
  } //if infected files exist in map
  else
  {
    std::cout << "All of the given files seem to be OK." << std::endl;
  }

  //list files which were too large to send to scan
  if (!largeFiles.empty())
  {
    //sort them by size (using a lambda expression)
    std::sort(largeFiles.begin(), largeFiles.end(),
              [](const std::pair<std::string, int64_t>& a, const std::pair<std::string, int64_t>& b)
              {
                   return a.second < b.second;
              }
             );

    //list files
    std::cout << std::endl << largeFiles.size() << " file(s) could not be "
              << "scanned because of file size restrictions for the scan API."
              << std::endl;
    for(const auto& largeElem : largeFiles)
    {
      std::cout << "  " << largeElem.first << " has a size of "
                      << libthoro::filesystem::getSizeString(largeElem.second)
                      << " and exceeds maximum file size for scan! "
                      << "File was skipped." << std::endl;
    } //for (range-based)
  } //if there are some "large" files
}

} //namespace

} //namespace
