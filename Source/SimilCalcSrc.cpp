#include "../Headers/SimiCalc.hpp"

/**
 * @fn void SimilCalcAbs::SetFirstMtx(const std::vector<std::vector<double>>& mtx)
 * @brief Sets the first matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalcAbs::SetMatrixA(const std::vector<std::vector<double>>& mtx)
{
    if(mtx.size() != 0 || !mtx.empty()){
        FirstMtx=mtx;
    }
}

/**
 * @fn void SimilCalcAbs::SetSecondMtx(const std::vector<std::vector<double>>& mtx) 
 * @brief Sets the second matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalcAbs::SetMatrixB(const std::vector<std::vector<double>>& mtx)
{
    if(mtx.size() != 0 || !mtx.empty())
    {
        SecMtx = mtx;
    }
}


/**
 * @fn void SimilCalcAbs::DiffCalculation(int row)
 * @brief Calculates the difference of a row and a whole matrix (Absoule Value Mode)
 * 
 * @param row the row to calculate the similarity
 */
void SimilCalcAbs::DiffCalculation(int row)
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



/**
 * @fn void SimilCalcBayesian::DiffCalculation(std::vector<int> ids, std::vector<std::vector<double>> means)
 * @brief Calculates bayesian similarity based on the Gaussian Distribution of the elements of the row with a certain ID of the comparing file
 * @param ids the ID vector of the comparing file
 * @param means the means matrix of the comparing file
 */
void SimilCalcBayesian::DiffCalculation(std::vector<int> ids, std::vector<std::vector<double>> means)
{
    std::vector<ProbabilityData> Vector;
    for(int i = 0; i < SecMtx.size(); i++){

        std::vector<ProbabilityData> test;
        ProbabilityData cont;
        
        for(int j = 0; j < ids.size(); j++){
            long double probability = 0.0;


            if(ids.size() == 1){
                try{
                    probability = CalculateProb(SecMtx, means, i, 0);
                } catch(std::invalid_argument &e){
                    std::cerr << e.what() << std::endl;
                    std::abort();
                }
                
            } else {
                try
                {
                    probability = CalculateProb(SecMtx, means, i, ids[j]);
                }
                catch(const std::invalid_argument& e)
                {
                    std::cerr << e.what() << std::endl;
                    std::abort();
                }
                
            }
            

            cont.prob   = probability;
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


/**
 * @fn std::vector<SimilCalcBayesian::ProbabilityData> SimilCalcBayesian::getOutputProb()
 * @brief returns the whole stored probability vector of the compared matrix
 * @returns Probability vector
 */
std::vector<SimilCalcBayesian::ProbabilityData> SimilCalcBayesian::getOutputProb()
{
    return ProbabilityVector;
}

