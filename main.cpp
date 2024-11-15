/**
 * @file main.cpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Compile with "Make", clean everything with "Make clean"
 *        Main file
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


//System libraries
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <thread>
#include <climits>
namespace fs = std::filesystem;
using namespace std::chrono_literals;

//--------------------------------------//

//Custom libraries
#include "Source/SimilCalcSrc.cpp"
#include "Headers/TUIController.hpp"
#include "Headers/MatrixOps.hpp"


//-------------------------------------//           


/**
 * check if the matrix is a valid matrix input. If it isnt, it exits with failure
 */
void checkMatrix(MatrixData data)
{
    if((data.dataSize == INT_MAX || data.frameSize == INT_MAX) && data.Matrix.size() == 0){
        std::cerr << "LECTURA INCORRECTA" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void SimilAbs(MatrixData first, MatrixData second, std::ofstream& similFile)
{
    SimilCalcAbs abs;
    

    abs.SetMatrixA(first.Matrix);
    abs.SetMatrixB(second.Matrix);
        
    for(int i = 0; i < std::min(first.dataSize, second.dataSize); i++){
        abs.DiffCalculation(i);
        SimilCalcAbs::diffData output;
        output = abs.getMostSimilarRow();

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
    SimilCalcBayesian Bay;
    FileSalvor hMan;

    Bay.SetMatrixA(first.Matrix);
    Bay.SetMatrixB(second.Matrix);
    std::vector<int> ids = hMan.GetMatrixIDs(first.Matrix);

    Bay.DiffCalculation(ids, means);
    std::vector<SimilCalcBayesian::ProbabilityData> output = Bay.getOutputProb();
    std::cout << output.size() << std::endl;


    for(int i = 0; i < output.size(); i++){
        similFile << "La fila " << i << " Tiene una similitud con el ID " << output[i].idx << " Con una probabilidad total de: " << output[i].prob << "\n";
    }
}

int main()
{
    const fs::path entryPath = "./input";
    std::vector<fs::path> files;
    int cols = GetTerminalSize().columns;
    std::string callout;
    FileSalvor mainT;


    //Input/Output folders check
    if((!fs::exists("./input") || fs::is_empty("./input")) || !fs::exists("./output")){
        std::cout << "La carpeta de entrada/salida (input/output) no existe o esta vacia\nPor favor cree la carpeta y coloque archivos en ella para utilizar el programa" << std::endl;
        return EXIT_FAILURE;
    }

    CleanTerminal();
    fileSearching(entryPath, files);
	MatrixData first = fileReading(cols, files);
    checkMatrix(first);

    std::cout << "Comprobando datos..." << std::endl;
    callout = "Desea generar un reporte en caso de datos no legibles? [Y/n]: ";
    std::cout << std::setw(((cols - callout.size())/2)+1) << std::setfill(' ') << "\0" << callout;
    char sel;
    std::cin >> sel;
    std::vector<std::vector<double>> meansFirst = mainT.DataMeanCalculation(first.Matrix);

    std::cout <<  "Desea calcular los datos de reemplazo por medio del\n[a] Promedio (Mas Rapido)\n[b] Mediana(Mas Lento)\n\n[S]:";
    char SelM;
    std::cin >> SelM;
    std::vector<std::vector<double>> data;


    switch (std::tolower(SelM))
    {
    case 'a':
        data = mainT.DataMeanCalculation(first.Matrix);
        break;

    case 'b':
        data = mainT.DataMedianCalculatuion(first.Matrix);
        break;
    
    default:
        std::cout << "Opcion no reconocida, usando el modo mas rapido (Promedio)" << std::endl;
        std::this_thread::sleep_for(2000ms);
        data = mainT.DataMeanCalculation(first.Matrix);
        break;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    MatrixProcessing(std::tolower(sel) == 'n', first, first.fileName, cols, meansFirst);

	std::cout << "Comenzando proceso de calculo de similitud" << std::endl;
    MatrixData second = fileReading(cols, files);
    checkMatrix(second);


    std::cout << "Usando modo de 'No Reporte' para la computacion rapida de similitud" << std::endl;
    std::this_thread::sleep_for(3000ms);

    std::vector<std::vector<double>> meansSecond = mainT.DataMeanCalculation(second.Matrix);
    MatrixProcessing(false, second, second.fileName, cols, meansSecond);

    std::cout << "Desea calcular la similitud de matrices por\n[a] Suma de valores absolutos\n[b] Probabilidad Bayesiana\n\n[S]:";
    char SimSel;
    std::cin >> SimSel;
    
    CleanTerminal();
    std::ofstream similFile("./output/similitude.txt");
    
    switch (std::tolower(SimSel))
    {
    case 'a':
        similFile << "Similitud entre Matriz 1 y Matriz 2 (Suma de valores Absolutos)\n";
        SimilAbs(first, second, similFile);
        break;

    case 'b':
        similFile << "Similitud entre Matriz 1 y Matriz 2 (Probabilidad Bayesiana)\n";
        SimilBayesian(first, second, similFile, meansFirst);
        break;
    
    default:
        std::cout << "Usando opcion mas rapida (Suma de Valores Absolutos)";
        std::this_thread::sleep_for(2000ms);
        SimilAbs(first, second, similFile);
        break;
    }   


    similFile.close();
    CleanTerminal();
    std::cout << "Programa terminado..." << std::endl;
    return 0;
}