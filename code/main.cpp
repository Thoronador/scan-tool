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

int main()
{
  std::cout << "Hello world!" << std::endl;

  Curly cURL;
  cURL.setURL("http://www.example.com/");
  cURL.addPostField("foo", "bar");
  std::string response = "";
  if (!cURL.perform(response))
  {
    std::cout << "Error: Request could not be performed." << std::endl;
    return 1;
  }
  if (response.empty())
    response = "(empty)";
  std::cout << "Request was successful!" << std::endl
            << "Code: " << cURL.getResponseCode() << std::endl
            << "Content-Type: " << cURL.getContentType() << std::endl
            << "Response text: " << response << std::endl;
  return 0;
}
