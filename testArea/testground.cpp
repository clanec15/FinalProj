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


void SimilAbs(MatrixData first, MatrixData second, std::ofstream& similFile)
{
    SimilCalcAbs test;

    test.SetFirstMtx(first.Matrix);
    test.setSecondMtx(second.Matrix);
        
    for(int i = 0; i < std::min(first.dataSize, second.dataSize); i++){
        test.diffCalc(i);
        SimilCalcAbs::diffData output;
        output = test.getMostSimilarRow();

        similFile << "La fila " << i << " de la primera matriz tiene una similitud con la fila " << output.idx << " de la segunda matriz con una similitud de: " << "(" << output.diff << ")";
        if(output.diff == 0.0){

            similFile << " [CONCORDANCIA EXACTA!!!]\n";
                
        } else {

            similFile << "\n";

        }
        
    }
}

void SimilBayesian(MatrixData& first, MatrixData& second, std::ofstream& similFile, std::vector<std::vector<double>> means)
{
    SimilCalcBayesian test;
    FileSalvor hMan;

    test.SetFirstMtx(first.Matrix);
    test.setSecondMtx(second.Matrix);
    std::vector<int> ids = hMan.GetMatrixIDs(first.Matrix);

    test.diffCalc(ids, means);
    std::vector<SimilCalcBayesian::ProbabilityData> output = test.getOutputProb();


    for(int i = 0; i < output.size(); i++){
        similFile << "La fila " << i << " Tiene una similitud con el ID " << output[i].idx << " Con una probabilidad total de: " << output[i].prob << "\n";
    }
}


int main()
{

        std::vector<std::vector<double>> testMtx {
        {2.66 ,0.24 ,7.05 ,0.74 ,8.24 ,5.12 ,0.41 ,5.98 ,3.55 ,3.11, 0},
        {5.18 ,1.23 ,3.70 ,3.09 ,3.87 ,2.51 ,8.89 ,0.64 ,8.37 ,2.50, 1},
        {6.06 ,6.40 ,3.99 ,1.06 ,1.22 ,2.80 ,6.60 ,0.62 ,0.86 ,6.87, 0},
        {0.81 ,8.96 ,7.42 ,5.90 ,5.27 ,8.43 ,2.53 ,1.40 ,2.03 ,5.59, 0},
        {0.51 ,4.31 ,5.58 ,4.66 ,6.74 ,2.32 ,7.63 ,2.68 ,3.35 ,8.39, 1},
        {2.14 ,2.18 ,2.98 ,3.41 ,5.65 ,2.64 ,0.47 ,8.90 ,0.97 ,7.75, 1},
        {6.01 ,5.33 ,0.95 ,7.72 ,3.73 ,0.06 ,7.50 ,8.09 ,0.52 ,5.82, 1},
        {6.45 ,6.67 ,4.32 ,0.95 ,3.40 ,7.12 ,4.31 ,5.75 ,3.08 ,5.10, 1},
        {1.09 ,5.71 ,6.95 ,6.98 ,1.23 ,0.15 ,4.39 ,3.28 ,1.77 ,8.24, 0},
        {2.27 ,4.70 ,0.97 ,8.07 ,0.68 ,8.24 ,5.00 ,4.32 ,7.72 ,0.07, 0}
    };

        std::vector<std::vector<double>> snTestMtx {
        {0.30 ,3.82 ,0.47 ,2.39 ,0.80 ,8.01 ,5.13 ,1.56 ,1.30 ,8.65, 1},
        {1.08 ,3.08 ,3.70 ,7.38 ,2.52 ,5.15 ,7.14 ,3.91 ,0.54 ,7.43, 0},
        {3.52 ,1.32 ,7.45 ,0.60 ,2.12 ,8.78 ,2.48 ,4.65 ,3.32 ,8.92, 0},
        {6.89 ,2.98 ,4.45 ,3.96 ,7.19 ,1.58 ,2.73 ,6.85 ,8.84 ,4.32, 1},
        {5.19 ,2.47 ,1.30 ,8.77 ,1.25 ,0.03 ,3.44 ,4.56 ,0.39 ,6.06, 0},
        {1.85 ,0.04 ,7.69 ,4.53 ,4.38 ,2.88 ,6.50 ,5.04 ,8.05 ,2.10, 0},
        {7.79 ,2.54 ,6.44 ,7.08 ,3.69 ,6.69 ,4.80 ,1.86 ,3.53 ,2.27, 0},
        {0.17 ,4.94 ,6.79 ,3.80 ,1.38 ,4.38 ,5.02 ,4.95 ,0.21 ,2.91, 1},
        {8.49 ,0.52 ,6.93 ,8.45 ,8.60 ,8.55 ,8.97 ,5.24 ,7.39 ,3.72, 1},
        {6.65 ,3.18 ,0.82 ,5.82 ,1.43 ,3.38 ,0.87 ,3.23 ,6.19 ,1.05, 1}
    };


    



    FileSalvor test;
    MatrixData first;
    first.dataSize = testMtx.size();
    first.frameSize = testMtx[0].size();
    first.Matrix = testMtx;

    MatrixData second;
    second.dataSize = snTestMtx.size();
    second.frameSize = snTestMtx[0].size();
    second.Matrix = snTestMtx;

    std::vector<std::vector<double>> data = test.DataMeanCalculation(first.Matrix);

    std::vector<std::vector<double>> testM;
    double SimilTime = 0.0;
    double BayesianTime = 0.0;
    int samples = 0;

    for(int i = 0; i < 50; i++){
        auto tMedianS = std::chrono::high_resolution_clock::now();
        testM = test.DataMedianCalculatuion(first.Matrix);
        auto tMedianE = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> ms_doubleMed = tMedianE - tMedianS;


        auto tMeanS = std::chrono::high_resolution_clock::now();
        testM = test.DataMeanCalculation(first.Matrix);
        auto tMeanE = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> ms_doubleMe = tMeanE - tMeanS;

        SimilTime += ms_doubleMed.count();
        BayesianTime += ms_doubleMe.count();
        samples++;
    }

    



    std::cout << "Median time: " << SimilTime/samples << "us" << std::endl;
    std::cout << "Meann Time: " << BayesianTime/samples << "us" << std::endl;
        

    return 0;
}   