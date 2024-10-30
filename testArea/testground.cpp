#include <iostream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include "../Headers/TUIController.hpp"
#include "../Headers/MatrixOps.hpp"


namespace fs = std::filesystem;



int main()
{
    
    int cols = GetTerminalSize().columns;
    const fs::path dir("../input/");
    std::vector<fs::path> files;

    files = fileSearching(dir);

    MatrixData fileTest = fileReading(cols, files);

    

    return 0;
}   