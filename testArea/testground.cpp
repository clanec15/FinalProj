#include <iostream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include "../Headers/TUIController.hpp"
#include "../Headers/MatrixOps.hpp"
#include "../Source/SimilCalcSrc.cpp"


namespace fs = std::filesystem;




int main()
{

    std::cout << truncate(2, 2.45645) << std::endl;
    std::cout << round(2.45645*100)/100 << std::endl;
    return 0;
}   