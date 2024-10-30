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


int main()
{


    //Input/Output folders check
    if((!fs::exists("./input") || fs::is_empty("./input")) || !fs::exists("./output")){
        std::cout << "La carpeta de entrada/salida (input/output) no existe o esta vacia\nPor favor cree la carpeta y coloque archivos en ella para utilizar el programa" << std::endl;
        return EXIT_FAILURE;
    }

    CleanTerminal();
    std::vector<fs::path> files = fileSearching();
	int cols = GetTerminalSize().columns;
	MatrixData first = fileReading(cols, files);

    if((first.dataSize == INT_MAX || first.frameSize == INT_MAX) && first.Matrix.size() == 0){
        std::cerr << "LECTURA INCORRECTA" << std::endl;
        return EXIT_FAILURE;
    }

	int dataSize = first.dataSize-2;
	int frameSize = first.frameSize;
	std::vector<std::vector<double>> firstMtx = first.Matrix;
	std::string file = first.fileName;

    std::cout << "Comprobando datos..." << std::endl;
    std::cout << std::setw(((cols - std::string("Desea generar un reporte en caso de datos no legibles? [Y/n]: ").size())/2)+1) << std::setfill(' ') << "\0" << "Desea generar un reporte en caso de datos no legibles? [Y/n]: ";
    char sel;
    std::cin >> sel;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if(std::tolower(sel) == 'n'){
        mtxProcessingNR(dataSize, frameSize, firstMtx, file);
    } else {
        mtxProcessingWR(cols, dataSize, frameSize, firstMtx, file);
    }

    


	std::cout << "Comenzando proceso de calculo de similitud" << std::endl;
	std::this_thread::sleep_for(5000ms);


    

    MatrixData second = fileReading(cols, files);

    if((second.dataSize == -65535 || second.frameSize == -65535) && second.Matrix.size() == 0){
        std::cerr << "LECTURA INCORRECTA" << std::endl;
        return EXIT_FAILURE;
    }

    int dt = second.dataSize-2;
    int frs = second.frameSize;
    std::vector<std::vector<double>> secondMtx = second.Matrix;
    std::string fl = second.fileName;
    std::cout << "Usando modo de 'No Reporte' para la computacion rapida de similitud" << std::endl;
    std::this_thread::sleep_for(3000ms);
    mtxProcessingNR(dt, frs, secondMtx, fl);

    SimilCalc similitud;

    similitud.SetFstMtx(firstMtx);
    similitud.setSecMtx(secondMtx);


    
    CleanTerminal();

    std::ofstream similFile("./output/similitude.txt", std::ios::app);

    

    similFile << "Similitud entre Matriz 1 y Matriz 2\n";


    for(int i = 0; i < (dt < dataSize ? dt : dataSize); i++){
        similitud.diffCalc(i);
        SimilCalc::diffData output;
        output = similitud.getMSimRow();

        similFile << "La fila " << i << " de la primera matriz tiene una similitud con la fila " << output.idx << " de la segunda matriz con una similitud de: " << "(" << output.diff << ")";
        if(output.diff == 0.0){

            similFile << " [CONCORDANCIA EXACTA!!!]\n";
            
        } else {

            similFile << "\n";

        }
        
    }

    similFile.close();

    CleanTerminal();

    std::cout << "Programa terminado..." << std::endl;

    return 0;
    
}