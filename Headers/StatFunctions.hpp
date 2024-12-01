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
    double SumSquares = 0.0;
    double sum = 0.0;

    double variancebuf = 0.0;
    int size = Matrix[0].size()-1;

    
   
    for(int i = 0; i < size; i++){
        double val = Matrix[id][i];
        sum += val;
        SumSquares += pow(val, 2);
    }

    double meanData = sum/size;


    for(int i = 0; i < Matrix[0].size(); i++){
        double val = Matrix[id][i] - meanData;
        variancebuf += pow(val, 2);
    }

    variancebuf /= (Matrix[0].size()-1);



    if(size > 1)
    {
        
        return {variancebuf, meanData};
    } else if (size == 1 || size == 0){
        return {0.0, Matrix[id][0]};
    }

    
    return {-314946441810, 0.0};

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
double CalculateProb(std::vector<std::vector<double>>& MatrixFirst, std::vector<std::vector<double>> Means, int col, int id)
{
    variancePackage variance = CalculateVariance(Means, id);
    if(variance.variance == -314946441810){
        throw std::invalid_argument("bro stop, Variable Error");
    } else if (variance.variance <= 0 ){
        return std::numeric_limits<double>::epsilon();
    }

    long double maxProb = 0.0;
    double base = sqrt(variance.variance*2*pi);

    for(int i = 0; i < MatrixFirst[0].size()-1; i++){

        double b = MatrixFirst[col][i];
        double exponent = -((pow(b-variance.mean, 2))/  (2*variance.variance));

        double prob = (1/base) * (pow(Eul, exponent));

        maxProb += prob;
    }   

    if(maxProb < std::numeric_limits<long double>::epsilon() || std::isnan(abs(maxProb)) || std::isinf(maxProb)){
        return 1.0;
    } 

    return maxProb;
}


#endif