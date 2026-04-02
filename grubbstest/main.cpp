#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <memory>
#include <vector>
#include <string>
#include "../helperfuncs/mainFunctions.hpp"

namespace nb = nanobind;

// run_Grubbs(data: dict, alpha: float) -> dict
// data: {str: number}, returns {str: [number, zscore]}
nb::dict run_Grubbs(nb::dict data, double alpha = 0.05) {
    size_t n = data.size();
    if (n == 0) return nb::dict();

    std::vector<std::string> keys;
    keys.reserve(n);
    std::shared_ptr<double[]> values(new double[n]);
    std::shared_ptr<double[]> zscores(new double[n]);

    size_t i = 0;
    for (auto item : data) {
        keys.push_back(nb::cast<std::string>(item.first));
        values[i] = nb::cast<double>(item.second);
        i++;
    }

    std::shared_ptr<double[]> finalValues;
    size_t finalSize = 0;

    int ret = performGrubbs(values, n, finalValues, &finalSize, zscores, alpha);
    if (ret != 0) {
        throw std::runtime_error("Grubbs test failed");
    }

    nb::dict result;
    for (size_t j = 0; j < n; j++) {
        nb::list entry;
        entry.append(values[j]);
        entry.append(zscores[j]);
        result[nb::cast(keys[j])] = entry;
    }
    return result;
}

// run_NoOutlier(data: dict) -> dict
// data: {str: number}, returns {str: [number, zscore]}
nb::dict run_NoOutlier(nb::dict data) {
    size_t n = data.size();
    if (n == 0) return nb::dict();

    std::vector<std::string> keys;
    keys.reserve(n);
    std::shared_ptr<double[]> values(new double[n]);
    std::shared_ptr<double[]> zscores(new double[n]);

    size_t i = 0;
    for (auto item : data) {
        keys.push_back(nb::cast<std::string>(item.first));
        values[i] = nb::cast<double>(item.second);
        i++;
    }

    int ret = performNoOutlier(values, n, zscores);
    if (ret != 0) {
        throw std::runtime_error("NoOutlier test failed");
    }

    nb::dict result;
    for (size_t j = 0; j < n; j++) {
        nb::list entry;
        entry.append(values[j]);
        entry.append(zscores[j]);
        result[nb::cast(keys[j])] = entry;
    }
    return result;
}

NB_MODULE(fastgrubbstest, m) {
    m.def("run_Grubbs", &run_Grubbs, nb::arg("data"), nb::arg("alpha") = 0.05,
          "Grubbs test with iterative outlier removal. Returns {id: [value, zscore]}.");
    m.def("run_NoOutlier", &run_NoOutlier, nb::arg("data"),
          "Standard z-score with no outlier removal. Returns {id: [value, zscore]}.");
}
