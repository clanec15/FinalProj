/**
 * @file MatrixOps.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Header for operations with the data matrices
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MATRIXOPS_HPP
#define MATRIXOPS_HPP
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <climits>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "../Source/FileReaderSrc.cpp"
#include "../Source/RowParserSrc.cpp"
#include "../Source/FileSalvorSrc.cpp"
#include "./TUIController.hpp"

namespace fs = std::filesystem;


std::vector<fs::path> fileSearching(fs::path input = "input/")
{
    const fs::path inputDir = input;

    std::vector<fs::path> fileArr;

    for(auto const& dir_entry : fs::directory_iterator(inputDir)){
        fileArr.push_back(dir_entry.path());
    }

    return fileArr;
}


/**
 * struct to hold the data from the parsed matrix
 */
struct MatrixData
{
	int dataSize; /**Size of the matrix as rows */
	int frameSize; /**Size of the rows (Data Frame) */
	std::string fileName; /**Input filename */
	std::vector<std::vector<double>> Matrix; /**The Data Matrix */
};


/**
 * Check matrix for NaN values
 * 
 * It checks if the entry [i][j] is equal to -65535 and in case it is, it sets the DataStatus flag
 * so in the mtxProcessingNR/WR call it rectificates those entries with valid values from the 
 * means data
 * 
 * 
 * @param dataSize the size of the Data Matrix (rows)
 * @param frameSize the size of the rows of the Data Matrix (frame size)
 * @param helper the FileSalvor class to set the flag
 * @param mtx The data Matrix
 * 
 * 
 * @see mtxProccesingNR
 * @see mtxProccesingWR
 * @see SetDataStatus
 */
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


/**
 * Saves the output of the Data Matrix to the specified output file and prints the output to stdout
 * 
 * @param frameSize the size of the rows of the Data Matrix (frame size)
 * @param file output file ofstream
 * @param mtx the Data Matrix
 */
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


/**
 * Processes the matrix in case the DataStatus flag is set, if its not, it just prints the matrix to stdout and saves it to file using the SaveNOutputMtx
 * (No output to report file)
 * 
 * @param dataSize size of the Data Matrix (rows)
 * @param frameSize size of the rows of the Matrix (Data Frame)
 * @param DataMtx The Data Matrix
 * @param outputFName the output filename (Not the report file name)
 * 
 * @see CheckMtx
 * @see GetDataStatus
 * @see DataSet
 * 
 */
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


/**
 * Processes the matrix in case the DataStatus flag is set, if its not, it just prints the matrix to stdout and saves it to file using the SaveNOutputMtx
 * (Outputs to report file)
 * 
 * @param dataSize size of the Data Matrix (rows)
 * @param frameSize size of the rows of the Matrix (Data Frame)
 * @param DataMtx The Data Matrix
 * @param outputFName the output filename (Not the report file name)
 * 
 * @see SaveNOutputMtx
 */
void mtxProcessingWR(int cols, int dataSize, int frameSize, std::vector<std::vector<double>>& DataMtx, std::string outputFName)
{
    FileSalvorWR salvor;
    std::string RepFile;
    std::vector<std::vector<double>> means = salvor.DataMeanCalculation(DataMtx);
    std::cout << std::setw(((cols - std::string("Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ").size())/2)-64) << std::setfill(' ') << "\0" << "Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ";
    getline(std::cin, RepFile);
    if(RepFile.empty()){
        RepFile = " ./output/NaNReport.txt";
    }
    


    std::ofstream outputFile("./output/" + outputFName);
    outputFile << dataSize << "\n";
    outputFile << frameSize << "\n";
    outputFile << "2\n";
        
    CheckMtx(dataSize, frameSize, salvor, DataMtx);

    if(salvor.GetDataStatus()){
        std::cout << "Datos no reconocidos en el archivo!, Corrigiendo" << std::endl;

        salvor.DataSet(DataMtx, means, RepFile);
            

        std::cout << "Datos Corregidos: " << std::endl;
        SaveNOutputMtx(frameSize, outputFile, DataMtx);

        outputFile.close();
    } else {
        SaveNOutputMtx(frameSize, outputFile, DataMtx);
        std::cout << "Datos correctos..." << std::endl;
        outputFile.close();
    }
}

/**
 * @brief Reads the file and outputs it to a MatrixData struct
 * 
 * @param cols the number of columns of the terminal for formatting purposes
 * 
 * It prepares two structs; one for normal output and one for an unexpected input (error) that
 * contains invalid data (NaN and an empty matrix), it then asks the user to input the filename
 * of a file inside the input directory, checks if its non-corrupt, if it is, it asks the user to
 * retry, if the user does not want to retry, it returns the error struct
 * 
 * it then calls a FileReader object to set the filename to be read and it gets the lines
 * as rows of the file
 * 
 * it prepares the matrix size and the data frame size from the first and second lines, and it deletes the third line
 * 
 * outputs to stdout the number of processed lines and check if the number of processed lines equals the size of the
 * lines array, if it doesnt, it outputs an error message and the error struct
 * 
 * it then creates the main output Data Matrix and starts filling it with the parsed data from the rows, it then deletes
 * the first two rows as they are used for the Data Matrix Size and the Data Frame Size
 * 
 * @return The MatrixData struct
 * @see FileReader
 * @see RowParser
 */
MatrixData fileReading(int cols, std::vector<fs::path>& fileArr)
{
	MatrixData output;
	MatrixData ErrorOut;


	ErrorOut.dataSize =     INT_MAX;
	ErrorOut.frameSize =    INT_MAX;
	ErrorOut.Matrix =       std::vector<std::vector<double>>();

	FileReader reader;
    RowParser parser;
    char sel;
    fs::path selFile;


    //File Reading

    if(fileArr.size() < 2){
        std::cout << "Usando unico elemento no seleccionado..." << std::endl;
        selFile = fileArr[0];
        output.fileName = selFile.string();
        std::ifstream file(output.fileName);

        if(file.good()){
            file.close();
        } else {
            std::cout << "Unico archivo restante corrupto/ilegible...\nSuspendiendo ejecucion..." << std::endl;
            return ErrorOut;
        }

    } else {
        while(true){
            std::string callout = "Seleccione el archvio a abrir: ";

            std::cout << std::setw(((cols - callout.size())/2)) << std::setfill(' ') << callout << std::endl;;
            for(int i = 0; i < fileArr.size(); i++){
                std::cout << "[" << char(97+i) << "]: " << fileArr[i].filename().string() << std::endl;
            }

            std::cout << "\n[S]: ";
            std::cin >> sel;

            selFile = fileArr[sel-97];
            output.fileName = selFile.string();

            std::ifstream file(selFile.string());
            if(file.good()){
                file.close();
                fileArr.erase(fileArr.begin() + sel-97);
                break;
            } else {
                callout = "El archivo no se encuentra dañado, desea seleccionar otro? [Y/n]: ";
                std::cout << std::setw(((cols - callout.size())/2)-1) << std::setfill(' ') << "\0" << callout;
                char sel;
                std::cin >> sel;
                if(std::toupper(sel) == 'Y'){
                    CleanTerminal();
                    file.close();
                    continue;
                } else {
                    file.close();
                    std::cerr << "Usuario decidio terminar el programa..." << std::endl;
                    std::system("PAUSE");
                    return ErrorOut;
                }
            }
        }
    }


    

	reader.setInputFile(selFile.string());
    std::vector<std::string> lines = reader.getLines();
	int dataSize = output.dataSize = std::stoi(lines[0]), frameSize = output.frameSize = std::stoi(lines[1]);
    lines.erase(lines.begin() + 2);
    
    std::cout << output.dataSize-2 << " Lineas procesadas..." << std::endl;
    
    if(output.dataSize == lines.size()-2){
        std::cout << "Todas las lineas procesadas con exito..." << std::endl;
    } else {
        std::cerr << "ERROR DE PROCESAMIENTO!" << std::endl;
        return ErrorOut;
    }
    std::cout << "Executando conversion..." << std::endl;

	std::vector<std::vector<double>> Matrix(output.dataSize, std::vector<double>(output.frameSize,0.0));
    
    //Parse rows
    for(int i = 0; i < dataSize; i++){
        parser.ConvertRow(lines[i], ',');
        std::vector<double> parsedRow = parser.getParsedRow();
        for(int j = 0; j < frameSize; j++){
            Matrix[i][j] = parsedRow[j];
        }
    }

    Matrix.erase(Matrix.begin(), Matrix.begin() + 2);

    std::cout << "Conversion completada..." << std::endl;
	output.Matrix = Matrix;
	return output;
}

#endif