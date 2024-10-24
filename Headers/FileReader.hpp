//Header for file reading, 2024


#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

class FileReader
{
    public:
    FileReader();
    ~FileReader();
    std::string inputFile;
    std::vector<std::string> getLines();
};