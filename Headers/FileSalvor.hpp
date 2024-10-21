#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>



class FileSalvor{

    
    
    
    public:

    struct DataPrecalculation{
        std::vector<double> meansA;
        std::vector<double> meansB;
        
    };

    FileSalvor();
    ~FileSalvor();
    int typeCalc(std::vector<std::vector<double>>, int);
    DataPrecalculation DataMeanCalculation(std::vector<std::vector<double>>);
    bool GetDataStatus();
    void SetDataStatus(bool);
    double DataSalvage(std::vector<std::vector<double>>, int, int);

    
    
    private:
    bool invDataDtc;
    int modeDtyp;
};


class FileSalvorNR : public FileSalvor{

    public:
    void DataSet(std::vector<std::vector<double>>&);
};

class FileSalvorWR : public FileSalvor{
    public:
    void DataSet(std::vector<std::vector<double>>&, std::string);   

};