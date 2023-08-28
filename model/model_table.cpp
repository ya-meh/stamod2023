#include "model_table.h"

TableModel::TableModel(const Vector<int> &k) : sum_(k.sum()), table_(sum_), probabilities_(k.size()), k(k) {
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

Model *TableModel::deviate(long double delta) {
    auto model = new TableModel{k.deviate(static_cast<int>(delta))};
    return model;
}

size_t TableModel::model(double alpha) const {
    auto d = static_cast<size_t>(alpha * sum_);
    return table_[std::min(static_cast<int>(d), sum_ - 1)];
}

const Vector<double> &TableModel::probabilities() const {
    return probabilities_;
}
