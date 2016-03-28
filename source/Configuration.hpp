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

#ifndef SCANTOOL_CONFIGURATION_HPP
#define SCANTOOL_CONFIGURATION_HPP

#include <string>
#include <vector>

/** \brief class that holds the configuration settings
 */
class Configuration
{
  public:
    /// constructor
    Configuration();


    /** \brief character that identifies comment lines
     */
    static const char cCommentCharacter;


    /** \brief returns the API key
     *
     * \return returns the API key. Returns an empty string,
     *         if no API key was read yet.
     */
    const std::string& apikey() const;


    /** \brief clears/resets existing values
     */
    void clear();


    /** \brief tries to read the configuration file from the given file name.
     *
     * \param fileName path to the configuration file
     * \return Returns true in case of success, or false if an error occurred.
     */
    bool loadFromFile(const std::string& fileName);
  private:
    std::string m_apikey; /**< the API key */
}; //class

#endif // SCANTOOL_CONFIGURATION_HPP