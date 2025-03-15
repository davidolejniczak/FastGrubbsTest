#include "mainFunctions.h"
#include <stdio.h>
#include <math.h> 
#include <stddef.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <Python.h>
#include <stdlib.h> 
#include <string.h>

double calcZScore(double mean, double sd, double xUnit) {
    return (xUnit - mean)/sd;  
}

//Welford's algorithm
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
    return (size > 1) ? sqrt(M2/size) : 0.0;
}

double calcFisher(double percentile, double dof1, size_t size) {
    double fisherResult = gsl_cdf_fdist_Pinv(percentile, dof1, (double)size);
    return fisherResult;
}

double calcC(double F, size_t n, size_t dof) {
    return sqrt(((3.0 * F) / (1.0 + ((3.0 * F - 1.0) / (double)dof))) * ((double)dof / (double)n));
}

double* calcResiduals(const double* values, double meanValue, size_t size) {
    double* residuals = (double*)malloc(size * sizeof(double));
    if (!residuals) {
        fprintf(stderr, "Error: Failed to allocate memory for residuals\n");
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++) {
        residuals[i] = fabs(values[i] - meanValue);
    }
    
    return residuals;
}


int maxResidual(const double* values, double meanValue, size_t size, double* maxRes, size_t* maxIndex) {
    if (!values || size == 0 || !maxRes || !maxIndex) {
        fprintf(stderr, "Error: Invalid parameters in maxResidual\n");
        return -1;
    }
    
    *maxRes = -1.0;
    *maxIndex = 0;
    
    for (size_t i = 0; i < size; i++) {
        double absResidual = fabs(values[i] - meanValue);
        if (absResidual > *maxRes) {
            *maxRes = absResidual;
            *maxIndex = i;
        }
    }
    
    return 0;
}

int performIterativeJackknife(double* values, size_t size, double** finalValues, 
                             size_t* finalSize, double* zscores, double percentile, double dof1) {
    if (size == 0 || !values || !finalValues || !finalSize || !zscores) {
        fprintf(stderr, "Error: IterativeJackKnife input params\n");
        return -1;
    }
    
    double* currentValues = (double*)malloc(size * sizeof(double));
    if (!currentValues) {
        fprintf(stderr, "Error: currentValues memory failed\n");
        return -1;
    }
    
    memcpy(currentValues, values, size * sizeof(double));
    size_t currentSize = size;
    double meanValue, stdValue, csFactor;
    int outlierFound = 1;
    size_t maxIndex;
    double maxRes;
        
    while (outlierFound && currentSize > 1) {
        stdValue = calcMeanStdDev(currentValues, currentSize, &meanValue);
        
        size_t dof = currentSize - 1;
        double F = calcFisher(percentile, dof1, dof);
        double C = calcC(F, currentSize, dof);
        csFactor = C * stdValue;
        if (csFactor <= 2.0) {
            csFactor = 2.0;
        }
        
        if (maxResidual(currentValues, meanValue, currentSize, &maxRes, &maxIndex) != 0) {
            free(currentValues);
            return -1;
        }
        
        if (maxRes > csFactor) {
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
        fprintf(stderr, "Error: finalValues memory failed\n");
        return -1;
    }
    
    memcpy(*finalValues, currentValues, currentSize * sizeof(double));
    *finalSize = currentSize;
    
    stdValue = calcMeanStdDev(*finalValues, *finalSize, &meanValue);
    for (size_t i = 0; i < size; i++) {
        zscores[i] = calcZScore(meanValue, stdValue, values[i]);
    }
    
    free(currentValues);
    return 0;
}

int performNoOutlier(double* values, size_t size, double* zscores) {
    if (size == 0 || !values || !zscores) {
        fprintf(stderr, "Error: NoOutlier input params\n");
        return -1;
    }

    double meanValue;
    double stdValue = calcMeanStdDev(values, size, &meanValue);
    
    if (stdValue == 0.0) {
        fprintf(stderr, "Error: Standard deviation is zero\n");
        return -1;
    }
    
    for (size_t i = 0; i < size; i++) {
        zscores[i] = calcZScore(meanValue, stdValue, values[i]);
    }
    
    return 0;
}
