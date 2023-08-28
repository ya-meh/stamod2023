#ifndef STATMOD_EXPERIMENT_H
#define STATMOD_EXPERIMENT_H


#include "../internal/vector.h"
#include "../internal/pair.h"


using Experiment = Pair<size_t, double>;

class Experiments : public Vector<Experiment> {
    [[nodiscard]] double expected(size_t i) const;

    [[nodiscard]] size_t observed(size_t i) const;

public:
    Experiments(const Vector<size_t> &observed, const Vector<double> &expected);

    Experiments unionize(double floor = 5);

    [[nodiscard]] double chi_square() const;

    [[nodiscard]] double p_value() const;
};


#endif //STATMOD_EXPERIMENT_H
