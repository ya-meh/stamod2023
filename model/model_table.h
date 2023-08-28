#ifndef STATMOD_MODEL_TABLE_H
#define STATMOD_MODEL_TABLE_H


#include "../internal/vector.h"
#include "experiment.h"
#include "model.h"

#include <algorithm>


class TableModel : public Model {
    int sum_ = 0;
    Vector<size_t> table_;
    Vector<double> probabilities_;
    const Vector<int> k;

public:
    explicit TableModel(const Vector<int> &k) : sum_(k.sum()), table_(sum_), probabilities_(k.size()), k(k) {
        size_t i = 1, j = 1, l = k[0];
        std::cout << k.to_string() << std::endl;
        while (l <= sum_) {
            while (i <= l) {
                table_[i - 1] = j;
                i = i + 1;
            }
            j = j + 1;
            l = l + k[(j - 1) % k.size()];
        }

        probabilities_ = k.convert<double>() / sum_;
    }

    [[nodiscard]] Model *deviate(long double delta) override {
        auto model = new TableModel{k.deviate(static_cast<int>(delta))};
        return model;
    }

    [[nodiscard]] size_t model(double alpha) const override {
        auto d = static_cast<size_t>(alpha * sum_);
        return table_[std::min(static_cast<int>(d), sum_ - 1)];
    }

    [[nodiscard]] const Vector<double> &probabilities() const override {
        return probabilities_;
    }
};


#endif //STATMOD_MODEL_TABLE_H
