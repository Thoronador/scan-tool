/*
 -------------------------------------------------------------------------------
    This file is part of scan-tool.
    Copyright (C) 2015  Thoronador

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

#include <iostream>
#include "Curly.hpp"
#include "ScannerVirusTotal.hpp"

const int rcInvalidParameter = 1;

int main(int argc, char ** argv)
{
  std::string key = "";

  if ((argc>1) and (argv!=NULL))
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        if ((param=="--key") or (param=="--apikey"))
        {
          //enough parameters?
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            key = std::string(argv[i+1]);
            ++i; //skip next parameter, because it's used as API key already
            std::cout << "API key was set to \"" << key << "\"." << std::endl;
          }
          else
          {
            std::cout << "Error: You have to enter some text after \""
                      << param <<"\"." << std::endl;
            return rcInvalidParameter;
          }
        }//API key
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\"." << std::endl;
                    //<< "Use --help to get a list of valid parameters.\n";
          return rcInvalidParameter;
        }
      }//parameter exists
      else
      {
        std::cout << "Parameter at index " << i << " is NULL." << std::endl;
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    }//while
  }//if arguments present

  if (key.empty())
  {
    std::cout << "Error: This program won't work properly without an API key! "
              << "Use --apikey to specifiy the VirusTotal API key." << std::endl;
    return rcInvalidParameter;
  }

  ScannerVirusTotal scanVT(key);
  //use SHA256 hash as resource identifier
  const std::string resource = "8d44a0cce1e229179fb1369842750d537606793bcb63686ce25f9e9c13885295";
  ScannerVirusTotal::Report report;
  if (!scanVT.getReport(resource, report))
  {
    std::cout << "Error: Could not retrieve report!" << std::endl;
    return 1;
  }

  std::cout << "Report data:" << std::endl
            << "  response code: " << report.response_code << std::endl
            << "  verbose message: " << report.verbose_msg << std::endl
            << "  resource: " << report.resource << std::endl
            << "  scan_id: " << report.scan_id << std::endl
            << "  scan_date: " << report.scan_date << std::endl
            << "  scan engines: " << report.total << std::endl
            << "  engines that detected a threat: " << report.positives << std::endl
            << "  permalink: " << report.permalink << std::endl
            << "  SHA256: " << report.sha256 << std::endl;

  return 0;
}
