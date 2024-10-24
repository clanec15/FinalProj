#include "../Headers/SimiCalc.hpp"

SimilCalc::SimilCalc(){sz = 0; BestChoice = SimilCalc::diffData{0, 0};};
SimilCalc::~SimilCalc(){};

void SimilCalc::SetFstMtx(std::vector<std::vector<double>> mtx)
{
    FirstMtx=mtx;
}

void SimilCalc::setSecMtx(std::vector<std::vector<double>> mtx){
    SecMtx = mtx;
}

void SimilCalc::GetMtxSzDiff()
{
    int diff = szCalc();

    switch (diff)
    {
    case -1:
        std::cout << "Second Matrix bigger than First Matrix, using Second Matrix size" << std::endl;
        break;

    case 0:
        std::cout << "First Matrix bigger than Second Matrix, using First Matrix size" << std::endl;
        break;

    case 1:
        std::cout << "Both matrices are the same size, using first matrix size" << std::endl;
        break;
    
    default:
        break;
    }
}

void SimilCalc::diffCalc(int row)
{
    GetMtxSzDiff();
    rowDiffs.clear();
    for(int i = 0; i < FirstMtx.size(); i++){
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

int SimilCalc::szCalc()
{
    if(SecMtx[0].size() > FirstMtx[0].size()){
        sz = SecMtx[0].size();
        return -1;
    } else if(SecMtx[0].size() < FirstMtx[0].size()) {
        sz = FirstMtx[0].size();
        return 0;
    } else {
        sz = FirstMtx[0].size();
        return 1;
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