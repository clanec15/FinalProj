//Victor Aguilar Rodriguez
//202341753
//Compilar usando el comando make


//System libraries
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <thread>
namespace fs = std::filesystem;
using namespace std::chrono_literals;

//--------------------------------------//

//Custom libraries
#include "Source/SimilCalcSrc.cpp"
#include "Headers/TUIController.hpp"
#include "Headers/MatrixOps.hpp"

//-------------------------------------//

MatrixData fileReading(int cols)
{
	MatrixData output;
	MatrixData ErrorOut;


	ErrorOut.dataSize = -65535;
	ErrorOut.frameSize = -65535;
	ErrorOut.Matrix = std::vector<std::vector<double>>();

	FileReader reader;
    RowParser parser;
    std::string ReqFile, fullFile;
    std::string inputPath = "./input";



    //File Reading
    while(true){
        std::cout << std::setw(((cols - std::string("Coloque el nombre del archivo: ").size())/2)) << std::setfill(' ') << "\0" << "Coloque el nombre del archivo: ";
        std::cin >> ReqFile;
		output.fileName = ReqFile;
        fullFile = "./input/" + ReqFile;

        std::ifstream file(fullFile);
        if(file.good()){
            break;
        } else {
            std::cout << std::setw(((cols - std::string("El archivo no se ha encontrado o esta dañado, desea volver a colocarlo? [Y/n]: ").size())/2)-64) << std::setfill(' ') << "\0" << "El archivo no se ha encontrado o esta dañado, desea volver a colocarlo? [Y/n]: ";
            char sel;
            std::cin >> sel;
            if(std::toupper(sel) == 'Y'){
                CleanTerminal();
                continue;
            } else {
                std::cerr << "Usuario decidio terminar el programa..." << std::endl;
                std::system("PAUSE");
				return ErrorOut;
            }
        }
    }

	reader.setInputFile(fullFile);
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
	output.Matrix = Matrix;
	return output;
}

int main()
{

    if((!fs::exists("./input") || fs::is_empty("./input")) || !fs::exists("./output")){
        std::cout << "La carpeta de entrada/salida (input/output) no existe o esta vacia\nPor favor cree la carpeta y coloque archivos en ella para utilizar el programa" << std::endl;
        return EXIT_FAILURE;
    }

    CleanTerminal();
	int cols = GetTerminalSize().columns;
	MatrixData first = fileReading(cols);

    if((first.dataSize == -65535 || first.frameSize == -65535) && first.Matrix.size() == 0){
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
    
    if(std::tolower(sel) == 'n'){
        mtxProcessingNR(dataSize, frameSize, firstMtx, file);
    } else {
        mtxProcessingWR(cols, dataSize, frameSize, firstMtx, file);
    }


	std::cout << "Comenzando proceso de calculo de similitud" << std::endl;
	std::this_thread::sleep_for(5000ms);

    MatrixData second = fileReading(cols);

    if((second.dataSize == -65535 || second.frameSize == -65535) && second.Matrix.size() == 0){
        std::cerr << "LECTURA INCORRECTA" << std::endl;
        return EXIT_FAILURE;
    }

    int dt = second.dataSize-2;
    int frs = second.frameSize;
    std::vector<std::vector<double>> secondMtx = second.Matrix;
    std::string fl = second.fileName;
    std::cout << "Usando modo de 'No Reporte' para la computacion rapida de similitud" << std::endl;
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