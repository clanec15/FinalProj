#include <vector>
#include <cmath>
#include <iostream>

class SimilCalc{

    public:

    struct diffData
    {
        int idx;
        double diff;
    };

    SimilCalc();
    ~SimilCalc();
    void SetFstMtx(std::vector<std::vector<double>>);
    void setSecMtx(std::vector<std::vector<double>>);
    void diffCalc(int);
    diffData getMSimRow();
    void GetMtxSzDiff();
    
    

    

    private:

    std::vector<std::vector<double>> FirstMtx;
    std::vector<std::vector<double>> SecMtx;
    diffData BestChoice;
    int szCalc();
    void SimilCalculation();
    std::vector<diffData> rowDiffs;
    int sz;


};