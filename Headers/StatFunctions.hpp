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


const long double Eul = 2.7182818284590452354;
const long double pi = 3.14159265358979323846;

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
    double variance;
    double mean;
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
    double val = 0.0;
    double idSz = Matrix[id].size();

    for(int i = 0; i < idSz; i++){
        val += Matrix[id][i];
    }

    val /= idSz;


    double sd = 0.0;
    for(int i = 0; i < idSz;  i++){
        sd += pow(Matrix[id][i] - val, 2);
    }

    sd /= idSz;

    return {sd, val};

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
        return std::numeric_limits<double>::epsilon();
    }

    
    long double maxProb = 0.0;
    long double base = (sqrt(2*M_PI*variance.variance));
    for(int i = 0; i < Row.size()-1; i++){
        maxProb += -0.5 * log(2 * M_PI * variance.variance) - pow((Row[i] - variance.mean), 2) / (2 * variance.variance);

    }

    return pow(M_E, maxProb);
}


#endif