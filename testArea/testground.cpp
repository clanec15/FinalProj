#include <iostream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "../Headers/TUIController.hpp"
#include "../Headers/MatrixOps.hpp"
#include "../Headers/RadixSort.hpp"
#include "../Source/SimilCalcSrc.cpp"


namespace fs = std::filesystem;


int main()
{
    std::vector<double> testArr = {2.66 ,0.24 ,7.05 ,0.74 ,8.24 ,5.12 ,0.41 ,5.98 ,3.55 ,3.11};
    std::vector<double> fixedArr;
    convData test;
    test = dvtoiv(testArr);



    RadixSrt(test.data);

    for(int i = 0; i < test.data.size(); i++){
        fixedArr.push_back(test.data[i]);
    }


    for(int i = 0; i < fixedArr.size(); i++){
        std::cout << fixedArr[i]/pow(10, test.globalExp) << (i == fixedArr.size()-1 ? "" : ", ");
    }

    std::cout << std::endl;
    


    return 0;
}   