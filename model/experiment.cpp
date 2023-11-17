#include "experiment.h"

#include "../internal/probdist.h"


template<typename T>
T sqr(const T &val) {
    return val * val;
}

Experiments::Experiments(const Vector<size_t> &observed, const Vector<double> &expected) : Vector<SingleExperiment>(
        observed.size()) {
    for (size_t i = 0; i < observed.size(); ++i) {
        (*this)[i] = {observed[i], i > expected.size() ? 0 : expected[i]};
    }
}

Experiments Experiments::unionize(double floor) {
    auto united = Experiments({}, {});

    auto bucket = SingleExperiment{0, 0};
    for (size_t i = 0; i < size(); ++i) {
        if (expected(i) > floor) {
            united.push_back((*this)[i]);
        } else {
            bucket = bucket + (*this)[i];
            if (bucket.second >= floor) {
                united.push_back(bucket);
                bucket = {0, 0};
            }
        }
    }
    if (bucket == SingleExperiment{0, 0}) {
        if (united.empty()) {
            united.push_back({0, 0});
        }
        united.back() = united.back() + bucket;
    }

    return united;
}

double Experiments::chi_square() const {
    double chi_square = 0;
    for (size_t i = 0; i < size(); ++i)
        chi_square += sqr(static_cast<double>(observed(i)) - expected(i)) / expected(i);
    return chi_square;
}

double Experiments::p_value() const {
    return 1 - pChi(chi_square(), size() - 1);
}

size_t Experiments::observed(size_t i) const {
    return (*this)[i].first;
}

double Experiments::expected(size_t i) const {
    return (*this)[i].second;
}
