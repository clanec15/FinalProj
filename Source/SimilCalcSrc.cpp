#include "../Headers/SimiCalc.hpp"

const double PISQRT = 2.5066282746310002;

//Constructor
SimilCalc::SimilCalc(){sz = 0; BestChoice = SimilCalc::diffData{0, 0};};
//Destructor
SimilCalc::~SimilCalc(){};

/**
 * Sets the first matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalc::SetFirstMtx(const std::vector<std::vector<double>>& mtx){FirstMtx=mtx;}

/**
 * Sets the second matrix to calculate the similitude
 * 
 * @param mtx The input matrix
 */
void SimilCalc::setSecondMtx(const std::vector<std::vector<double>>& mtx){SecMtx = mtx;}


/**
 * Calculates the difference of a row and a whole matrix
 * 
 * @param row The row to compute the difference of
 */
void SimilCalc::diffCalc(int row, bool mode)
{

    if(!mode){
        
    }

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
SimilCalc::diffData SimilCalc::getMostSimilarRow()
{
    SimilCalculation();
    return BestChoice;
}


//Private area
/**
 * calculates the data frame size and chooses the smallest one
 */
void SimilCalc::szCalc()
{
    sz = std::min(FirstMtx[0].size(), SecMtx[0].size());
}


/**
 * Calculates which row has the smallest difference
 */
void SimilCalc::SimilCalculation()
{
    diffData elem = rowDiffs[0];

    for(const auto& elemt : rowDiffs){
        if(elemt.diff < elem.diff){
            elem = elemt;
        }
    }

    BestChoice = elem;
}