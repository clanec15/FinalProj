#include <iostream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "../Headers/TUIController.hpp"
#include "../Headers/MatrixOps.hpp"
#include "../Source/SimilCalcSrc.cpp"


namespace fs = std::filesystem;


int main()
{
    
    long double test = 1.60805e-82;

    if(test < __DBL_EPSILON__){
        std::cout << "YIKEES" << std::endl;
    }

    return 0;
}   