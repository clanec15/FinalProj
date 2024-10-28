/**
 * @file SimiCalc.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Class for similitude calculation between two matrices
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <vector>
#include <cmath>
#include <iostream>

class SimilCalc{

    public:

    struct diffData
    {
        int idx;
        double diff;
    };

    SimilCalc();
    ~SimilCalc();
    void SetFstMtx(std::vector<std::vector<double>>&);
    void setSecMtx(std::vector<std::vector<double>>&);
    void diffCalc(int);
    diffData getMSimRow();
    
    

    

    private:

    std::vector<std::vector<double>> FirstMtx;
    std::vector<std::vector<double>> SecMtx;
    diffData BestChoice;
    void szCalc();
    void SimilCalculation();
    std::vector<diffData> rowDiffs;
    int sz;


};