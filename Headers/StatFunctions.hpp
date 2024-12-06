/**
 * @file StatFunctions.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Header for stadistics files 
 * @version 0.1
 * @date 2024-10-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#ifndef STATFUNCTIONS_HPP_
#define STATFUNCTIONS_HPP_



#include<vector>
#include<math.h>
#include <stdexcept>
#include <limits>

int idIndexFinder(int elem, std::vector<int> ids){
    for(int i = 0; i < ids.size(); i++){
        if(ids[i] == elem){
            return i;
        }
    }
    return 0;
}

struct variancePackage
{
    long double variance;
    long double mean;
};

/**
 * @fn double CalculateVariance(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
 * @brief Calculates the variance of the row using the Two-pass Variance algorithm to compensate for floating-point number summation innacuracies
 * @returns the variance of the row
 * @param Matrix The input matrix
 * @param Means The means of the comparing matrix
 * @param col the column to be calculated
 * @param id the id of the comparing matrix means
 */
variancePackage CalculateVariance(std::vector<std::vector<double>>& Matrix, int id)
{
    long double val = 0.0;
    int idSz = Matrix[id].size();

    for(int i = 0; i < idSz; i++){
        val += Matrix[id][i];
    }

    val /= (double)idSz;


    double sd = 0.0;
    for(int i = 0; i < idSz;  i++){
        sd += pow(Matrix[id][i] - val, 2);
    }

    sd /= ((double)idSz);

    return {sqrt(sd), val};

}



/**
 * @fn double CalculateProb(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
 * @brief Calculates the probability of the selected row to the comparing matrix
 * @param Matrix The input matrix
 * @param Means The means of the comparing matrix
 * @param col the column to be calculated
 * @param id the id of the comparing matrix means
 * @returns the gaussian distribution of the row
 */
double CalculateProb(std::vector<double>& Row, std::vector<std::vector<double>> Means, int id)
{
    variancePackage variance = CalculateVariance(Means, id);
    if (variance.variance <= 0 ){
        return std::numeric_limits<long double>::min();
    }

    
    long double maxProb = 1.0;
    long double base = (sqrt(2*M_PI)*variance.variance);
    for(int i = 0; i < Row.size()-1; i++){
        double b = Row[i];
        long double exponent = -pow((b-variance.mean),2)/(2*(variance.variance*variance.variance));
        maxProb *= ((1/base) * (pow(M_E, exponent)));
    }

    return maxProb;

}


#endif