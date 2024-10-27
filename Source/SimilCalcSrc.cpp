#include "../Headers/SimiCalc.hpp"

SimilCalc::SimilCalc(){sz = 0; BestChoice = SimilCalc::diffData{0, 0};};
SimilCalc::~SimilCalc(){};
void SimilCalc::SetFstMtx(std::vector<std::vector<double>>& mtx){FirstMtx=mtx;}
void SimilCalc::setSecMtx(std::vector<std::vector<double>>& mtx){SecMtx = mtx;}


void SimilCalc::diffCalc(int row)
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

SimilCalc::diffData SimilCalc::getMSimRow()
{
    SimilCalculation();
    return BestChoice;
}


//Private area

void SimilCalc::szCalc()
{
    if(SecMtx[0].size() > FirstMtx[0].size()){
        sz = SecMtx[0].size();

    } else if(SecMtx[0].size() < FirstMtx[0].size()) {
        sz = FirstMtx[0].size();

    } else {
        sz = FirstMtx[0].size();

    }
}

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