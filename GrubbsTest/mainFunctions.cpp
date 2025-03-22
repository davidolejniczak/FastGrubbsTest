#include "mainFunctions.h"
#include <iostream>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <boost/math/distributions/students_t.hpp>

namespace py = pybind11;

double calcZScore(double mean, double sd, double xUnit) {
    return (xUnit - mean)/sd;  
}

// Welford's algorithm
double calcMeanStdDev(const double* arr, size_t size, double* meanResult) {
    if (size == 0) {
        if (meanResult) *meanResult = 0.0;
        return 0.0;
    }
    
    double mean = 0.0, M2 = 0.0;
    for (size_t i = 0; i < size; i++) {
        double d1 = arr[i] - mean;
        mean += d1 / (i + 1);
        double d2 = arr[i] - mean;
        M2 += d1 * d2;
    }
    
    if (meanResult) *meanResult = mean;
    return (size > 1) ? std::sqrt(M2/size) : 0.0;
}

double calcG(double T, size_t n) {
    return ((n-1)/std::sqrt(n)) * ((T*T)/ (std::sqrt(n-2+(T*T))));
}

double* calcResiduals(const double* values, double meanValue, size_t size) {
    double* residuals = (double*)std::malloc(size * sizeof(double));
    if (!residuals) {
        std::cerr << "Error: residuals memory failed" << std::endl;
        return nullptr; 
    }
    
    for (size_t i = 0; i < size; i++) {
        residuals[i] = std::fabs(values[i] - meanValue);
    }
    
    return residuals;
}

int maxResidual(const double* values, double meanValue, size_t size, double* maxRes, size_t* maxIndex) {
    if (!values || size == 0 || !maxRes || !maxIndex) {
        std::cerr << "Error: maxResidual input params" << std::endl;
        return -1;
    }
    
    *maxRes = -1.0;
    *maxIndex = 0;
    
    for (size_t i = 0; i < size; i++) {
        double absResidual = std::fabs(values[i] - meanValue);
        if (absResidual > *maxRes) {
            *maxRes = absResidual;
            *maxIndex = i;
        }
    }
    
    return 0;
}

double calcTDist(double alpha, size_t n) {
    auto dist = boost::math::students_t_distribution<double>(n-2);
    return boost::math::quantile(dist, (1-(alpha/(2*n))));
}

int performGrubbs(double* values, size_t size, double** finalValues, 
                 size_t* finalSize, double* zscores, double alpha) {
    if (size == 0 || !values || !finalValues || !finalSize || !zscores) {
        std::cerr << "Error: Grubbs input params" << std::endl;
        return -1;
    }
    double* currentValues = (double*)std::malloc(size * sizeof(double));
    if (!currentValues) {
        std::cerr << "Error: currentValues memory failed" << std::endl;
        return -1;
    }
    
    std::memcpy(currentValues, values, size * sizeof(double));
    size_t currentSize = size;
    double meanValue, stdValue;
    int outlierFound = 1;
    size_t maxIndex;
    double maxRes;
        
    while (outlierFound && currentSize > 1) {

        stdValue = calcMeanStdDev(currentValues, currentSize, &meanValue); 
        double T = calcTDist(alpha, currentSize);
        double GFactor = calcG(T, currentSize);
        
        if (maxResidual(currentValues, meanValue, currentSize, &maxRes, &maxIndex) != 0) {
            std::free(currentValues);  
            return -1;
        }
        
        if (maxRes > GFactor) {
            std::memmove(&currentValues[maxIndex], &currentValues[maxIndex + 1], 
                    (currentSize - maxIndex - 1) * sizeof(double));
            currentSize--;
        } else {
            outlierFound = 0;
        }
    }
    
    *finalValues = (double*)std::malloc(currentSize * sizeof(double));  
    if (!*finalValues) {
        std::free(currentValues); 
        std::cerr << "Error: finalValues memory failed" << std::endl;
        return -1;
    }
    
    std::memcpy(*finalValues, currentValues, currentSize * sizeof(double));
    *finalSize = currentSize;
    
    stdValue = calcMeanStdDev(*finalValues, *finalSize, &meanValue);
    for (size_t i = 0; i < size; i++) {
        zscores[i] = calcZScore(meanValue, stdValue, values[i]);
    }
    std::free(currentValues);
    return 0;
}

int performNoOutlier(double* values, size_t size, double* zscores) {
    if (size == 0 || !values || !zscores) {
        std::cerr << "Error: NoOutlier input params" << std::endl;
        return -1;
    }
    double meanValue;
    double stdValue = calcMeanStdDev(values, size, &meanValue);
    
    if (stdValue == 0.0) {
        std::cerr << "Error: Standard deviation is zero" << std::endl;
        return -1;
    }
    
    for (size_t i = 0; i < size; i++) {
        zscores[i] = calcZScore(meanValue, stdValue, values[i]);
    }
    
    return 0;
}
