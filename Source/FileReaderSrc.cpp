#include "../Headers/FileReader.hpp"

FileReader::FileReader()
{
    inputFile = "";
}

FileReader::~FileReader()
{}


std::vector<std::string> FileReader::getLines()
{
    std::string buf;
    std::ifstream Read(inputFile);
    std::vector<std::string> output;
    

    while(getline(Read,buf)){
        output.push_back(buf);
    }
    
    Read.close();

    return output;
}