#ifndef mainFunctions_H
#define mainFunctions_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>

double zScore(double xbar, double sd, double xUnit);
double sumArray(const double* arr, size_t size);
double mean(const double* arr, size_t size);
double standardDeviation(const double* arr, double xbar, size_t size);
double calcFisher(double percentile, double dof1, size_t vectorSize);
double calculateC(double F, size_t n, size_t dof);
double* calculateResiduals(const double* values, double meanValue, size_t size);
int performIterativeJackknife(double* values, size_t size, double** finalValues, 
                             size_t* finalSize, double* zscores, double percentile, double dof1);

#ifdef __cplusplus
}
#endif 

#endif
