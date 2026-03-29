#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H

#include <cstddef>
#include <memory>

double calcZScore(double xbar, double sd, double xUnit);
double calcMeanStdDev(const double* arr, size_t size, double* meanResult);
double calcG(double T, size_t n);
std::shared_ptr<double[]> calcResiduals(const double* values, double meanValue, size_t size);
int maxResidual(const double* values, double meanValue, size_t size, 
               double* maxRes, size_t* maxIndex);
double calcTDist(double alpha, size_t n);
int performGrubbs(std::shared_ptr<double[]>& values, size_t size, std::shared_ptr<double[]>& finalValues, 
                 size_t* finalSize, std::shared_ptr<double[]>& zscores, double alpha);
int performNoOutlier(std::shared_ptr<double[]>& values, size_t size, std::shared_ptr<double[]>& zscores);

#endif // MAINFUNCTIONS_H