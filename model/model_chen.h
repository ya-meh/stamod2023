#ifndef STATMOD_MODEL_CHEN_H
#define STATMOD_MODEL_CHEN_H

#include "model.h"
#include "../internal/vector.h"


class ChenModel : public Model {
    Vector<double> probabilities_;
    Vector<double> sums_;
    Vector<double> additional_;
    size_t m;
public:
    const Vector<int> k;

    explicit ChenModel(const Vector<double> &prob, size_t m);

    [[nodiscard]] size_t model(std::mt19937 *rng) const override;

    [[nodiscard]] const Vector<double> &probabilities() const override;

    [[nodiscard]] Model *deviate(long double delta) override;;
};


#endif //STATMOD_MODEL_CHEN_H
