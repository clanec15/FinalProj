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

#ifndef MATRIXOPS_HPP_
#define MATRIXOPS_HPP_
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <climits>

#include "../Source/FileReaderSrc.cpp"
#include "../Source/RowParserSrc.cpp"
#include "../Source/FileSalvorSrc.cpp"
#include "./TUIController.hpp"

namespace fs = std::filesystem;

/**
 * @fn void fileSearching(fs::path input, std::vector<fs::path>& fileArray)
 * @brief recursive file searching subroutine
 * 
 * Will search for files inside the defined input path and recursively search in case there are directories
 * 
 * @param input         the entry path
 * @param fileArray     the file array that will contain the paths of the files
 */
void fileSearching(fs::path input, std::vector<fs::path>& fileArray)
{
    const fs::path inputDir = input;

    if(!fs::exists(input)){
        std::cerr << "INVINPUTDIR" << std::endl;
        return;
    }

    for(auto const& dir_entry : fs::directory_iterator(inputDir)){
        if(dir_entry.is_directory()){
            fileSearching(dir_entry, fileArray);
        } else {
            fileArray.push_back(dir_entry.path().relative_path());
        }
    }
}


/**
 * struct to hold the data from the parsed matrix
 */
struct MatrixData
{
    /**Size of the matrix as rows */
	int dataSize;
    /**Size of the rows (Data Frame) */
	int frameSize; 

    /**Input filename */
	std::string fileName; 

    /**The Data Matrix */
	std::vector<std::vector<double>> Matrix; 
};


/**
 * 
 * @fn bool CheckMtx(FileSalvor& helper, MatrixData DataMatrix)
 * @brief Check matrix for NaN values
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
 * @see SetDataStatus
 */
bool CheckMtx(FileSalvor& helper, MatrixData DataMatrix)
{
    for(int i = 0; i < DataMatrix.dataSize; i++){
            for(int j = 0; j < DataMatrix.frameSize; j++){
                if(DataMatrix.Matrix[i][j] == static_cast<double>(-65535)) {
                    return true;
                }
            }
        }
    return false;
}


/**
 * @fn void SaveNOutputMtx(int frameSize, std::ofstream& file, std::vector<std::vector<double>> DataMatrix)
 * @brief Saves the output of the Data Matrix to the specified output file and prints the output to stdout
 * 
 * @param frameSize the size of the rows of the Data Matrix (frame size)
 * @param file output file ofstream
 * @param mtx the Data Matrix
 */
void SaveNOutputMtx(int frameSize, std::ofstream& file, std::vector<std::vector<double>> DataMatrix)
{
    for(int i = 0; i < DataMatrix.size(); i++){
        for(int j = 0; j < DataMatrix[0].size(); j++){
            std::cout << DataMatrix[i][j] << (j == frameSize-1 ? "\t" : ",\t");
            file << DataMatrix[i][j] << (j == frameSize-1 ? "" : ",");
        }
        file << "\n";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @fn                      void MatrixProcessing(bool ProcType, MatrixData& DataMatrix, fs::path outputFileName, int cols, std::vector<std::vector<double>> means)
 * @brief                   processes the matrix and fixes it in case it has NaN values
 * @param ProcType          Sets if the user wants report or not
 * @param DataMatrix        The MatrixData struct for the main matrix data
 * @param outputFileName    The output filename (generally the original one)
 * @param cols              The columns of the terminal (for formatting purposes)
 */
void MatrixProcessing(bool ProcType, MatrixData& DataMatrix, fs::path outputFileName, int cols, std::vector<std::vector<double>> means)
{
    FileSalvor salvorMain;
    std::vector<int> ids = salvorMain.GetMatrixIDs(DataMatrix.Matrix);
    

    std::ofstream outputFile("./output/" + outputFileName.filename().string());
    if(!outputFile.is_open() || !fs::exists("./output")){
        std::cerr << "No se pudo generar archivo!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    outputFile << DataMatrix.dataSize << "\n";
    outputFile << DataMatrix.frameSize << "\n";
    outputFile << ids.size() << "\n";
    salvorMain.SetDataStatus(CheckMtx(salvorMain, DataMatrix));

    if(salvorMain.GetDataStatus()){
        std::cout << "Datos no reconocidos en el archivo!, Corrigiendo" << std::endl;
        std::this_thread::sleep_for(2000ms);
        if(!ProcType){
            CleanTerminal();
            std::string RepFile;
            std::cout << std::setw(((cols - std::string("Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ").size())/2)-64) << std::setfill(' ') << "\0" << "Coloque el nombre del archivo de reporte (Predeterminado: ./output/NaNReport.txt): ";

            getline(std::cin, RepFile);
            if(RepFile.empty()){
                RepFile = "./output/NaNReport.txt";
            }
            FileSalvorWR gen;
            gen.DataSet(DataMatrix.Matrix, means, RepFile);
        } else {
            FileSalvorNR gen;
            gen.DataSet(DataMatrix.Matrix, means);
        }
        
        std::cout << "Datos Corregidos: " << std::endl;
        SaveNOutputMtx(DataMatrix.frameSize, outputFile, DataMatrix.Matrix);
        outputFile.close();
    } else {
        std::cout << "Datos correctos..." << std::endl;
        SaveNOutputMtx(DataMatrix.frameSize, outputFile, DataMatrix.Matrix);
        outputFile.close();
    }
}



/**
 * @fn MatrixData fileReading(int cols, std::vector<fs::path>& fileArr)
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
            std::exit(EXIT_FAILURE);
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
                callout = "El archivo se encuentra dañado, desea seleccionar otro? [Y/n]: ";
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
                    std::exit(EXIT_SUCCESS);
                }
            }
        }
    }


    

	reader.setInputFile(selFile.string());
    std::vector<std::string> lines = reader.getLines();
	int dataSize = output.dataSize = std::stoi(lines[0]), frameSize = output.frameSize = std::stoi(lines[1]);

    for(int i = 3; i > 0; i--){
        lines.erase(lines.begin());
    }
    
    std::cout << output.dataSize << " Lineas procesadas..." << std::endl;
    
    if(output.dataSize == lines.size()){
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

    std::cout << "Conversion completada..." << std::endl;
	output.Matrix = Matrix;
	return output;
}

#endif