#include "../Headers/FileOperations/FileReader.hpp"

FileReader::FileReader()
{
    inputFile = "";
}

FileReader::~FileReader(){}

void FileReader::setInputFile(std::string file)
{
    inputFile=file;
}

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