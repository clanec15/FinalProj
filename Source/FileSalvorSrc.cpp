#include "../Headers/FileOperations/FileSalvor.hpp"

/**Constructor */
FileSalvor::FileSalvor(){invDataDtc = false;} 

const double MISS_DATA = -65535.0; //Missing Data Placeholder


/**
 * @fn bool FileSalvor::GetDataStatus()
 * @brief Get the DataStatus flag
 * 
 * @return The DataStatus flag as bool
 */
bool FileSalvor::GetDataStatus()
{
    return invDataDtc;
}

/**
 * @fn void FileSalvor::SetDataStatus(bool status)
 * @brief Sets the DataStatus flag
 * 
 * @param status The status to set the flag (True | False)
 */
void FileSalvor::SetDataStatus(bool status)
{
    invDataDtc = status;
}

/**
 * @fn double FileSalvor::DataSalvage(std::vector<std::vector<double>>& inputMtx, int col, int id)
 * @brief Calculates the mean of all elements of a row based on the id (last value of the row)
 * 
 * @param inputMtx The input Data Matrix
 * @param col The column to calculate the mean
 * @param id the id of the row
 * 
 * @return the mean of the rows with the same id
 */
double FileSalvor::DataSalvage(std::vector<std::vector<double>>& inputMtx, int col, int id){
    double mean = 0.0;
    int items = 0;
    for(int i = 0; i<inputMtx.size(); i++){
        if((inputMtx[i][inputMtx[0].size()-1] != static_cast<double>(id)) || (inputMtx[i][col] == static_cast<double>(MISS_DATA))){
            continue;
        } else {
            mean += inputMtx[i][col];
            items++;
        }
    }
    
    return (items > 0 ? mean/(double)items : 0.0);
}


/**
 * @fn std::vector<std::vector<double>> MatrixTrans(std::vector<std::vector<double>> Matrix)
 * @brief Gets the medians of a column of the input matrix
 * 
 * It also checks if the resulting column is odd or even
 * 
 * @param InputMtx the input matrix
 * @param col The desired column
 * @param id The ID to exclude entries
 * 
 * @returns The median of the desired column
 * 
 */
std::vector<std::vector<double>> MatrixTrans(std::vector<std::vector<double>> Matrix){

    size_t Rows = Matrix.size();
    size_t Cols = Matrix[0].size();

    std::vector<std::vector<double>> AuxMat(Cols, std::vector<double>(Rows, 0));

    for (size_t i = 0; i < Cols; i++)
    {
        for(size_t j = 0; j < Rows; j++){
            AuxMat[i][j] = Matrix[j][i];
        }
    }

    return AuxMat;
}

/**
 * @fn double FileSalvor::GetMedian(std::vector<std::vector<double>>& InputMtx, int col, int id)
 * @brief Gets the medians of a column of the input matrix
 * 
 * It also checks if the resulting column is odd or even
 * 
 * @param InputMtx the input matrix
 * @param col The desired column
 * @param id The ID to exclude entries
 * 
 * @returns The median of the desired column
 * 
 */
double FileSalvor::GetMedian(std::vector<std::vector<double>>& InputMtx, int col, int id)
{
    std::vector<double> buf;
    std::vector<std::vector<double>> TransMatrix = MatrixTrans(InputMtx);

    for(int i = 0; i < TransMatrix[0].size(); i++){
        if(TransMatrix[TransMatrix.size()-1][i] == id){
            buf.push_back(TransMatrix[col][i]);
        }
    }

    std::sort(buf.begin(), buf.end());


    int sz = buf.size();

    if(sz%2 == 0){
        return (buf[(sz/2) + 1] + buf[sz/2])/2.0;
    } else {
        return buf[(sz+1)/2];
    }

    return -1;
}



/**
 * @fn std::vector<int> FileSalvor::idFinder(std::vector<std::vector<double>>& inputMtx)
 * @brief Finds all the ids of the Data Matrix
 * @param inputMtx the input Data Matrix
 * 
 * @return the ids as a vector of int's
 */
std::vector<int> FileSalvor::idFinder(std::vector<std::vector<double>>& inputMtx){
    std::vector<int> ids = {};
    int currentId = inputMtx[0].size()-1;

    

    for(int i = 0; i < inputMtx.size(); i++){

        if(inputMtx[i][currentId] == MISS_DATA){
            std::cerr << "ID DAÑADA, REEMPLAZANDO" << std::endl;
            inputMtx[i][currentId] = 0;
        }

        int counts = std::count(ids.begin(), ids.end(), inputMtx[i][currentId]);

        if(counts > 0){
            continue;
        } else {
            ids.push_back(inputMtx[i][currentId]);
        }
    }

    return ids;
}


/**
 * @fn std::vector<int> FileSalvor::GetMatrixIDs(std::vector<std::vector<double>>& inputMtx)
 * @brief Gets the ID's of a Data Matrix for future usage
 * @param inputMtx The input matrix
 * 
 * @returns The ID's in a vector
 */
std::vector<int> FileSalvor::GetMatrixIDs(std::vector<std::vector<double>>& inputMtx)
{
    return idFinder(inputMtx);
}

/**
 * 
 * @fn double truncate(int n_places, double num)
 * @brief truncates a double value to n places
 * @param n_places The number of decimal digits to truncate to
 * @param num The number to truncate
 * 
 * @return the truncated value
 */
double truncate(int n_places, double num){
    double trunc = 10;
    if(n_places == 0){
        trunc = 1;
    } else {
        trunc = pow(trunc, n_places);
    }
    return round(num*trunc)/trunc;
}

/**
 * @fn std::vector<std::vector<double>> FileSalvor::DataMeanCalculation(std::vector<std::vector<double>>& inputMtx)
 * @brief calculates all the means of the Data Matrix
 * 
 * @param inputMtx the input Data Matrix
 * 
 * @returns the means as a matrix with the rows being the id's
 * 
 * @see idFinder
 * @see DataSalvage
 */
std::vector<std::vector<double>> FileSalvor::DataMeanCalculation(std::vector<std::vector<double>>& inputMtx)
{
    std::vector<int> ids = FileSalvor::idFinder(inputMtx);
    std::vector<std::vector<double>> means(ids.size());
    if(inputMtx.empty() || inputMtx[0].empty()){
        return {};
    }

    for(int i = 0; i < inputMtx[0].size()-1; i++){
        for(int j = 0; j < ids.size(); j++){
            means[j].push_back(truncate(2, DataSalvage(inputMtx, i, ids[j])));
        }
    }

    return means;
}


/**
 * @fn std::vector<std::vector<double>> FileSalvor::DataMeanCalculation(std::vector<std::vector<double>>& inputMtx)
 * @brief calculates all the medians of the Data Matrix
 * 
 * @param inputMtx the input Data Matrix
 * 
 * @returns the medians as a matrix with the rows being the id's
 * 
 * @see idFinder
 * @see GetMedian
 */
std::vector<std::vector<double>> FileSalvor::DataMedianCalculatuion(std::vector<std::vector<double>>& inputMtx)
{
    std::vector<int> ids = FileSalvor::idFinder(inputMtx);
    std::vector<std::vector<double>> medians(ids.size());

    if(inputMtx.empty() || inputMtx[0].empty()){
        return {};
    }

    if(ids.size() != 1){
      for(int i = 0; i < inputMtx[0].size()-1; i++){
            for(int j = 0; j < ids.size();j++){
                medians[j].push_back(truncate(2, GetMedian(inputMtx, i, ids[j])));
            }
        }  
    } else {
    	for(int i = 0; i < inputMtx[0].size()-1; i++){
			for(int j = 0; j < ids.size();j++){
				medians[j].push_back(truncate(2, GetMedian(inputMtx, i, ids[0])));
			}
		}
    }

    return medians;
}


/**
 * @fn void FileSalvorNR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data)
 * @brief Modifies the NaN values of the data matrix with the values of the means Data Matrix
 * 
 * @param inputMtx The input Data Matrix (as reference)
 * @param data The Mean Data Matrix
 */
void FileSalvorNR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data)
{

    if(data.size() > 2){
        data.pop_back();
    }

    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < inputMtx[0].size(); j++){
            if(inputMtx[i][j] == MISS_DATA){
                int entry = inputMtx[j][inputMtx[0].size()-1];
                inputMtx[i][j] = data[entry][j];
            }
        }
    }
}

/**
 * @fn void FileSalvorWR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data, std::string OutputFile="./output/NaNReport.txt")
 * @brief Modifies the NaN values of the data matrix with the values of the means Data Matrix and logs the changes to a file
 * 
 * @param inputMtx      The input Data Matrix (as reference)
 * @param data          The Mean Data Matrix
 * @param OutputFile    the log (report) filename to output to
 */
void FileSalvorWR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data, std::string OutputFile="./output/NaNReport.txt"){
    int failedItems = 0;
    
    if(data.size() > 2){
        data.pop_back();
    }
    std::ofstream ReportFile(OutputFile);

    if(ReportFile.fail() || !ReportFile.is_open()){
        std::cerr << "CREACION DE ARCHIVO DE REPORTE FALLIDA [FileSalvorSrc.cpp]" << std::endl;
        std::abort();
    }


    ReportFile << "Datos de reemplazo: \n";
    ReportFile << "\t";
    for(int h = 0; h < data[0].size(); h++){
        ReportFile << "[" << h+1 << "]\t";
    }
    ReportFile << "\n";
    for(int i = 0; i < data.size(); i++){
        ReportFile << "ID[" << i << "]:\t";

        for(int j = 0; j < data[0].size(); j++){
            ReportFile << data[i][j] << (j == data[0].size()-1 ? "\t" : ",\t");
        }

        ReportFile << "\n\n";
    }
    
    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < inputMtx[0].size(); j++){
            if(inputMtx[i][j] == MISS_DATA){
                ReportFile << "Dato corregido en: (" << i << ", " << j << ")" << " por el dato: " << data[inputMtx[j][inputMtx[0].size()-1]][j] << "\n";
                std::cout << "Dato corregido en: (" << i << ", " << j << ")" << " por el dato: " << data[inputMtx[j][inputMtx[0].size()-1]][j] << "\n";
                inputMtx[i][j] = data[inputMtx[j][inputMtx[0].size()-1]][j];
                failedItems++;
            }
        }
    }

    ReportFile << "Datos no reconocidos: " << failedItems << "\n";
    ReportFile << "Porcentaje de la matriz dañada: " << ((double)failedItems/(inputMtx.size()*inputMtx[0].size()))*100 << "%" << "\n";
    
    ReportFile.close();
}