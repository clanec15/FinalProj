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
    const fs::path dir("./testFolder");
    std::vector<fs::path> files;

    fileSearching(dir, files);
    

    for(int i = 0; i < files.size(); i++){
        std::cout << "[" << i << "]: ";
        std::cout << files[i].string() << std::endl;
    }

    return 0;
}   