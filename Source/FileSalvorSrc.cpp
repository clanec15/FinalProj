#include "../Headers/FileOperations/FileSalvor.hpp"



FileSalvor::FileSalvor(){invDataDtc = false; modeDtyp = 0;}
FileSalvor::~FileSalvor(){}


/**
 * Get the DataStatus flag
 * 
 * @return The DataStatus flag as bool
 */
bool FileSalvor::GetDataStatus()
{
    return invDataDtc;
}

/**
 * Sets the DataStatus flag
 * 
 * @param status The status to set the flag (True | False)
 */
void FileSalvor::SetDataStatus(bool status)
{
    invDataDtc = status;
}

/**
 * Calculates the mean of all elements of a row based on the id (last value of the row)
 * 
 * @param inputMtx The input Data Matrix
 * @param col The column to calculate the mean
 * @param id the id of the row
 * 
 * @return the mean of the rows with the same id
 */
double FileSalvor::DataSalvage(std::vector<std::vector<double>> inputMtx, int col, int id){
    double mean;
    int items = 0;
    for(int i = 0; i<inputMtx.size(); i++){
        if((inputMtx[i][inputMtx[0].size()-1] != static_cast<double>(id))){
            continue;
        } else {
            if((inputMtx[i][col] != static_cast<double>(-65535))){
                mean += inputMtx[i][col];
                items++;
            } else {
                continue;
            }
            
        }
        
    }
    
    return mean/items;
}

/**
 * Finds all the ids of the Data Matrix
 * @param inputMtx the input Data Matrix
 * 
 * @return the ids as a vector of int's
 */
std::vector<int> FileSalvor::idFinder(std::vector<std::vector<double>> inputMtx){
    std::vector<int> ids = {};
    for(int i = 0; i < inputMtx.size(); i++){
        int test = std::count(ids.begin(), ids.end(), inputMtx[i][inputMtx[0].size()-1]);

        if(test > 0){
            continue;
        } else {
            ids.push_back(inputMtx[i][inputMtx[0].size()-1]);
        }
    }

    return ids;
}

/**
 * calculates all the means of the Data Matrix
 * 
 * @param inputMtx the input Data Matrix
 * 
 * @returns the means as a matrix with the rows being the id's
 * 
 * @see idFinder
 * @see DataSalvage
 */
std::vector<std::vector<double>> FileSalvor::DataMeanCalculation(std::vector<std::vector<double>> inputMtx)
{
    std::vector<int> ids = FileSalvor::idFinder(inputMtx);
    std::vector<std::vector<double>> means(ids.size());
    FileSalvor hlp;

    for(int i = 0; i < inputMtx[0].size()-1; i++){
        for(int j = 0; j < ids.size(); j++){
            //Truncate to 2 decimal places using round
            means[j].push_back(round(hlp.DataSalvage(inputMtx, i, ids[j])*100.0)/100.0);
        }
    }

    return means;
}


/**
 * Modifies the NaN values of the data matrix with the values of the means Data Matrix
 * 
 * @param inputMtx The input Data Matrix (as reference)
 * @param data The Mean Data Matrix
 */
void FileSalvorNR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data)
{
    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < inputMtx[0].size(); j++){
            if(inputMtx[i][j] == static_cast<double>(-65535)){
                inputMtx[i][j] = data[inputMtx[j][inputMtx[0].size()-1]][j];
            }
        }
    }
}

/**
 * Modifies the NaN values of the data matrix with the values of the means Data Matrix and logs the changes to a file
 * 
 * @param inputMtx The input Data Matrix (as reference)
 * @param data The Mean Data Matrix
 * @param OutputFile the log (report) filename to output to
 */
void FileSalvorWR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data, std::string OutputFile="./output/NaNReport.txt"){
    if(data.size() > 2){
        data.pop_back();
    }           
    std::ofstream ReportFile(OutputFile);
    ReportFile << "Datos de reemplazo: \n";
    ReportFile << "\t ";
    for(int h = 0; h < data[0].size(); h++){
        ReportFile << "[" << h+1 << "]\t";
    }
    ReportFile << "\n";
    for(int i = 0; i < data.size(); i++){
        ReportFile << "ID[" << i << "]:\t";

        for(int j = 0; j < data[0].size(); j++){
            std::stringstream buf;
            buf << data[i][j];
            ReportFile << buf.str() << (j == data[0].size()-1 ? "\t" : ",\t");
        }

        ReportFile << "\n\n";
    }
    
    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < inputMtx[0].size(); j++){
            if(inputMtx[i][j] == static_cast<double>(-65535)){
                ReportFile << "Dato corregido en: (" << i << ", " << j << ")" << " por el dato: " << data[inputMtx[j][inputMtx[0].size()-1]][j] << "\n";
                inputMtx[i][j] = data[inputMtx[j][inputMtx[0].size()-1]][j];
            }
        }
    }
    
    ReportFile.close();
}