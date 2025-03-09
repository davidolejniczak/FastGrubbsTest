#include "mainFunctions.h"
#include <stdio.h>
#include <math.h> 
#include <stddef.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <Python.h>
#include <stdlib.h> 

double zScore(double xbar, double sd, double xUnit) {
    return (xUnit - xbar)/sd;  
}

double sumArray(const double* arr, size_t size) {
    double sum = 0.0; 
    for (size_t i = 0; i < size; i++){
        sum += arr[i]; 
    }
    return sum;
}

double mean(const double* arr, size_t size) {
    if (size == 0) {
        return 0.0;
    }
    double sum = sumArray(arr, size);
    return sum/size; 
}

double standardDeviation(const double* arr, double xbar, size_t size) {
    if (size == 0) {
        return 0.0;
    }
    double xxbarSquare = 0.0; 
    for (size_t i = 0; i < size; i++){
        xxbarSquare += ((arr[i] - xbar)*(arr[i] - xbar));
    }
    return sqrt(xxbarSquare/size); 
}

double calcFisher(double percentile, double dof1, size_t vectorSize) {
    if (percentile < 0.0 || percentile > 1.0) {
        PyErr_SetString(PyExc_ValueError, "Percentile must be between 0 and 1");
        return -1.0;
    }
    
    if (dof1 < 0.0) {
        PyErr_SetString(PyExc_ValueError, "First degrees of freedom must be greater than zero");
        return -1.0;
    }
    
    if (vectorSize < 1) {
        PyErr_SetString(PyExc_ValueError, "Vector size must be at least 1");
        return -1.0;
    }
    
    double fisherResult = gsl_cdf_fdist_Pinv(percentile, dof1, (double)vectorSize);
    if (isnan(fisherResult)) {
        PyErr_SetString(PyExc_ValueError, "Computation resulted in an invalid (NaN) Fisher result");
        return -1.0;
    }
    return fisherResult;
}

double calculateC(double F, size_t n, size_t dof) {
    double numerator = 3.0 * F;
    double denominator = 1.0 + ((3.0 * F - 1.0) / (double)dof);
    double fraction = (double)dof / (double)n;
    return sqrt((numerator / denominator) * fraction);
}

double* calculateResiduals(const double* values, double meanValue, size_t size) {
    double* residuals = (double*)malloc(size * sizeof(double));
    if (!residuals) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for residuals");
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++) {
        residuals[i] = fabs(values[i] - meanValue);
    }
    
    return residuals;
}

int performIterativeJackknife(double* values, size_t size, double** finalValues, 
                             size_t* finalSize, double* zscores, double percentile, double dof1) {
    if (size == 0 || !values || !finalValues || !finalSize || !zscores) {
        PyErr_SetString(PyExc_ValueError, "Invalid input parameters");
        return -1;
    }
    
    double* currentValues = (double*)malloc(size * sizeof(double));
    if (!currentValues) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for current values");
        return -1;
    }
    
    memcpy(currentValues, values, size * sizeof(double));
    
    size_t currentSize = size;
    double meanValue, stdValue, csFactor;
    int outlierFound = 1;
    size_t maxIndex;
    double maxResidual;
        
    while (outlierFound && currentSize > 1) {
        meanValue = mean(currentValues, currentSize);
        stdValue = standardDeviation(currentValues, meanValue, currentSize);
        
        size_t dof = currentSize - 1;
        double F = calcFisher(percentile, dof1, dof);
        double C = calculateC(F, currentSize, dof);
        csFactor = C * stdValue;
        if (csFactor <= 2.0) {
            csFactor = 2.0;
        }
        
        maxResidual = -1.0;
        maxIndex = 0;
        for (size_t i = 0; i < currentSize; i++) {
            double residual = fabs(currentValues[i] - meanValue);
            if (residual > maxResidual) {
                maxResidual = residual;
                maxIndex = i;
            }
        }
        
        if (maxResidual > csFactor) {
            memmove(&currentValues[maxIndex], &currentValues[maxIndex + 1], 
                    (currentSize - maxIndex - 1) * sizeof(double));
            currentSize--;
        } else {
            outlierFound = 0;
        }
    }
    
    *finalValues = (double*)malloc(currentSize * sizeof(double));
    if (!*finalValues) {
        free(currentValues);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for final values");
        return -1;
    }
    
    memcpy(*finalValues, currentValues, currentSize * sizeof(double));
    *finalSize = currentSize;
    
    meanValue = mean(*finalValues, *finalSize);
    stdValue = standardDeviation(*finalValues, meanValue, *finalSize);

    for (size_t i = 0; i < size; i++) {
        zscores[i] = zScore(meanValue, stdValue, values[i]);
    }
    
    free(currentValues);
    return 0;
}
