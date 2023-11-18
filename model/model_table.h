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
    explicit TableModel(const Vector<int> &k);

    [[nodiscard]] Model *deviate(long double delta, size_t seed = SEED) override;

    [[nodiscard]] size_t model(std::mt19937 *rng = nullptr) const override;

    [[nodiscard]] const Vector<double> &probabilities() const override;

    size_t model(std::mt19937 rng) const;
};


#endif //STATMOD_MODEL_TABLE_H
