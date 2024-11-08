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
    using SimilCalc::diffCalc;
    void diffCalc() override{};
    void diffCalc(int);
    diffData getMostSimilarRow();
    std::vector<diffData> getOutput();



    private:
    void szCalc();
    void SimilCalculation();
    diffData BestChoice;
    std::vector<diffData> rowDiffs;
    std::vector<diffData> output;
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
    void diffCalc() override{};
    void diffCalc(std::vector<int>, std::vector<std::vector<double>>);
    std::vector<ProbabilityData> getOutputProb();


    private:
    std::vector<ProbabilityData> ProbabilityVector;
    std::vector<ProbabilityData> outputVector;
    ProbabilityData BestChoiceProb;
};