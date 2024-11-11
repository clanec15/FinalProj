/**
 * @file FileReader.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Class for reading project related files
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

#ifndef FILEREADER_HPP_
#define FILEREADER_HPP_
/**
 * @brief Main class
 * 
 */
class FileReader
{
    public:
    FileReader(); //Constructor
    ~FileReader(); //Destructor
    void setInputFile(std::string);
    std::vector<std::string> getLines();

    private:
    std::string inputFile; /**The input filename */
    
};

#endif