#ifndef MATRIXOPS_HPP
#define MATRIXOPS_HPP
#include <string>
#include <vector>
#include <iomanip>
#include "../Source/FileReaderSrc.cpp"
#include "../Source/RowParserSrc.cpp"
#include "../Source/FileSalvorSrc.cpp"


struct MatrixData
{
	int dataSize;
	int frameSize;
	std::string fileName;
	std::vector<std::vector<double>> Matrix;
};

void CheckMtx(int dataSize, int frameSize, FileSalvor& helper, std::vector<std::vector<double>> mtx)
{
    for(int i = 0; i < dataSize; i++){
            for(int j = 0; j < frameSize; j++){
                if(mtx[i][j] == static_cast<double>(-65535)) {
                    helper.SetDataStatus(true);
                }
                std::cout << mtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
            }
            std::cout << std::endl;
        }
    std::cout << std::endl;
}

void SaveNOutputMtx(int frameSize, std::ofstream& file, std::vector<std::vector<double>> mtx)
{
    for(int i = 0; i < mtx.size(); i++){
        for(int j = 0; j < mtx[0].size(); j++){
            std::cout << mtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
            file << mtx[i][j] << (j == frameSize-1 ? "" : ",");
        }
        file << "\n";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void mtxProcessingNR(int dataSize, int frameSize, std::vector<std::vector<double>>& DataMtx, std::string outputFName)
{
    FileSalvorNR salvor;
    std::vector<std::vector<double>> means = salvor.DataMeanCalculation(DataMtx);
    std::ofstream outputFile("./output/" + outputFName);
    outputFile << dataSize << "\n";
    outputFile << frameSize << "\n";
    outputFile << "2\n";

    CheckMtx(dataSize, frameSize, salvor, DataMtx);
        
    if(salvor.GetDataStatus()){
        std::cout << "Datos no reconocidos en el archivo!, Corrigiendo" << std::endl;
            
        salvor.DataSet(DataMtx, means);

        std::cout << "Datos Corregidos: " << std::endl;
        SaveNOutputMtx(frameSize, outputFile, DataMtx);
        std::cout << std::endl;

        outputFile.close();
    } else {
        SaveNOutputMtx(frameSize, outputFile, DataMtx);
        std::cout << "Datos correctos..." << std::endl;
        outputFile.close();  
    }
    salvor.~FileSalvorNR();
}

void mtxProcessingWR(int cols, int dataSize, int frameSize, std::vector<std::vector<double>>& DataMtx, std::string outputFName)
{
    FileSalvorWR salvor;
    std::string RepFile;
    std::vector<std::vector<double>> means = salvor.DataMeanCalculation(DataMtx);
    std::cout << std::setw(((cols - std::string("Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ").size())/2)-64) << std::setfill(' ') << "\0" << "Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ";
    std::cin >> RepFile;


    std::ofstream outputFile("./output/" + outputFName);
    outputFile << dataSize << "\n";
    outputFile << frameSize << "\n";
    outputFile << "2\n";
        
    CheckMtx(dataSize, frameSize, salvor, DataMtx);

    if(salvor.GetDataStatus()){
        std::cout << "Datos no reconocidos en el archivo!, Corrigiendo" << std::endl;

        if(!RepFile.empty()){
            salvor.DataSet(DataMtx, means, "./output/" + RepFile);
        } else {
            salvor.DataSet(DataMtx, means);
        }
            

        std::cout << "Datos Corregidos: " << std::endl;
        SaveNOutputMtx(frameSize, outputFile, DataMtx);

        outputFile.close();
    } else {
        SaveNOutputMtx(frameSize, outputFile, DataMtx);
        std::cout << "Datos correctos..." << std::endl;
        outputFile.close();
    }
}

#endif