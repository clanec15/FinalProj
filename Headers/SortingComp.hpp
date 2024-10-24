//Header for Radix Sort implementation, 2024

#include <vector>
#include <cmath>
#include <algorithm>
#ifndef SORTINGCOMP_HPP
#define SORTINGCOMP_HPP


struct convData
{
    int globalMult;
    std::vector<int> data;
};

int dtoi(double n){
    int i = 0;
    while (std::fabs(n - std::round(n)) > __FLT_EPSILON__) {
        n *= 10;
        i++;
    }
    
    return i;
}

int getMax(std::vector<int> arr){
    return *std::max_element(arr.begin(), arr.end());
}

convData dvtoiv(std::vector<double> vct){
    std::vector<int> mults;
    int globalMult;
    convData output;


    for(int i = 0; i < vct.size(); i++){
        mults.push_back(dtoi(vct[i]));
    }

    globalMult = getMax(mults);
    output.globalMult = globalMult;

    std::vector<int> intArr;

    for(int i = 0; i < vct.size(); i++){
        intArr.push_back(vct[i]*pow(10, globalMult));
    }

    output.data = intArr;
    return output;
}

void countSrt(std::vector<int>& arr, int exp){
    int n = arr.size();
    std::vector<int> out(n);
    int count[10] = {0};

    for(int i = 0; i < n; i++){
        int dgt = (arr[i]/exp)%10;
        count[dgt]++;
    }

    for(int i = 1; i < 10; i++){
        count[i] += count[i-1];
    }

    for(int i = n-1; i >= 0; i--){
        int dgt = (arr[i]/exp)%10;
        out[count[dgt]-1] = arr[i];
        count[dgt]--; 
    }

    for(int i = 0; i < n; i++){
        arr[i] = out[i];
    }
}

void RadixSrt(std::vector<int>& vct){
    int mxVal = getMax(vct);
    for(int j = 1; mxVal/j > 0; j*=10){
        countSrt(vct, j);
    }
}

#endif