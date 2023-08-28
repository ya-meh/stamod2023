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

    [[nodiscard]] Model *deviate(long double delta) override;

    [[nodiscard]] size_t model(double alpha) const override;

    [[nodiscard]] const Vector<double> &probabilities() const override;
};


#endif //STATMOD_MODEL_TABLE_H
