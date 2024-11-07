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
    virtual void diffCalc() = 0;
};


class SimilCalcAbs : public SimilCalc{

    public:

    struct diffData
    {
        int idx;
        double diff;
    };

    void SetFirstMtx(const std::vector<std::vector<double>>&);
    void setSecondMtx(const std::vector<std::vector<double>>&);
    void diffCalc(int);
    diffData getMostSimilarRow();

    private:
    
    diffData BestChoice;
    
    void szCalc();
    void SimilCalculation();
    std::vector<diffData> rowDiffs;
    int sz;

    protected:
    std::vector<std::vector<double>> FirstMtx;
    std::vector<std::vector<double>> SecMtx;



};

class SimilCalcBayesian : public SimilCalcAbs{
    public:
    struct ProbabilityData
    {
        int idx;
        double prob;
    };

    ProbabilityData getMostProbableRow();
    std::vector<ProbabilityData> diffCalc(std::vector<int>, std::vector<std::vector<double>>);

    private:
    std::vector<ProbabilityData> ProbabilityVector;
    ProbabilityData BestChoiceProb;
    void SimilCalculationProb();

};