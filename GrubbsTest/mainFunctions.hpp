#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include <cstddef>

double calcZScore(double xbar, double sd, double xUnit);
double calcMeanStdDev(const double* arr, size_t size, double* meanResult);
double calcG(double T, size_t n);
double* calcResiduals(const double* values, double meanValue, size_t size);
int maxResidual(const double* values, double meanValue, size_t size, 
               double* maxRes, size_t* maxIndex);
double calcTDist(double alpha, size_t n);
int performGrubbs(double* values, size_t size, double** finalValues, 
                 size_t* finalSize, double* zscores, double alpha);
int performNoOutlier(double* values, size_t size, double* zscores);

#endif // MAINFUNCTIONS_H