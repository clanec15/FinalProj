/**
 * @file RowParser.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Class for parsing rows of the data file
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <sstream>
#include <vector>

#ifndef ROWPARSER_HPP_
#define ROWPARSER_HPP_

/**
 * @brief Main Class
 * 
 */
class RowParser
{
    public:

    RowParser();//Constructor
    ~RowParser();//Destructor
    void ConvertRow(const std::string&, char);
    const std::vector<double>& getParsedRow();
    

    private:
    std::vector<double> parsedRow; /** The parsed row as a double datatype vector */
};

#endif