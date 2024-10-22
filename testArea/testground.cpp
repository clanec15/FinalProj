#include <iostream>
#include <vector>
#include <algorithm>
#include "../Source/FileSalvorSrc.cpp"

int main()
{
    std::vector<std::vector<double>> testMtx {
        {2.66 ,0.24 ,7.05 ,0.74 ,8.24 ,5.12 ,0.41 ,5.98 ,3.55 ,3.11, 0},
        {5.18 ,-65535 ,3.70 ,3.09 ,3.87 ,2.51 ,8.89 ,0.64 ,8.37 ,2.50, 1},
        {6.06 ,6.40 ,3.99 ,1.06 ,1.22 ,2.80 ,6.60 ,0.62 ,0.86 ,6.87, 0},
        {0.81 ,8.96 ,7.42 ,5.90 ,5.27 ,8.43 ,2.53 ,1.40 ,2.03 ,5.59, 0},
        {0.51 ,4.31 ,5.58 ,4.66 ,6.74 ,-65535 ,7.63 ,2.68 ,3.35 ,8.39, 1},
        {2.14 ,2.18 ,2.98 ,3.41 ,5.65 ,2.64 ,0.47 ,8.90 ,0.97 ,7.75, 1},
        {6.01 ,5.33 ,0.95 ,7.72 ,3.73 ,0.06 ,7.50 ,8.09 ,0.52 ,5.82, 1},
        {6.45 ,6.67 ,-65535 ,0.95 ,3.40 ,7.12 ,4.31 ,5.75 ,3.08 ,5.10, 1},
        {1.09 ,5.71 ,6.95 ,6.98 ,1.23 ,0.15 ,4.39 ,3.28 ,1.77 ,8.24, 0},
        {2.27 ,4.70 ,0.97 ,8.07 ,0.68 ,8.24 ,5.00 ,-65535 ,7.72 ,0.07, 0}
    };

    FileSalvorNR test;
    int a = 0;
    

    std::vector<std::vector<double>> means = test.DataMeanCalculation(testMtx);

    

    

    for(int i = 0; i < means.size(); i++){
        std::cout << "ID[" << i << "]: ";
        for(int j = 0; j < means[0].size(); j++){
            std::cout << means[i][j] << "\t";
        }
        std::cout << std::endl;
    }
     
    

    for(int i = 0; i < testMtx.size(); i++){
        for(int j = 0; j < testMtx[0].size();j++){
            std::cout << testMtx[i][j] << "\t";
        }
        std::cout << std::endl;
   }

   std::cout << std::endl;
    
    

    test.DataSet(testMtx, means);

   std::cout << "Despues de la modificcion: " << std::endl;

   for(int i = 0; i < testMtx.size(); i++){
        for(int j = 0; j < testMtx[0].size();j++){
            std::cout << testMtx[i][j] << "\t";
        }
        std::cout << std::endl;
   }

   std::cout << std::endl;

   

    

   
    

    

    return 0;
}