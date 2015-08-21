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
#include <string>
#include <unordered_set>
#include "Curly.hpp"
#include "ScannerVirusTotal.hpp"
#include "../libthoro/common/StringUtils.h"
#include "../libthoro/filesystem/FileFunctions.hpp"
#include "../libthoro/hash/sha256/FileSourceUtility.hpp"
#include "../libthoro/hash/sha256/sha256.hpp"

//return codes
const int rcInvalidParameter = 1;
const int rcFileError = 2;
const int rcScanError = 3;

void showHelp()
{
  std::cout << "\nscan-tool [FILE ...]\n"
            << "options:\n"
            << "  --help           - displays this help message and quits\n"
            << "  -?               - same as --help\n"
            << "  --version        - displays the version of the program and quits\n"
            << "  -v               - same as --version\n"
            << "  --apikey KEY     - sets the API key for VirusTotal\n"
            << "  --silent         - produce less text on the standard output\n"
            << "  --maybe N        - sets the limit for false positives to N. N must be an\n"
            << "                     unsigned integer value. Default is 2.\n"
            << "  FILE             - file that shall be scanned. Can be repeated multiple\n"
            << "                     times, if you want to scan several files.\n";
}

void showVersion()
{
  std::cout << "scan-tool, version 0.01, 2015-08-22\n";
}

int main(int argc, char ** argv)
{
  //string that will hold the API key
  std::string key = "";
  //whether output will be reduced
  bool silent = false;
  // limit for "maybe infected"; higher count means infected
  int maybeLimit = 0;
  //files that will be checkes
  std::unordered_set<std::string> files_scan = std::unordered_set<std::string>();

  if ((argc>1) and (argv!=NULL))
  {
    int i=1;
    while (i<argc)
    {
      if (argv[i]!=NULL)
      {
        const std::string param = std::string(argv[i]);
        //help parameter
        if ((param=="--help") or (param=="-?") or (param=="/?"))
        {
          showHelp();
          return 0;
        }//help
        //version information requested?
        else if ((param=="--version") or (param=="-v"))
        {
          showVersion();
          return 0;
        } //version
        else if ((param=="--key") or (param=="--apikey"))
        {
          //enough parameters?
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            key = std::string(argv[i+1]);
            ++i; //Skip next parameter, because it's used as API key already.
            if (!silent)
              std::cout << "API key was set to \"" << key << "\"." << std::endl;
          }
          else
          {
            std::cout << "Error: You have to enter some text after \""
                      << param <<"\"." << std::endl;
            return rcInvalidParameter;
          }
        } //API key
        else if ((param=="--silent") or (param=="-s"))
        {
          //enough parameters?
          if (silent)
          {
            std::cout << "Error: Parameter " << param << " must not occur more than once!"
                      << std::endl;
            return rcInvalidParameter;
          }
          silent = true;
        } //silent
        else if ((param=="--maybe") or (param=="--limit"))
        {
          //enough parameters?
          if ((i+1<argc) and (argv[i+1]!=NULL))
          {
            const std::string integer = std::string(argv[i+1]);
            int limit = -1;
            if (!stringToInt(integer, limit))
            {
              std::cout << "Error: \"" << integer << "\" is not an integer!" << std::endl;
              return rcInvalidParameter;
            }
            if (limit < 0)
            {
              std::cout << "Error: " << limit << " is negative, but only"
                        << " non-negative values are allowed here." << std::endl;
              return rcInvalidParameter;
            }
            maybeLimit = limit;
            ++i; //Skip next parameter, because it's used as limit already.
          }
          else
          {
            std::cout << "Error: You have to enter an integer value after \""
                      << param <<"\"." << std::endl;
            return rcInvalidParameter;
          }
        } //"maybe" limit
        //file for scan
        else if (libthoro::filesystem::File::exists(param))
        {
          files_scan.insert(param);
        } //file
        else
        {
          //unknown or wrong parameter
          std::cout << "Invalid parameter given: \"" << param << "\"." << std::endl
                    << "Use --help to get a list of valid parameters.\n" << std::endl;
          return rcInvalidParameter;
        } //if unknown parameter
      } //if parameter exists
      else
      {
        std::cout << "Parameter at index " << i << " is NULL." << std::endl;
        return rcInvalidParameter;
      }
      ++i;//on to next parameter
    } //while
  } //if arguments present

  if (key.empty())
  {
    std::cout << "Error: This program won't work properly without an API key! "
              << "Use --apikey to specifiy the VirusTotal API key." << std::endl;
    return rcInvalidParameter;
  }
  if (files_scan.empty())
  {
    std::cout << "No file scans requested, stopping here." << std::endl;
    return 0;
  } //if no requests

  if (maybeLimit <= 0)
    maybeLimit = 2;

  ScannerVirusTotal scanVT(key, silent);

  //iterate over all files for scan requests
  for(const std::string& i : files_scan)
  {
    const SHA256::MessageDigest fileHash = SHA256::computeFromFile(i);
    if (fileHash.isNull())
    {
      std::cout << "Error: Could not determine SHA256 hash of " << i
                << "!" << std::endl;
      return rcFileError;
    } //if no hash
    ScannerVirusTotal::Report report;
    if (scanVT.getReport(fileHash.toHexString(), report))
    {
      if (report.response_code == 1)
      {
        //got report
        if (report.positives == 0)
        {
          if (!silent)
            std::cout << i << " OK" << std::endl;
        }
        else if (report.positives <= maybeLimit)
        {
          std::clog << i << " might be infected, got " << report.positives
                    << " positives." << std::endl;
        }
        else if (report.positives > maybeLimit)
        {
          std::clog << i << " is INFECTED, got " << report.positives
                    << " positives." << std::endl;
        }
      } //if file was in report database
      else if (report.response_code == 0)
      {
        //no data present for file
        std::string scan_id = "";
        if (!scanVT.scan(i, scan_id))
        {
          std::cerr << "Error: Could not submit file " << i << " for scanning."
                    << std::endl;
          return rcScanError;
        }
        if (!silent)
          std::clog << "Info: File " << i << " was queued for scan. Scan ID is "
                    << scan_id << "." << std::endl;
      } //else
      else
      {
        //unexpected response code
        std::cerr << "Error: Got unexpected response code ("<<report.response_code
                  << ") for report of file " << i << "." << std::endl;
        return rcScanError;
      }
    }
    else
    {
      std::clog << "Warning: Could not get report for file " << i << "!" << std::endl;
    }
  } //for (range-based)

  return 0;
}
