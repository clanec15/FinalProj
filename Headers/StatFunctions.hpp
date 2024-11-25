#ifndef STATFUNCTIONS_HPP_
#define STATFUNCTIONS_HPP_



#include<vector>
#include<math.h>
#include <stdexcept>

const long double Eul = 2.7182818284590452354;
const long double pi = 3.14159265358979323846;

/**
 * @fn double CalculateVariance(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
 * @brief Calculates the variance of the row using the Two-pass Variance algorithm to compensate for floating-point number summation innacuracies
 * @returns the variance of the row
 * @param Matrix The input matrix
 * @param Means The means of the comparing matrix
 * @param col the column to be calculated
 * @param id the id of the comparing matrix means
 */
double CalculateVariance(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
{
    //double sum = 0.0;
    double comp = 0.0;
    double SumSquares = 0.0;
    double compSquare = 0.0;
    double mean = Means[id][col];

    
    double sum = 0.0;
    for(int i = 0; i < Matrix[0].size(); i++){
        double diff = Matrix[col][i] - mean;
        SumSquares += pow(diff, 2);
    }



    
    if(Matrix[0].size() > 1)
    {
        return SumSquares / (Matrix[0].size() - 1);
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
        throw std::invalid_argument("bro stop, Variable Error");
    } else if (variance == 0){
        return 1;
    } else if (variance < 0){
        throw std::invalid_argument("Bro what did you do, Variance cannot less than zero"); 
    }

    long double maxProb = 0.0;

    for(int i = 0; i < Matrix[0].size()-1; i++){

        double base = sqrt(variance*2*pi);

        double exponent = -(
            (pow(Matrix[col][i]-Means[id][col], 2))
            /
            (2*variance)
            );

        double prob = (1/base) * (pow(Eul, exponent));

        maxProb += prob;
    }   

    if(maxProb < std::numeric_limits<long double>::epsilon() || std::isnan(abs(maxProb))){
        return 0.0;
    } else if (std::isinf(abs(maxProb))){
        return 1;
    }

    return maxProb;
}


#endif