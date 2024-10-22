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
        if(!(inputMtx[i][inputMtx[0].size()-1] == static_cast<double>(id))){
            continue;
        } else {
            mean += inputMtx[i][col];
            items++;
        }
        
    }
    
    return mean/items;
}


int FileSalvor::typeCalc(std::vector<std::vector<double>> inputMtx, int Col)
{
    std::vector<std::string> types;

    for(int i = 0; i < inputMtx.size(); i++){
        if(inputMtx[i][Col] == static_cast<double>(-65535)){
            continue;
        }
        if(std::floor(inputMtx[i][Col]) == inputMtx[i][Col]){
            types.push_back("int");
        } else {
            types.push_back("dbl");
        }
    }

    std::cout << std::endl;

    int dblQty = 0, intQty = 0;

    for(int i = 0; i < types.size(); i++){
        if(types[i] == "int"){
            intQty++;
        } else {
            dblQty++;
        }
    }

    if(intQty > dblQty){
        return 1;
    } else if(intQty < dblQty){
        return 2;
    }

    return -1;
}

/*
std::vector<int> idFinder(std::vector<std::vector<double>> mtx)
{
    std::vector<int> ids = {};
    for(int i = 0; i < mtx.size(); i++){
        auto it = std::find(ids.begin(), ids.end(), mtx[i][mtx[0].size()-1]);

        if(it != ids.end()){
            continue;
        } else {
            ids.push_back(mtx[i][mtx[0].size()-1]);
        }
    }

    return ids;
}
*/

std::vector<int> FileSalvor::idFinder(std::vector<std::vector<double>> inputMtx){
    std::vector<int> ids = {};
    for(int i = 0; i < inputMtx.size(); i++){
        auto it = std::find(ids.begin(), ids.end(), inputMtx[i][inputMtx[0].size()-1]);

        if(it != ids.end()){
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

void FileSalvorNR::DataSet(std::vector<std::vector<double>>& inputMtx)
{
    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < inputMtx[0].size(); j++){
            if(inputMtx[i][j] == static_cast<double>(-65535)){
                int modeDataType = typeCalc(inputMtx, j);
                double datSalvage = DataSalvage(inputMtx, j, inputMtx.size()-1);
                if(modeDataType == 1){
                    inputMtx[i].at(j) = (int)datSalvage;
                } else {
                    inputMtx[i].at(j) = datSalvage;
                }
            }
        }
    }
}

void FileSalvorWR::DataSet(std::vector<std::vector<double>>& inputMtx, std::string OutputFile="./output/NaNReport.txt"){
    std::ofstream ReportFile(OutputFile);
    for(int i = 0; i < inputMtx.size(); i++){
        for(int j = 0; j < inputMtx[0].size(); j++){
            if(inputMtx[i][j] == static_cast<double>(-65535)){
                
                int modeDataType = typeCalc(inputMtx, j);
                double datSalvage = DataSalvage(inputMtx, j, inputMtx.size()-1);

                ReportFile << "Dato corregido en: (" << i << ", " << j << ")" << " por el dato: " << datSalvage << " De tipo: " << (modeDataType == 1 ? "Entero\n" : "Flotante\n");

                if(modeDataType == 1){
                    inputMtx[i].at(j) = (int)datSalvage;
                } else {
                    inputMtx[i].at(j) = datSalvage;
                }

            }
        }
    }
    ReportFile.close();




}




