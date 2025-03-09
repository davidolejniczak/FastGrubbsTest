#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "mainFunctions.h"
#include <pybind11/pybind11.h> 
#include <pybind11/stl.h>
#include <stddef.h>
#include <map>
#include <algorithm>

using namespace std; 
namespace py = pybind11;

/**
 * @brief Configuration class for the JackKnife algorithm
 * 
 * This class provides an interface to configure and run the iterative jackknife
 * algorithm with customizable parameters. It maintains the percentile and degrees
 * of freedom settings and provides methods to modify them.
 */
class JackKnifeConfig {
private:
    double percentile; 
    double dof1;      
    bool useListOutput;
    bool warningFlag;

public:
    /**
     * @brief Constructor initializing default parameters
     */
    JackKnifeConfig() : percentile(0.95), dof1(3.0), useListOutput(true), warningFlag(true) {}
    
    /**
     * @brief Get current percentile value
     * @return Current percentile setting
     */
    double getPercentile() const { return percentile; }

    /**
     * @brief Get current degrees of freedom parameter
     * @return Current dof1 setting
     */
    double getDof1() const { return dof1; }
    
    /**
     * @brief Get current output format setting
     * @return True if using list output format, false if using dictionary
     */
    bool getUseListOutput() const { return useListOutput; }
    
    /**
     * @brief Set new percentile value
     * @param p New percentile value (must be between 0 and 1)
     * @throws ValueError if p is not between 0 and 1
     */
    void setPercentile(double p) {
        if (p < 0.0 || p > 1.0) {
            PyErr_SetString(PyExc_ValueError, "Percentile must be between 0 and 1");
            return;
        }
        percentile = p;
    }
    
    /**
     * @brief Set new degrees of freedom parameter
     * @param d New dof1 value (must be greater than 0)
     * @throws ValueError if d is not positive
     */
    void setDof1(double d) {
        if (d <= 0.0) {
            PyErr_SetString(PyExc_ValueError, "Degrees of freedom must be greater than 0");
            return;
        }
        dof1 = d;
    }
    
    /**
     * @brief Set the output format to use (list or dictionary)
     * @param useList True to use list output format, false to use dictionary
     */
    void setUseListOutput(bool useList) {
        useListOutput = useList;
    }

    /**
     * @brief Set warnings on or off 
     * @param flag True activates warnings, false turns them off
     */
    void setwarningFlag(bool flag) {
        warningFlag = flag;
    }
    
    /**
     * @brief Generate z-scores using the iterative jackknife algorithm
     * 
     * @param dataList List containing input values
     * @return Dictionary mapping input values to their z-scores
     * @throws MemoryError if unable to allocate required memory
     */
    py::object runJackknife(const py::list& dataList) const {
        vector<double> dataValues;
        
        for (auto item : dataList) {
            double value = item.cast<double>();
            dataValues.push_back(value);
        }
        
        size_t size = dataValues.size();
        if (size == 0) {
            return useListOutput ? py::cast<py::object>(py::list()) : py::cast<py::object>(py::dict());
        }
        
        double* values = (double*)malloc(size * sizeof(double));
        double* zscores = (double*)malloc(size * sizeof(double));
        
        if (!values || !zscores) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for jackknife algorithm");
            if (values) free(values);
            if (zscores) free(zscores);
            return useListOutput ? py::cast<py::object>(py::list()) : py::cast<py::object>(py::dict());
        }
        
        for (size_t i = 0; i < size; i++) {
            values[i] = dataValues[i];
        }
        
        double* finalValues = nullptr;
        size_t finalSize = 0;
        int status = performIterativeJackknife(values, size, &finalValues, &finalSize, zscores, percentile, dof1);
        
        if (status != 0) {
            free(values);
            free(zscores);
            if (finalValues) free(finalValues);
            return useListOutput ? py::cast<py::object>(py::list()) : py::cast<py::object>(py::dict());
        }
        
        if (useListOutput) {
            py::list resultList;
            for (size_t i = 0; i < size; i++) {
                py::list pair;
                pair.append(dataValues[i]); 
                pair.append(zscores[i]);   
                resultList.append(pair);
            }
            
            free(values);
            free(zscores);
            if (finalValues) free(finalValues);
            return resultList;
        } else {
            // Create a dictionary mapping values to z-scores (original behavior)
            py::dict resultDict;
            for (size_t i = 0; i < size; i++) {
                resultDict[py::cast(dataValues[i])] = py::cast(zscores[i]);
            }
            
            free(values);
            free(zscores);
            if (finalValues) free(finalValues);
            return resultDict;
        }
    }
    
    /**
     * @brief Process data using the iterative jackknife algorithm with dict input
     * Overloading dict input.
     * 
     * @param dataDict Dictionary containing input values
     * @return Dictionary mapping input values to their z-scores
     */
    py::object runJackknife(const py::dict& dataDict) const {
        if (!useListOutput && warningFlag) {
            PyErr_WarnEx(PyExc_Warning, 
                "Using dictionary input/output format may lose duplicate values. "
                "Consider using list input with setUseListOutput(True) for handling duplicates.", 
                1);
        }
        
        py::list dataList;
            for (auto item : dataDict) {
                item.first.cast<double>();
                dataList.append(item.first);
            }
        return runJackknife(dataList);
    }
};

/**
 * @brief Wrapper function for calcFisher with default parameters
 */
double calcFisher_wrapper(size_t vectorSize, double percentile = 0.95, double dof1 = 3.0) {
    return calcFisher(percentile, dof1, vectorSize);
}

PYBIND11_MODULE(main, JKI) {
    JKI.doc() = "Iterative JackKnife module for identifying outliers in datasets using statistical methods";

    JKI.def("zScore", &zScore,"Calculate z-score for a value.\n\n");
    JKI.def("calcFisher", &calcFisher, "Calculate F-distribution value with full parameter control.");
    JKI.def("calcFisher", &calcFisher_wrapper, py::arg("vectorSize"), py::arg("percentile") = 0.95, py::arg("dof1") = 3.0,
            "Calculate F-distribution value with default parameters.\n\n");

    py::class_<JackKnifeConfig>(JKI, "JackKnifeConfig","Configuration class for the iterative jackknife algorithm.\n\n")
        .def(py::init<>(), "Create new JackKnifeConfig with default parameters")
        .def("getPercentile", &JackKnifeConfig::getPercentile,"Get current percentile setting")
        .def("getDof1", &JackKnifeConfig::getDof1,"Get current degrees of freedom setting")
        .def("getUseListOutput", &JackKnifeConfig::getUseListOutput,"Get current output format setting (True for list, False for dict)")
        .def("setPercentile", &JackKnifeConfig::setPercentile, "Set new percentile value (must be between 0 and 1)")
        .def("setDof1", &JackKnifeConfig::setDof1,  "Set new degrees of freedom parameter (must be positive)")
        .def("setUseListOutput", &JackKnifeConfig::setUseListOutput, "Set output format (True for list of [data, zscore] pairs, False for dict)")
        .def("runJackknife", py::overload_cast<const py::dict&>(&JackKnifeConfig::runJackknife, py::const_),"Process data using dictionary input. Returns either dict or list based on settings.")
        .def("runJackknife", py::overload_cast<const py::list&>(&JackKnifeConfig::runJackknife, py::const_),"Process data using list input. Returns either dict or list based on settings.");
}