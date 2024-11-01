#include "../Headers/FileOperations/FileReader.hpp"

//Constructor
FileReader::FileReader()
{
    inputFile = "";
}

//Destructor
FileReader::~FileReader(){}


/**
 * Sets the input file for the FileReader
 * @param file the input file
 */
void FileReader::setInputFile(std::string file)
{
    inputFile=file;
}

/**
 * Gets all the rows from the input file as a string and outputs it as an array
 * 
 * @return the array of rows
 */
std::vector<std::string> FileReader::getLines()
{
    std::string buf;
    std::ifstream Read(inputFile);
    std::vector<std::string> output;

    if(!Read.is_open()){
        std::cerr << "Error al abrir el archivo!!!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    

    while(getline(Read,buf)){
        output.push_back(buf);
    }

    return output;
}