#include "../Headers/FileSalvor.hpp"



FileSalvor::FileSalvor(){invDataDtc = false; modeDtyp = 0;}
FileSalvor::~FileSalvor(){}

bool FileSalvor::GetDataStatus()
{
    return invDataDtc;
}

void FileSalvor::SetDataStatus(bool status)
{
    invDataDtc = status;
}

/*
Calculates the new value of a broken one using the mean of the column and the id type of it



std::vector<std::vector<double>> inputMtx = matrix to salvage the data from
int col = column to operate on
int id = to operate on the rows with the same id
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


std::vector<std::vector<double>> FileSalvor::DataMeanCalculation(std::vector<std::vector<double>> inputMtx)
{
    std::vector<int> ids = FileSalvor::idFinder(inputMtx);
    std::vector<std::vector<double>> means(ids.size());
    FileSalvor hlp;

    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < ids.size(); j++){
            means[j].push_back(hlp.DataSalvage(inputMtx, i, ids[j]));
        }
    }

    return means;
}

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

void FileSalvorWR::DataSet(std::vector<std::vector<double>>& inputMtx, std::vector<std::vector<double>> data, std::string OutputFile="./output/NaNReport.txt"){
    if(data.size() > 2){
        data.pop_back();
    }
    std::ofstream ReportFile(OutputFile);
    ReportFile << "Datos de reemplazo: \n";
    for(int i = 0; i < data.size(); i++){
        ReportFile << "ID[" << i << "]: ";

        for(int j = 0; j < data[0].size(); j++){
            ReportFile << data[i][j] << ", ";
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