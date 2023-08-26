#ifndef STATMOD_MODEL_TABLE_H
#define STATMOD_MODEL_TABLE_H


#include "../internal/vector.h"
#include "experiment.h"

#include <algorithm>


class Model15 {
    int sum_ = 0;
    Vector<size_t> table_;
    Vector<double> probabilities_;
public:
    explicit Model15(const Vector<int> &k) : sum_(k.sum()), table_(sum_), probabilities_(k.size()) {
        size_t i = 1, j = 1, l = k[0];
        while (l <= sum_) {
            while (i <= l) {
                table_[i - 1] = j;
                i = i + 1;
            }
            j = j + 1;
            l = l + k[j - 1];
        }

        for (size_t i = 0; i < k.size(); ++i)
            probabilities_[i] = static_cast<double>(k[i]) / sum_;
    }

    [[nodiscard]] size_t model(double alpha) const {
        auto d = static_cast<size_t>(alpha * sum_);
        return table_[std::min(static_cast<int>(d), sum_ - 1)];
    }

    [[nodiscard]] Vector<size_t> raw_experiment(size_t count, size_t up_to) const {
        auto result_to_count = Vector<size_t>(up_to);
        for (size_t i = 0; i < count; ++i) {
            auto result = model(rnd());
            if (result > up_to)
                result = up_to;
            result_to_count[result - 1] += 1;
        }
        return result_to_count;
    }

    [[nodiscard]] Experiments experiment(size_t count, size_t up_to) const {
        return {raw_experiment(count, up_to), probabilities() * count};
    }

    [[nodiscard]] const Vector<double> &probabilities() const {
        return probabilities_;
    }
};

#endif //STATMOD_MODEL_TABLE_H
