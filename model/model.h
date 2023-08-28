#ifndef STATMOD_MODEL_H
#define STATMOD_MODEL_H


#include "../internal/vector.h"
#include "experiment.h"

#include <cstddef>
#include <algorithm>
#include <numeric>


// Model -- interface for stat model
class Model {
public:
    [[nodiscard]] virtual size_t model(double alpha) const = 0;

    [[nodiscard]] virtual const Vector<double> &probabilities() const = 0;

    [[nodiscard]] virtual Model *deviate(long double delta) { return this; };

    [[nodiscard]] Vector<size_t> raw_experiment(size_t count) const {
        auto result_to_count = Vector<size_t>(probabilities().size());
        for (size_t i = 0; i < count; ++i) {
            result_to_count[std::min(model(rnd()), probabilities().size()) - 1] += 1;
        }
        return result_to_count;
    }

    [[nodiscard]] Vector<double> calc_p_values(size_t experiments_n, size_t p_vals_n, Model *alt = nullptr) {
        auto p_values = Vector<double>(p_vals_n);

        for (size_t i = 0; i < p_vals_n; ++i) {
            p_values[i] = experiment(experiments_n, alt)
                    .unionize()
                    .p_value();
        }

        return p_values;
    }

    [[nodiscard]] Experiments experiment(size_t count, const Model *alt = nullptr) const {
        if (alt == nullptr)
            alt = this;

        return {(*alt).raw_experiment(count), probabilities() * count};
    }

    virtual ~Model() = default;
};


#endif //STATMOD_MODEL_H
