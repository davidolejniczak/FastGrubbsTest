#include "mainFunctions.hpp"
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

std::shared_ptr<double[]> calcResiduals(const double* values, double meanValue, size_t size) {
    std::shared_ptr<double[]> residuals(new double[size]);    
    for (size_t i = 0; i < size; i++) {
        residuals[i] = std::fabs(values[i] - meanValue);
    }

    return residuals;
}

int maxResidual(const double* values, double meanValue, size_t size, double* maxRes, size_t* maxIndex) {    
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

int performGrubbs(std::shared_ptr<double[]>& values, size_t size, std::shared_ptr<double[]>& finalValues, 
                 size_t* finalSize, std::shared_ptr<double[]>& zscores, double alpha) {
    if (size == 0 || !values || !finalSize || !zscores) {
        std::cerr << "Error: Grubbs input params" << std::endl;
        return -1;
    }

    std::shared_ptr<double[]> currentValues(new double[size]);
    
    std::copy(values.get(),values.get() + size, currentValues.get());
    size_t currentSize = size;
    double meanValue, stdValue;
    int outlierFound = 1;
    size_t maxIndex;
    double maxRes;
        
    while (outlierFound && currentSize > 1) {

        stdValue = calcMeanStdDev(currentValues.get(), currentSize, &meanValue); 
        double T = calcTDist(alpha, currentSize);
        double GFactor = calcG(T, currentSize);
        
        if (maxResidual(currentValues.get(), meanValue, currentSize, &maxRes, &maxIndex) != 0) {
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
    
    std::shared_ptr<double[]> newFinalValues(new double[currentSize]);
    finalValues = newFinalValues;

    std::copy(currentValues.get(), currentValues.get() + currentSize,finalValues.get());    
    stdValue = calcMeanStdDev(finalValues.get(), currentSize, &meanValue);

    if (stdValue == 0.0) {
        std::cerr << "Error: Standard deviation is zero" << std::endl;
        return -1;
    }
    
    for (size_t i = 0; i < size; i++) {
        zscores[i] = calcZScore(meanValue, stdValue, values[i]);
    }
    return 0;
}

int performNoOutlier(std::shared_ptr<double[]>& values, size_t size,std::shared_ptr<double[]>& zscores) {
    if (size == 0 || !values || !zscores) {
        std::cerr << "Error: NoOutlier input params" << std::endl;
        return -1;
    }
    double meanValue;
    double stdValue = calcMeanStdDev(values.get(), size, &meanValue);
    
    if (stdValue == 0.0) {
        std::cerr << "Error: Standard deviation is zero" << std::endl;
        return -1;
    }
    
    for (size_t i = 0; i < size; i++) {
        zscores[i] = calcZScore(meanValue, stdValue, values[i]);
    }
    
    return 0;
}
