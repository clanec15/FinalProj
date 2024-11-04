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
    double test = 234.1234;


    std::cout << round(test*100.0)/100.0 <<std::endl;

    return 0;
}   