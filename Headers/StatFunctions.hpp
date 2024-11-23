#ifndef STATFUNCTIONS_HPP_
#define STATFUNCTIONS_HPP_



#include<vector>
#include<math.h>

constexpr double TWOPISQRT = 2.50662827463;


/**
 * @fn double CalculateVariance(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
 * @brief Calculates the variance of the row using the Kahan summation algorithm to compensate for floating-point number summation innacuracies
 * @returns the variance of the row
 * @param Matrix The input matrix
 * @param Means The means of the comparing matrix
 * @param col the column to be calculated
 * @param id the id of the comparing matrix means
 */
double CalculateVariance(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
{
    double SumSquares = 0.0;
    int items = 0;


    double sum = 0.0;
    double c = 0.0;
    for(int i = 0; i < Matrix[0].size(); i++){
        double y = pow(Matrix[col][i] - Means[id][col], 2);
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
        items++;
    }

    SumSquares = sum;
    
    if(items > 1)
    {
        return SumSquares / (items - 1);
    }

    return -314946441810;

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
double CalculateProb(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
{
    double variance = CalculateVariance(Matrix, Means, col, id);
    if(variance == -314946441810){
        return -94838553137746;
    } 

    double maxProb = 0.0;

    for(int i = 0; i < Matrix[0].size()-1; i++){

        double base = sqrt(variance)*TWOPISQRT;

        double exponent = -(pow(Matrix[col][i]-Means[id][col], 2)/variance);

        double prob = (1/base) * (pow(M_E, exponent));

        maxProb += prob;
    }

    if(maxProb < __DBL_EPSILON__){
        return 0.0;
    }

    return maxProb;
}


#endif