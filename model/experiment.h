#ifndef STATMOD_EXPERIMENT_H
#define STATMOD_EXPERIMENT_H


#include "../internal/vector.h"
#include "../internal/pair.h"
#include "model.h"
#include "stat.h"

using Experiment = Pair<size_t, double>;

class Experiments : public Vector<Experiment> {
    [[nodiscard]] double expected(size_t i) const {
        return (*this)[i].second;
    }

    [[nodiscard]] size_t observed(size_t i) const {
        return (*this)[i].first;
    }

public:
    Experiments(const Vector<size_t> &observed, const Vector<double> &expected) : Vector<Experiment>(observed.size()) {
        for (size_t i = 0; i < observed.size(); ++i) {
            (*this)[i] = {observed[i], i > expected.size() ? 0 : expected[i]};
        }
    }

    Experiments unionize(double floor = 5) {
        auto united = Experiments({}, {});

        auto bucket = Experiment{0, 0};
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
        if (bucket.first != 0 || bucket.second != 0) {
            if (united.empty()) {
                united.push_back({0, 0});
            }
            united.back() = united.back() + bucket;
        }

        return united;
    }

    [[nodiscard]] double chi_square() const {
        double chi_square = 0;
        for (size_t i = 0; i < size(); ++i)
            chi_square += sqr(static_cast<double>(observed(i)) - expected(i)) / expected(i);
        return chi_square;
    }

    [[nodiscard]] double p_value() const {
        return pChi(chi_square(), size() - 1);
    }
};


#endif //STATMOD_EXPERIMENT_H
