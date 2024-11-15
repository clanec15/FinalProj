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
#include "./StatFunctions.hpp"

#ifndef SIMICALC_HPP_
#define SIMICALC_HPP_

/**
 * Abstract base class
 */
class SimilCalc{
    public:
    virtual void DiffCalculation() = 0;
};

/**
 * Summation of Absolute Values class
 */
class SimilCalcAbs : public SimilCalc{

    public:
    struct diffData final
    {  
        /**Index of the row */
        int idx;
        /**Difference of the row */
        double diff;
    };

    void SetMatrixA(const std::vector<std::vector<double>>&);
    void SetMatrixB(const std::vector<std::vector<double>>&);


    void DiffCalculation() override{};
    void DiffCalculation(int);

    
    diffData getMostSimilarRow();



    private:
    void szCalc();
    void SimilCalculation();
    /** The choice with the lowest difference */
    diffData BestChoice; 
    /** The differences of all the rows */
    std::vector<diffData> rowDiffs; 
    /** Dataframe size of the smallest matrix */
    int sz; 



    protected:
    std::vector<std::vector<double>> FirstMtx; /**First matrix to compare (Comparator) */
    std::vector<std::vector<double>> SecMtx; /**Second matrix to compare (Compared) */



};

/**
 * Bayesian probability class
 */
class SimilCalcBayesian : public SimilCalcAbs{
    public:
    struct ProbabilityData
    {
        /**Index of the row */
        int idx;
        /**Probability */
        double prob; 
    };
    void DiffCalculation() override{};
    void DiffCalculation(std::vector<int>, std::vector<std::vector<double>>);
    std::vector<ProbabilityData> getOutputProb();


    private:
    /**Vector with the probability data of all the rows of the Second Matrix */
    std::vector<ProbabilityData> ProbabilityVector; 
    /** The choice with the biggest probability */
    ProbabilityData BestChoiceProb;
};

#endif