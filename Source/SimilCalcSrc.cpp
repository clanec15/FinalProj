#include "../Headers/SimiCalc.hpp"

#define TWOPISQRT   sqrt(M_PI*2)

/**
 * @fn void SimilCalcAbs::SetFirstMtx(const std::vector<std::vector<double>>& mtx)
 * @brief Sets the first matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalcAbs::SetFirstMtx(const std::vector<std::vector<double>>& mtx){FirstMtx=mtx;}

/**
 * @fn void SimilCalcAbs::SetSecondMtx(const std::vector<std::vector<double>>& mtx)
 * @brief Sets the second matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalcAbs::setSecondMtx(const std::vector<std::vector<double>>& mtx){SecMtx = mtx;}


/**
 * @fn void SimilCalcAbs::diffCalc(int row)
 * @brief Calculates the difference of a row and a whole matrix (Absoule Value Mode)
 * 
 * @param row the row to calculate the similarity
 */
void SimilCalcAbs::diffCalc(int row)
{
    szCalc();
    rowDiffs.clear();
    for(int i = 0; i < std::min(FirstMtx.size(), SecMtx.size()); i++){
        double diff = 0;
        diffData main;
        for(int k = 0; k < sz-1; k++){ 
            diff += fabs(FirstMtx[row][k]-SecMtx[i][k]);
        }
        main.idx = i;
        main.diff = diff;
        rowDiffs.push_back(main);
    }
}

/**
 * gets the row of the second matrix with the least difference (aka the best choice)
 * 
 * @return the best choice as a struct
 */
SimilCalcAbs::diffData SimilCalcAbs::getMostSimilarRow()
{
    SimilCalculation();
    return BestChoice;
}

//Private area
/**
 * calculates the data frame size and chooses the smallest one
 */
void SimilCalcAbs::szCalc()
{
    sz = std::min(FirstMtx[0].size(), SecMtx[0].size());
}


/**
 * Calculates which row has the smallest difference
 */
void SimilCalcAbs::SimilCalculation()
{
    diffData elem = rowDiffs[0];

    for(const auto& elemt : rowDiffs){
        if(elemt.diff < elem.diff){
            elem = elemt;
        }
    }

    BestChoice = elem;
}




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


void SimilCalcBayesian::diffCalc(std::vector<int> ids, std::vector<std::vector<double>> means)
{
    std::vector<ProbabilityData> Vector;
    for(int i = 0; i < SecMtx.size(); i++){
        std::vector<ProbabilityData> test;
        ProbabilityData cont;
        for(int j = 0; j < ids.size(); j++){
            cont.prob   = CalculateProb(SecMtx, means, i, ids[j]);
            cont.idx    = ids[j];
            test.push_back(cont);
        }

        ProbabilityData elem = test[0];

        for(const auto& elemt : test){
            if(elemt.prob > elem.prob){
                elem = elemt;
            }
        }

        Vector.push_back(elem);
    }

    ProbabilityVector = Vector;

}

std::vector<SimilCalcBayesian::ProbabilityData> SimilCalcBayesian::getOutputProb()
{
    return ProbabilityVector;
}

