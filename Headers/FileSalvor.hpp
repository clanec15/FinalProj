#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>



class FileSalvor{

    
    
    
    public:

    FileSalvor();
    ~FileSalvor();
    int typeCalc(std::vector<std::vector<double>>, int);
    std::vector<std::vector<double>> DataMeanCalculation(std::vector<std::vector<double>>);
    bool GetDataStatus();
    void SetDataStatus(bool);
    double DataSalvage(std::vector<std::vector<double>>, int, int);


    
    
    private:
    bool invDataDtc;
    int modeDtyp;
    std::vector<int> idFinder(std::vector<std::vector<double>>);
};


class FileSalvorNR : public FileSalvor{

    public:
    void DataSet(std::vector<std::vector<double>>&, std::vector<std::vector<double>>);
};

class FileSalvorWR : public FileSalvor{
    public:
    void DataSet(std::vector<std::vector<double>>&, std::vector<std::vector<double>>, std::string);   

};