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
    void setInputFile(std::string);
    std::vector<std::string> getLines();

    private:
    std::string inputFile;
    
};