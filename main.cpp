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
#include <climits>
namespace fs = std::filesystem;


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

        similFile << "La fila " << i+1 << " de la primera matriz tiene una similitud con la fila " << output.idx + 1 << " de la segunda matriz con una similitud de: " << "(" << std::fixed << std::setprecision(7) << output.diff << ")";
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

    if(ids.size() == 1){
        similFile << "ID unico detectado!\n\n";
    }

    
    similFile << std::fixed;
    similFile << std::setprecision(7);
    similFile.unsetf(std::ios::scientific);
    
    for(int i = 0; i < output.size(); i++){
        similFile << "La fila " << i+1 << " Tiene una similitud con el ID " << output[i].idx << " Con una probabilidad total de: " << output[i].prob << "\n";
    }
}

int main()
{
    const fs::path entryPath = "./input";
    std::vector<fs::path> files;
    TerminalSizeGet TUIHelper;
    int cols = TUIHelper.GetTerminalSize().columns;
    std::string callout;
    FileSalvor mainT;


    //Input/Output folders check
    if((!fs::exists("./input") || fs::is_empty("./input")) || !fs::exists("./output")){
        std::cout << "La carpeta de entrada/salida (input/output) no existe o esta vacia\nPor favor cree la carpeta y coloque archivos en ella para utilizar el programa" << std::endl;
        return EXIT_FAILURE;
    }

    CleanTerminal();
    fileSearching(entryPath, files);
    std::cout << "Comprobando datos..." << std::endl;
	MatrixData first = fileReading(cols, files);
    checkMatrix(first);

//                                                                                          Separator                   

    
    callout = "Desea generar un reporte en caso de datos no legibles? [Y/n]: ";

    int separator = ((cols - callout.size())/2)+1;
    std::cout << std::setw(separator) << std::setfill(' ') << "\0" << callout;
    char sel;
    std::cin >> sel;

    std::cout <<  "Desea calcular los datos de reemplazo por medio del\n[a] Promedio (Mas Rapido)\n[b] Mediana (Mas Lento)\n\n[S]:";
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

        data = mainT.DataMeanCalculation(first.Matrix);
        break;
    }


    CleanTerminal();
    std::cout  << "Datos de reemplazo: \n";
    std::cout  << "\t";
    for(int h = 0; h < data[0].size(); h++){
        std::cout  << "[" << h+1 << "]\t";
    }
    std::cout  << "\n";
    for(int i = 0; i < data.size(); i++){
        std::cout  << "ID[" << i << "]:\t";

        for(int j = 0; j < data[0].size(); j++){
            std::cout  << data[i][j] << (j == data[0].size()-1 ? "\t" : ",\t");
        }

        std::cout  << "\n\n";
    }

    std::cout << "Mostrando datos de reemplazo en caso de datos dañados (Presione enter para continuar...)" << std::endl;
    std::cin.get();

    if(std::tolower(sel) == 'n'){
        std::cin.ignore();
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    MatrixProcessing(std::tolower(sel) == 'n', first, first.fileName, cols, data);
    std::vector<std::vector<double>> meansFirst = mainT.DataMeanCalculation(first.Matrix);

    std::cout << (std::tolower(sel) == 'n' ? "Procesamiento completado... (Presione enter para continuar...)" : "Mostrando reemplazo de datos... (Presione enter para continuar...)") << std::endl;
    std::cin.get();
    std::cin.ignore();

    CleanTerminal();

//                                                                                                  Separator                           

	std::cout << "Comenzando proceso de calculo de similitud (Procesando segunda matriz...)" << std::endl;
    
    MatrixData second = fileReading(cols, files);
    std::cout << "Comprobando datos..." << std::endl;
    checkMatrix(second);


    
    callout = "Desea generar un reporte en caso de datos no legibles? [Y/n]: ";
    std::cout << std::setw(separator) << std::setfill(' ') << "\0" << callout;
    std::cin >> sel;


    std::cout <<  "Desea calcular los datos de reemplazo. Por medio del\n[a] Promedio (Mas Rapido)\n[b] Mediana (Mas Lento)\n\n[S]:";
    std::cin >> SelM;
    std::vector<std::vector<double>> dataSecond;


    switch (std::tolower(SelM))
    {
    case 'a':
        dataSecond = mainT.DataMeanCalculation(second.Matrix);
        break;

    case 'b':
        dataSecond = mainT.DataMedianCalculatuion(second.Matrix);
        break;
    
    default:
        std::cout << "Opcion no reconocida, usando el modo mas rapido (Promedio)" << std::endl;
        std::cin.get();
        std::cin.ignore();
        dataSecond = mainT.DataMeanCalculation(second.Matrix);
        break;
    }
    CleanTerminal();
    std::cout  << "Datos de reemplazo: \n";
    std::cout  << "\t";
    for(int h = 0; h < dataSecond[0].size(); h++){
        std::cout  << "[" << h+1 << "]\t";
    }
    std::cout  << "\n";
    for(int i = 0; i < dataSecond.size(); i++){
        std::cout  << "ID[" << i << "]:\t";

        for(int j = 0; j < dataSecond[0].size(); j++){
            std::cout  << dataSecond[i][j] << (j == dataSecond[0].size()-1 ? "\t" : ",\t");
        }

        std::cout  << "\n\n";
    }

    std::cout << "Mostrando datos de reemplazo en caso de datos dañados (Presione enter para continuar...)" << std::endl;
    std::cin.get();
    if(std::tolower(sel) == 'n'){
        std::cin.ignore();
    } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    

    MatrixProcessing(std::tolower(sel) == 'n', second, second.fileName, cols, dataSecond);
    std::cout << (std::tolower(sel) == 'n' ? "Procesamiento completado... (Presione enter para continuar...)" : "Mostrando reemplazo de datos... (Presione enter para continuar...)") << std::endl;

    std::cout << "Desea calcular la similitud de matrices por\n[a] Suma de valores absolutos\n[b] Probabilidad Bayesiana\n\n[S]:";
    char SimSel;
    std::cin >> SimSel;
    
    CleanTerminal();

    std::ofstream similFile("./output/similReport.txt");

    if(similFile.fail()){
        std::cerr << "REPCRERR!" << std::endl;
        std::abort();
    } else if(!similFile.is_open()){
        std::cerr << "REPOPERR!" << std::endl;
        std::abort();
    }

    similFile << "PLCH\n";
    
    switch (std::tolower(SimSel))
    {
    case 'a':
        similFile << "Similitud entre Matriz 1 (" << first.fileName << ") y Matriz 2 (" << second.fileName << ") [Suma de Valores Absolutos]\n";
        SimilAbs(first, second, similFile);
        break;

    case 'b':
        similFile << "Similitud entre Matriz 1 (" << first.fileName << ") y Matriz 2 (" << second.fileName << ") [Probabilidad Bayesiana]\n";
        SimilBayesian(first, second, similFile, meansFirst);
        break;
    
    default:
        std::cout << "Usando opcion mas rapida (Suma de Valores Absolutos)" << std::endl;
        similFile << "Similitud entre Matriz 1 (" << first.fileName << ") y Matriz 2 (" << second.fileName << ") [Suma de Valores Absolutos]\n";
        SimilAbs(first, second, similFile);
        break;
    }   


    similFile.close();
    CleanTerminal();
    std::cout << "Programa terminado..." << std::endl;
    return 0;
}