#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "../Source/SimilCalcSrc.cpp"



int main()
{
    double test;
    try
    {
        test = std::stod("asdf");
    }
    catch(std::invalid_argument e)
    {
        std::cerr << e.what() << '\n';
        test = 0;
    }

    std::cout << test << std::endl;

    
    

    return 0;
}   