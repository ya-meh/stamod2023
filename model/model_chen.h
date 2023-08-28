//
// Created by Mikhail Kiselev on 28.08.2023.
//

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

    explicit ChenModel(const Vector<double> &prob, size_t m) : m(m),
                                                               sums_(prob.size()),
                                                               additional_(m),
                                                               probabilities_{prob} {
        sums_[0] = prob[0];
        for (size_t i = 1; i < prob.size(); ++i)
            sums_[i] = sums_[i - 1] + prob[i];

        size_t i = 1;
        for (size_t j = 1; j <= m; ++j) {
            while (static_cast<double>(m) * sums_[i - 1] + 1 <= static_cast<double>(j))
                i += 1;
            additional_[j - 1] = static_cast<double>(i);
        }
    }

    [[nodiscard]] size_t model(double alpha) const override {
        size_t j = static_cast<double>(m) * alpha + 1, i = static_cast<size_t>(additional_[j - 1]);
        while (alpha > sums_[i - 1])
            i += 1;
        return i;
    }

    [[nodiscard]] const Vector<double> &probabilities() const override {
        return probabilities_;
    }

    [[nodiscard]] virtual Model *deviate(long double delta) {
        return new ChenModel{probabilities_.deviate<double>(delta).normalize(), m};
    };
};


#endif //STATMOD_MODEL_CHEN_H
