#include <iostream>
#include <cstdlib>
#include <vector>
#include "mainFunctions.h"
#include <pybind11/pybind11.h> 
#include <pybind11/stl.h>
#include <stddef.h>

namespace py = pybind11;

class BaseConfig {
protected:
    bool useList;
    bool useId;

public:
    BaseConfig() : useList(true), useId(false) {}

    bool getUseList() const { return useList; }
    void setUseList(bool useList) {
        useList = useList;
    }

    bool getUseId() const { return useId; }
    void setUseId(bool useId) {
        useId = useId;
    }

    bool processInput(const py::object& input,
                      std::vector<py::object>& ids,
                      std::vector<double>& values) const {
        try {
            if (py::isinstance<py::list>(input)) {
                py::list listInput = input.cast<py::list>();

                for (auto item : listInput) {
                    if (py::isinstance<py::list>(item) || py::isinstance<py::tuple>(item)) {
                        auto insideList = item.cast<py::list>();
                        std::size_t len = insideList.size();

                        if (len >= 2 && useId) {
                            ids.push_back(insideList[0]);
                            values.push_back(insideList[1].cast<double>());
                        } else if (len >= 1) {
                            if (useId) {
                                ids.push_back(py::int_(ids.size()));
                            }
                            values.push_back(insideList[0].cast<double>());
                        }
                    } else {
                        if (useId) {
                            ids.push_back(py::int_(ids.size()));
                        }
                        values.push_back(item.cast<double>());
                    }
                }
            }
            else if (py::isinstance<py::dict>(input)) {
                py::dict dictInput = input.cast<py::dict>();

                for (auto item : dictInput) {
                    py::handle keyVar = item.first;
                    py::handle valueVar = item.second;

                    py::object key = py::reinterpret_borrow<py::object>(keyVar);
                    py::object value = py::reinterpret_borrow<py::object>(valueVar);

                    ids.push_back(key);

                    if (py::isinstance<py::tuple>(value) || py::isinstance<py::list>(value)) {
                        auto tuple_or_list = value.cast<py::tuple>();
                        if (tuple_or_list.size() >= 1) {
                            values.push_back(tuple_or_list[0].cast<double>());
                        }
                    } else {
                        values.push_back(value.cast<double>());
                    }
                }
            }
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error processing input data: " << e.what() << std::endl;
            return false;
        }
    }

    py::object formatOutput(const std::vector<py::object>& ids,
                           const std::vector<double>& values,
                           const std::vector<double>& zscores) const {
        std::size_t size = values.size();
        if (size == 0) {
            return useList ? py::cast<py::object>(py::list()) : py::cast<py::object>(py::dict());
        }

        if (useList) {
            py::list resultList;
            for (std::size_t i = 0; i < size; i++) {
                py::list item;
                if (useId) {
                    item.append(ids[i]);
                }
                item.append(values[i]);
                item.append(zscores[i]);
                resultList.append(item);
            }
            return resultList;
        } else {
            py::dict resultDict;
            for (std::size_t i = 0; i < size; i++) {
                py::tuple valueZscore = py::make_tuple(values[i], zscores[i]);
                resultDict[ids[i]] = valueZscore;
            }
            return resultDict;
        }
    }
};


class JackKnifeConfig : public BaseConfig {
private:
    double percentile;
    double dof1;

public:
    JackKnifeConfig() : BaseConfig(), percentile(0.95), dof1(3.0) {}

    double getPercentile() const { return percentile; }
    void setPercentile(double p) {
        if (p < 0.0 || p > 1.0) {
            std::cerr << "Error: Percentile must be between 0 and 1" << std::endl;
            return;
        }
        percentile = p;
    }

    double getDof1() const { return dof1; }
    void setDof1(double d) {
        if (d <= 0.0) {
            std::cerr << "Error: Dof1 must be greater than 0"<< std::endl;
            return;
        }
        dof1 = d;
    }

    py::object runJackknife(const py::object& input) const {
        std::vector<py::object> ids;
        std::vector<double> values;

        if (!processInput(input, ids, values)) {
            return py::none();
        }

        if (values.empty()) {
            return useList ? py::cast<py::object>(py::list()) : py::cast<py::object>(py::dict());
        }

        if (!useId) {
            ids.clear();
            for (std::size_t i = 0; i < values.size(); i++) {
                ids.push_back(py::int_(i));
            }
        }

        std::size_t size = values.size();
        double* valuesArray = (double*)std::malloc(size * sizeof(double));
        double* zscores = (double*)std::malloc(size * sizeof(double));

        if (!valuesArray || !zscores) {
            std::cerr << "Error: Failed to allocate memory for jackknife" << std::endl;
            if (valuesArray) std::free(valuesArray);
            if (zscores) std::free(zscores);
            return py::none();
        }

        for (std::size_t i = 0; i < size; i++) {
            valuesArray[i] = values[i];
        }

        double* finalValues = nullptr;
        std::size_t finalSize = 0;
        int status = performIterativeJackknife(valuesArray, size, &finalValues, &finalSize, zscores, percentile, dof1);

        if (status != 0) {
            std::free(valuesArray);
            std::free(zscores);
            if (finalValues) std::free(finalValues);
            return py::none();
        }

        std::vector<double> zscoreVector(zscores, zscores + size);
        py::object result = formatOutput(ids, values, zscoreVector);

        std::free(valuesArray);
        std::free(zscores);
        if (finalValues) std::free(finalValues);

        return result;
    }
};


class NoOutlierConfig : public BaseConfig {
public:
    NoOutlierConfig() : BaseConfig() {}
    py::object runNoOutlier(const py::object& input) const {
        std::vector<py::object> ids;
        std::vector<double> values;

        if (!processInput(input, ids, values)) {
            return py::none();
        }

        if (values.empty()) {
            return useList ? py::cast<py::object>(py::list()) : py::cast<py::object>(py::dict());
        }

        if (!useId) {
            ids.clear();
            for (std::size_t i = 0; i < values.size(); i++) {
                ids.push_back(py::int_(i));
            }
        }

        std::size_t size = values.size();
        double* valuesArray = (double*)std::malloc(size * sizeof(double));
        double* zscores = (double*)std::malloc(size * sizeof(double));

        if (!valuesArray || !zscores) {
            std::cerr << "Error: Failed to allocate memory for noOutlier" << std::endl;
            if (valuesArray) std::free(valuesArray);
            if (zscores) std::free(zscores);
            return py::none();
        }

        for (std::size_t i = 0; i < size; i++) {
            valuesArray[i] = values[i];
        }

        int status = performNoOutlier(valuesArray, size, zscores);

        if (status != 0) {
            std::free(valuesArray);
            std::free(zscores);
            return py::none();
        }

        std::vector<double> zscoreVector(zscores, zscores + size);
        py::object result = formatOutput(ids, values, zscoreVector);

        std::free(valuesArray);
        std::free(zscores);

        return result;
    }
};

PYBIND11_MODULE(main, JKI) {
    JKI.doc() = "Iterative JackKnife module for identifying outliers and calculating zscores.";

    py::class_<JackKnifeConfig>(JKI, "JackKnifeConfig")
        .def(py::init<>())
        .def_property("Percentile", &JackKnifeConfig::getPercentile,&JackKnifeConfig::setPercentile, "Percentile setting")
        .def_property("Dof1", &JackKnifeConfig::getDof1,&JackKnifeConfig::setDof1,"Dof1 value")
        .def_property("UseList", &JackKnifeConfig::getUseList,&JackKnifeConfig::setUseList,"Use List Output, True for list, False for dict")
        .def_property("UseId", &JackKnifeConfig::getUseId,&JackKnifeConfig::setUseId,"Use ID Field, True for ID, False for no ID")
        .def("runJackknife", &JackKnifeConfig::runJackknife, "Return standardised deviate of each data point using Iterative JackKnife");

    py::class_<NoOutlierConfig>(JKI, "NoOutlierConfig")
        .def(py::init<>())
        .def_property("UseList", &JackKnifeConfig::getUseList,&JackKnifeConfig::setUseList,"Use List Output, True for list, False for dict")
        .def_property("UseId", &JackKnifeConfig::getUseId,&JackKnifeConfig::setUseId,"Use ID Field, True for ID, False for no ID")
        .def("runNoOutlier", &NoOutlierConfig::runNoOutlier, "Returns standardised deviate of each data point");
}