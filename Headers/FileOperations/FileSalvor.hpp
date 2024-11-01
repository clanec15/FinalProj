/**
 * @file FileSalvor.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Class for missing data calculation and recovery
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <sstream>

/**
 * @brief Main class
 * 
 */
class FileSalvor{
    public:

    FileSalvor();
    ~FileSalvor();
    std::vector<std::vector<double>> DataMeanCalculation(std::vector<std::vector<double>>&);
    bool GetDataStatus();
    void SetDataStatus(bool);
    double DataSalvage(std::vector<std::vector<double>>&, int, int);


    
    
    private:
    bool invDataDtc;
    int modeDtyp;
    std::vector<int> idFinder(std::vector<std::vector<double>>&);
};

/**
 * @brief No data change logging
 * 
 */
class FileSalvorNR : public FileSalvor{

    public:
    void DataSet(std::vector<std::vector<double>>&, std::vector<std::vector<double>>);
};


/**
 * @brief w/Data Change logging
 * 
 */
class FileSalvorWR : public FileSalvor{
    public:
    void DataSet(std::vector<std::vector<double>>&, std::vector<std::vector<double>>, std::string);   

};