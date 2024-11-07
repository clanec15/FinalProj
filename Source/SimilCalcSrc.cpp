#include "../Headers/SimiCalc.hpp"

#define TWOPISQRT   2.50662827463100
#define EULER       2.71828182845905

//Constructor
SimilCalcAbs::SimilCalcAbs(){sz = 0; BestChoice = SimilCalcAbs::diffData{0, 0};};
//Destructor
SimilCalcAbs::~SimilCalcAbs(){};

/**
 * Sets the first matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalcAbs::SetFirstMtx(const std::vector<std::vector<double>>& mtx){FirstMtx=mtx;}

/**
 * Sets the second matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalcAbs::setSecondMtx(const std::vector<std::vector<double>>& mtx){SecMtx = mtx;}


/**
 * Calculates the difference of a row and a whole matrix
 * 
 * @param row The row to compute the difference of
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
    int items = -1;

    for(int i = 0; i < Matrix.size(); i++){
        if(Matrix[i][Matrix[0].size()-1] == 0){
            SumSquares += pow(Matrix[i][col] - Means[id][col], 2);
            items++;
        }
    }
    return SumSquares/items;
}

double CalculateProb(std::vector<std::vector<double>>& Matrix, std::vector<std::vector<double>> Means, int col, int id)
{
    double variance = CalculateVariance(Matrix, Means, col, id);
    double maxProb = 0.0;


    for(int i = 0; i < Matrix[0].size()-1; i++){

        double base = variance*TWOPISQRT;
        double exponent = -(pow(Matrix[col][i]-Means[id][col], 2)/variance);
        double prob = (1/base) * (pow(EULER, exponent));

        //std::cout << "[" << col << ", " << i <<"] (" << TestMatrix[col][i] << "): " << prob << std::endl;
        maxProb += prob;
    }

    

    return maxProb;
}


std::vector<SimilCalcBayesian::ProbabilityData> SimilCalcBayesian::diffCalc(std::vector<int> ids, std::vector<std::vector<double>> means)
{
    std::vector<ProbabilityData> outputVector;
    for(int i = 0; i < SecMtx.size(); i++){
        std::vector<ProbabilityData> test;
        ProbabilityData cont;
        for(int j = 0; j < ids.size(); j++){
            cont.prob   = CalculateProb(SecMtx, means, i, ids[j]);
            cont.idx     = ids[j];
            test.push_back(cont);
        }

        ProbabilityData elem = test[0];

        for(const auto& elemt : test){
            if(elemt.prob < elem.prob){
                elem = elemt;
            }
        }

        outputVector.push_back(elem);
    }

    return outputVector;
}