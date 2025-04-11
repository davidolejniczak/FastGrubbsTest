#include <iostream>
#include <cstdlib>
#include <vector>
#include "mainFunctions.hpp"
#include <pybind11/pybind11.h> 
#include <pybind11/stl.h>
#include <stddef.h>
#include <memory>

namespace py = pybind11;

class BaseConfig {
protected:
    bool useList;
    bool useId;

public:
    BaseConfig() : useList(true), useId(false) {}

    bool getUseList() const { return useList; }
    void setUseList(bool value) {
        this->useList = value;  
    }

    bool getUseId() const { return useId; }
    void setUseId(bool value) {
        this->useId = value;
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
                if (useId) {
                    py::tuple valueZscore = py::make_tuple(values[i], zscores[i]);
                    resultDict[ids[i]] = valueZscore;
                } else {
                    resultDict[py::float_(values[i])] = zscores[i];
                }
            }
            return resultDict;
        }
    }
};

class GrubbsConfig : public BaseConfig {
private:
    double alpha;

public:
    GrubbsConfig() : BaseConfig(), alpha(0.05) {}

    double getAlpha() const { return alpha; }
    void setAlpha(double a) {
        if (a < 0.0 || a > 1.0) {
            std::cerr << "Error: Alpha must be between 0 and 1" << std::endl;
            return;
        }
        alpha = a;
    }

    py::object runGrubbs(const py::object& input) const {
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
        std::shared_ptr<double[]> valuesArray(new double[size]);
        std::shared_ptr<double[]> zscores(new double[size]);

        for (std::size_t i = 0; i < size; i++) {
            valuesArray[i] = values[i];
        }

        std::shared_ptr<double[]> finalValues = nullptr;

        std::size_t finalSize = 0;
        int status = performGrubbs(valuesArray, size, finalValues, &finalSize, zscores, alpha);

        if (status != 0) {
            return py::none();
        }

        std::vector<double> zscoreVector(zscores.get(), zscores.get() + size); //can maybe change to span
        py::object result = formatOutput(ids, values, zscoreVector);

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
        std::shared_ptr<double[]> valuesArray(new double[size]);
        std::shared_ptr<double[]> zscores(new double[size]); 

        for (std::size_t i = 0; i < size; i++) {
            valuesArray[i] = values[i];
        }

        int status = performNoOutlier(valuesArray, size, zscores);

        if (status != 0) {
            return py::none();
        }

        std::vector<double> zscoreVector(zscores.get(), zscores.get() + size); //can maybe change to span 
        py::object result = formatOutput(ids, values, zscoreVector);

        return result;
    }
};

PYBIND11_MODULE(_grubbstest_impl, m) {
    m.doc() = "Fast Grubbs Test.";

    py::class_<GrubbsConfig>(m, "GrubbsConfig")
        .def(py::init<>())
        .def_property("Alpha", &GrubbsConfig::getAlpha,&GrubbsConfig::setAlpha, "Alpha setting")
        .def_property("UseList", &GrubbsConfig::getUseList,&GrubbsConfig::setUseList,"Use List Output, True for list, False for dict")
        .def_property("UseId", &GrubbsConfig::getUseId,&GrubbsConfig::setUseId,"Use ID Field, True for ID, False for no ID")
        .def("runGrubbs", &GrubbsConfig::runGrubbs, "Return standardised deviate of each data point using Grubb's test");

    py::class_<NoOutlierConfig>(m, "NoOutlierConfig")
        .def(py::init<>())
        .def_property("UseList", &NoOutlierConfig::getUseList,&NoOutlierConfig::setUseList,"Use List Output, True for list, False for dict")
        .def_property("UseId", &NoOutlierConfig::getUseId,&NoOutlierConfig::setUseId,"Use ID Field, True for ID, False for no ID")
        .def("runNoOutlier", &NoOutlierConfig::runNoOutlier, "Returns standardised deviate of each data point");
}