#ifndef STATFUNCTIONS_HPP_
#define STATFUNCTIONS_HPP_



#include<vector>
#include<math.h>

constexpr double TWOPISQRT = 2.50662827463;

double CalculateVariance(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
{
    double SumSquares = 0.0;
    int items = 0;

    for(int i = 0; i < Matrix[0].size(); i++){
        SumSquares += pow(Matrix[col][i] - Means[id][col], 2);
        items++;
    }
    
    if(items > 1)
    {
        return SumSquares / (items - 1);
    }

    return -314946441810;

}

double CalculateProb(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
{
    double variance = CalculateVariance(Matrix, Means, col, id);
    if(variance == -314946441810) return -94838553137746;
    double maxProb = 0.0;


    for(int i = 0; i < Matrix[0].size()-1; i++){

        double base = sqrt(variance)*TWOPISQRT;
        double exponent = -(pow(Matrix[col][i]-Means[id][col], 2)/variance);
        double prob = (1/base) * (pow(M_E, exponent));

        maxProb += prob;
    }

    

    return maxProb;
}


#endif