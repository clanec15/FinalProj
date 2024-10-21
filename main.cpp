//Victor Aguilar Rodriguez
//202341753
//Compilar usando el comando make

#include <iostream>
#include <iomanip>
#include <filesystem>
namespace fs = std::filesystem;

#include "Source/FileReaderSrc.cpp"
#include "Source/RowParserSrc.cpp"
#include "Source/FileSalvorSrc.cpp"
#include "Headers/TUIController.hpp"


int main()
{
    CleanTerminal();
    FileReader reader;
    RowParser parser;
    int cols = GetTerminalSize().columns;
    std::string ReqFile, fullFile;

    std::string inputPath = "./input";
    if(fs::is_empty(inputPath)){
        std::cout << "La carpeta de lectura de archivos esta vacia, porfavor coloca archivos ahi antes de correr el programa..." << std::endl;
        return EXIT_FAILURE;
    }


    while(true){
        std::cout << std::setw(((cols - std::string("Coloque el nombre del archivo: ").size())/2)-64) << std::setfill(' ') << "\0" << "Coloque el nombre del archivo: ";
        std::cin >> ReqFile;
        fullFile = "./input/" + ReqFile;

        std::ifstream file(fullFile);
        if(file.good()){
            break;
        } else {
            std::cout << std::setw(((cols - std::string("El archivo no se ha encontrado, desea volver a colocarlo? [Y/n]: ").size())/2)-64) << std::setfill(' ') << "\0" << "El archivo no se ha encontrado, desea volver a colocarlo? [Y/n]: ";
            char sel;
            std::cin >> sel;
            if(std::toupper(sel) == 'Y'){
                CleanTerminal();
                continue;
            } else {
                std::cerr << "Usuario decidio terminar el programa..." << std::endl;
                std::cin.get();
                return -1;
            }
        }

    }

    reader.inputFile = fullFile;
    std::vector<std::string> lines = reader.getLines();
    int dataSize = std::stoi(lines[0])+2;
    int frameSize = std::stoi(lines[1]);

    lines.erase(lines.begin() + 2);
    
    std::cout << dataSize-2 << " Lineas procesadas..." << std::endl;
    if(dataSize == lines.size()){
        std::cout << "Todas las lineas procesadas con exito..." << std::endl;
    } else {
        std::cerr << "ERROR DE PROCESAMIENTO!" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Executando conversion..." << std::endl;
    

    std::vector<std::vector<double>> DataMtx(dataSize, std::vector<double>(frameSize,0.0));
    

    for(int i = 0; i < dataSize; i++){
        parser.ConvertRow(lines[i], ',');
        std::vector<double> parsedRow = parser.getParsedRow();
        for(int j = 0; j < frameSize; j++){
            DataMtx[i][j] = parsedRow[j];
        }
    }

    DataMtx.erase(DataMtx.begin(), DataMtx.begin() + 2);

    dataSize -= 2;

    

    std::cout << "Comprobando datos..." << std::endl;

    

    std::cout << std::setw(((cols - std::string("Desea generar un reporte en caso de datos no legibles? [Y/n]: ").size())/2)+1) << std::setfill(' ') << "\0" << "Desea generar un reporte en caso de datos no legibles? [Y/n]: ";
    char sel;
    std::cin >> sel;
    
    if(std::tolower(sel) == 'n'){
        FileSalvorNR salvor;
        std::ofstream outputFile("./output/" + ReqFile);
        outputFile << dataSize << "\n";
        outputFile << frameSize << "\n";
        outputFile << "2\n";
        
        for(int i = 0; i < dataSize; i++){
            for(int j = 0; j < frameSize; j++){
                if(DataMtx[i][j] == static_cast<double>(-65535)) {
                    salvor.SetDataStatus(true);
                }
                std::cout << DataMtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        if(!salvor.GetDataStatus()){
            std::cout << "Datos no reconocidos en el archivo!, Corrigiendo" << std::endl;

            salvor.DataSet(DataMtx);

            std::cout << "Datos Corregidos: " << std::endl;
            for(int i = 0; i < DataMtx.size(); i++){
                for(int j = 0; j < DataMtx[0].size(); j++){
                    std::cout << DataMtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
                    outputFile << DataMtx[i][j] << (j == frameSize-1 ? "" : ",");
                }
                outputFile << "\n";
                std::cout << std::endl;
            }
            std::cout << std::endl;

            outputFile.close();
        } else {
            for(int i = 0; i < DataMtx.size(); i++){
                for(int j = 0; j < DataMtx[0].size(); j++){
                    std::cout << DataMtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
                    outputFile << DataMtx[i][j] << (j == frameSize-1 ? "" : ",");
                }
                outputFile << "\n";
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << "Datos correctos..." << std::endl;
            outputFile.close();
        }
    } else {
        FileSalvorWR salvor;
        std::string RepFile;
        std::cout << std::setw(((cols - std::string("Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ").size())/2)-64) << std::setfill(' ') << "\0" << "Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ";
        std::cin >> RepFile;


        std::ofstream outputFile("./output/" + ReqFile);
        outputFile << dataSize << "\n";
        outputFile << frameSize << "\n";
        outputFile << "2\n";
        
        for(int i = 0; i < dataSize; i++){
            for(int j = 0; j < frameSize; j++){
                if(DataMtx[i][j] == static_cast<double>(-65535)) {
                    salvor.SetDataStatus(true);
                }
                std::cout << DataMtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        if(salvor.GetDataStatus()){
            std::cout << "Datos no reconocidos en el archivo!, Corrigiendo" << std::endl;

            if(!RepFile.empty()){
                salvor.DataSet(DataMtx, "./output/" + RepFile);
            } else {
                salvor.DataSet(DataMtx);
            }
            

            std::cout << "Datos Corregidos: " << std::endl;
            for(int i = 0; i < DataMtx.size(); i++){
                for(int j = 0; j < DataMtx[0].size(); j++){
                    std::cout << DataMtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
                    outputFile << DataMtx[i][j] << (j == frameSize-1 ? "" : ",");
                }
                outputFile << "\n";
                std::cout << std::endl;
            }
            std::cout << std::endl;

            outputFile.close();
        } else {
            std::cout << "Datos correctos..." << std::endl;
            for(int i = 0; i < DataMtx.size(); i++){
                for(int j = 0; j < DataMtx[0].size(); j++){
                    std::cout << DataMtx[i][j] << (j == frameSize-1 ? "\t" : ",\t");
                    outputFile << DataMtx[i][j] << (j == frameSize-1 ? "" : ",");
                }
                outputFile << "\n";
                std::cout << std::endl;
            }
            std::cout << std::endl;

            outputFile.close();
        }
    }

    return 0;
    
}