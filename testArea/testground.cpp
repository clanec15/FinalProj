#include <iostream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include "../Headers/TUIController.hpp"


namespace fs = std::filesystem;



int main()
{
    
    int cols = GetTerminalSize().columns;
    const fs::path dir("testFolder/");
    std::vector<fs::path> files;


    for(auto const& dir_entry : fs::directory_iterator(dir)){
        files.push_back(dir_entry.path());
    }

    CleanTerminal();
    std::cout << std::setw((cols-std::string("Select a file: ").size())/2) << std::setfill(' ') << "Select a file:" << std::endl;
    for(int i = 0; i < files.size(); i++){
        std::cout << "[" << char(97+i) << "]: " << files[i].filename().string() << std::endl;
    }

    char sel;
    std::cout << "\n[S]: ";
    std::cin >> sel;

    std::ifstream file(files[sel-97].string());

    std::string test;

    CleanTerminal();
    std::cout << "Output: " << std::endl;
    while(getline(file, test))
    {
        std::cout << test << std::endl;;
    }
    std::cout << std::endl;


    return 0;
}   