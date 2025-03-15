#ifndef mainFunctions_H
#define mainFunctions_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>

double calcZScore(double xbar, double sd, double xUnit);
double calcMeanStdDev(const double* arr, size_t size, double* meanResult);
double calcFisher(double percentile, double dof1, size_t vectorSize);
double calcC(double F, size_t n, size_t dof);
double* calcResiduals(const double* values, double meanValue, size_t size);
int performIterativeJackknife(double* values, size_t size, double** finalValues, 
                             size_t* finalSize, double* zscores, double percentile, double dof1);
int performNoOutlier(double* values, size_t size, double* zscores);
int maxResidual(const double* values, double meanValue, size_t size, 
               double* maxRes, size_t* maxIndex);

#ifdef __cplusplus
}
#endif 

#endif
