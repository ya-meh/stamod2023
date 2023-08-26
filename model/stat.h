//
// Created by Mikhail Kiselev on 26.08.2023.
//

#ifndef STATMOD_STAT_H
#define STATMOD_STAT_H

#include "vector.h"
#include "probdist.h"

#include <algorithm>

template<typename T>
Vector<double> to_probabilities(const Vector <T> &vec) {
    auto probabilities = Vector<double>(vec.size());
    T sum = std::accumulate(begin(vec), end(vec), static_cast<T>(0));
    for (size_t i = 0; i < vec.size(); ++i)
        probabilities[i] = static_cast<double>(vec[i]) / sum;
    return probabilities;
}

template<typename T>
T sqr(const T &val) {
    return val * val;
}

template<typename T>
double calc_chi_square(const Vector<T> &obs, const Vector<double> &exp) {
    double chi_square = 0;
    for (size_t i = 0; i < obs.size(); ++i)
        chi_square += sqr(static_cast<double>(obs[i]) - exp[i]) / exp[i];
    return chi_square;
}

#endif //STATMOD_STAT_H
