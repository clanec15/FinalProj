/**
 * @file RadixSort.hpp
 * @author Victor Aguilar Rodriguez (ar202341753@alm.buap.mx)
 * @brief Header for implementation of Radix Sort with floating point numbers
 * @version 0.1
 * @date 2024-09-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <vector>
#include <cmath>
#include <algorithm>
#ifndef SORTINGCOMP_HPP_
#define SORTINGCOMP_HPP_



/**
 * Basic struct to hold the converted floating point array and its rounding exponent
 */
struct convData
{
    /** The biggest exponent of the array */
    int globalExp; 
    /** The converted array */
    std::vector<int> data; 
};

/**
 * @fn int dtoi(double n)
 * @brief gets the rounding exponent of a floating point number
 * 
 * This function finds the rounding exponent (aka: xd*(10^n) = xi, now find n) of a floating point number
 * 
 * @param n the floating point number
 * @returns the rounding exponent of the FP number
 */
int dtoi(double n){
    int i = 0;
    while (std::fabs(n - std::round(n)) > __FLT_EPSILON__) {
        n *= 10;
        i++;
    }
    
    return i;
}

/**
 * @fn int getMax(std::vector<int> arr)
 * @brief Gets the largest element of a vector
 * 
 * @param arr The input array
 * @returns Iterator referencing the first instance of the largest value.
 */
int getMax(std::vector<int> arr){
    return *std::max_element(arr.begin(), arr.end());
}

/**
 * @fn convData dvtoiv(std::vector<double> vct)
 * @brief converts a double datatype vector into an integet vector
 * 
 * It gets the rounding exponent of all the elements of the input vector, gets the biggest one
 * and multiplies all the elements of the vector with that exponent and stores it on a new
 * integer vector
 * 
 * @param vct The input vector (double)
 * @returns A convData struct
 * 
 * @see convData
 * @see dtoi
 * 
 * 
 */
convData dvtoiv(std::vector<double> vct){
    std::vector<int> mults;
    int globalMult;
    convData output;

    for(int i = 0; i < vct.size(); i++){
        mults.push_back(dtoi(vct[i]));
    }
    
    globalMult = getMax(mults);
    output.globalExp = globalMult;
    std::vector<int> intArr;
    for(int i = 0; i < vct.size(); i++){
        intArr.push_back(vct[i]*pow(10, globalMult));
    }
    output.data = intArr;
    return output;
}

//--------------------------------------------------------------------------//

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

/**
 * @fn void RadixSrt(std::vector<int>& vct)
 * @brief Radix Sort
 * 
 * Thats it, there's nothing especial to it
 * its just radix sort
 * 
 * @param vct the vector to be sorted
 */
void RadixSrt(std::vector<int>& vct){
    int mxVal = getMax(vct);
    for(int j = 1; mxVal/j > 0; j*=10){
        countSrt(vct, j);
    }
}

#endif