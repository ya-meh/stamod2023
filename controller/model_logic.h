#ifndef STATMOD_MODEL_LOGIC_H
#define STATMOD_MODEL_LOGIC_H


#include "../gui/graph_input.h"
#include "../model/model_chen.h"
#include "../model/model_table.h"


class ChenModelRunner : public ModelRunner {
    const Vector<int> SPEC = {INT, INT, INT, INT, DOUBLE};
public:
    Vector<double> from(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto [msg, ok] = is_valid(data);
        if (!ok)
            throw std::invalid_argument(msg.toStdString());

        auto prob_len = to_int(data[0]);
        auto m = to_int(data[1]);
        auto exp_count = to_int(data[2]);
        auto p_vals = to_int(data[3]);
        auto deviation = to_double(data[4]);

        auto model = ChenModel(Vector<double>::generate_normal(prob_len), m);

        return model
                       .calc_p_values(exp_count, p_vals, model.deviate(deviation))
                       .count_buckets(count)
                       .convert<double>()
               / p_vals;

    }

    Config default_config() override {
        return Config{"Chen Stat Model",
                      {"Threshold"},
                      {"P-Value"},
                      {
                              {"Prob. Len.", 40},
                              {"m", 10},
                              {"Experiments Num.", 200},
                              {"p-val Num.", 1000},
                              {"Max. Deviation", 0.025, Config::Field::DOUBLE},
                      }
        };
    }

    [[nodiscard]] const Vector<int> &input_types_spec() const override { return SPEC; }
};


class TableModelRunner : public ModelRunner {
    const Vector<int> SPEC = {INT, INT, INT, INT, DOUBLE};

public:
    Vector<double> from(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto [msg, ok] = is_valid(data);
        if (!ok)
            throw std::invalid_argument(msg.toStdString());

        auto k_len = to_int(data[0]);
        auto max = to_int(data[1]);
        auto exp_count = to_int(data[2]);
        auto p_vals = to_int(data[3]);
        auto deviation = to_double(data[4]);

        auto model = TableModel(Vector<int>::generate(k_len, max));

        return model
                       .calc_p_values(exp_count, p_vals, model.deviate(deviation))
                       .count_buckets(count)
                       .convert<double>()
               / p_vals;

    }

    Config default_config() override {
        return Config{"Table Stat Model",
                      {"Threshold"},
                      {"P-Value"},
                      {
                              {"K Len.", 15},
                              {"Max. Val.", 100},
                              {"Experiments Num.", 200},
                              {"p-val Num.", 1000},
                              {"Max. Deviation", 33},
                      }
        };
    }

    [[nodiscard]] const Vector<int> &input_types_spec() const override { return SPEC; }
};


#endif //STATMOD_MODEL_LOGIC_H
