#include "model_chen.h"

ChenModel::ChenModel(const Vector<double> &prob, size_t m) : m(m),
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

size_t ChenModel::model(std::mt19937 *rng) const {
    if (rng == nullptr) {
        rng = &rng_;
    }

    double alpha = rnd(*rng);
    size_t j = static_cast<double>(m) * alpha + 1, i = static_cast<size_t>(additional_[j - 1]);
    while (alpha > sums_[i - 1])
        i += 1;
    return i;
}

const Vector<double> &ChenModel::probabilities() const {
    return probabilities_;
}

Model *ChenModel::deviate(long double delta, size_t seed) {
    return new ChenModel{probabilities_.deviate<double>(delta, seed).normalize(), m};
}
