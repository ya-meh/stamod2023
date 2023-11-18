#ifndef STATMOD_P_VAL_MODEL_RUNNER_H
#define STATMOD_P_VAL_MODEL_RUNNER_H

#include "model_runner.h"
#include "../view/p_val_graph_widget.h"

#include "../model/model_chen.h"
#include "../model/model_table.h"

#include <QWidget>


class PValuesModelRunner : public ModelRunner {
public:
    QWidget *new_graph_widget(QWidget *parent) override {
        return new PValueGraphWidget(dynamic_cast<ModelRunner *>(this), parent);
    }
};


class ChenPValuesModelRunner : public PValuesModelRunner {
    const Vector<Spec> SPEC = {{INT,    1, 1'000},
                               {INT,    1, 1'000},
                               {INT,    1, 1'000},
                               {INT,    1, 100'000},
                               {DOUBLE, 0, 1},
                               {INT,    0, 1'000'000'000}};

    Vector<double> last_valid;

public:
    Vector<double> gen(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto [msg, ok] = is_valid(data);
        if (!ok) {
            if (msg == "ignore") {
                return last_valid;
            }
            throw std::invalid_argument(msg.toStdString());
        }

        auto prob_len = to_int(data[0]);
        auto m = to_int(data[1]);
        auto exp_count = to_int(data[2]);
        auto p_vals = to_int(data[3]);
        auto deviation = to_double(data[4]);
        auto seed = to_int(data[5]);
        auto h_0_seed = SEED;
        if (deviation == 0) {
            h_0_seed = seed;
        }

        auto model = ChenModel(Vector<double>::generate_normal(prob_len, h_0_seed), m);

        return last_valid = (model
                                     .calc_p_values(exp_count, p_vals, model.deviate(deviation, seed))
                                     .count_buckets(count)
                                     .convert<double>()
                             / p_vals)
                .map([](auto el) { return 1 - el; })
                .reverse();
    }

    Config default_config() override {
        return Config{"Chen Stat Model\n(Type I/II Errors)",
                      {"Significance Level"},
                      {"P-Value"},
                      {
                              {"Prob. Len.", 20},
                              {"m", 10},
                              {"Exp. Num.", 200},
                              {"p-val. Num.", 1000},
                              {"Alt. Deviation", 0},
                              {"Seed", SEED},
                      }
        };
    }

    const Vector<Spec> &input_types_spec() const override { return SPEC; }

    int error_type(const Vector<Pair<QLabel *, QLineEdit *>> &data) const override {
        return 1 + (std::abs(to_double(data[4])) >= std::numeric_limits<double>::epsilon());
    }
};


class TablePValuesModelRunner : public PValuesModelRunner {
    const Vector<Spec> SPEC = {{INT, 1, 10'000},
                               {INT, 1, 10'000},
                               {INT, 1, 10'000},
                               {INT, 1, 100'000},
                               {INT, 0, 10'000},
                               {INT, 0, 1'000'000'000}};
    Vector<double> last_valid;

public:
    Vector<double> gen(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto [msg, ok] = is_valid(data);
        if (!ok) {
            if (msg == "ignore") {
                return last_valid;
            }
            throw std::invalid_argument(msg.toStdString());
        }

        auto k_len = to_int(data[0]);
        auto max = to_int(data[1]);
        auto exp_count = to_int(data[2]);
        auto p_vals = to_int(data[3]);
        auto deviation = to_double(data[4]);
        auto seed = to_int(data[5]);
        auto h_0_seed = SEED;
        if (deviation == 0) {
            h_0_seed = seed;
        }

        auto model = TableModel((Vector<double>::generate_floaty(k_len, h_0_seed) * max).convert_rounded<int>());

        return last_valid = (model
                                     .calc_p_values(exp_count, p_vals, model.deviate(deviation, seed))
                                     .count_buckets(count)
                                     .convert<double>()
                             / p_vals)
                .map([](auto el) { return 1 - el; })
                .reverse();
    }

    Config default_config() override {
        return Config{"Table Stat Model\n(Type I/II Errors)",
                      {"Significance Level"},
                      {"P-Value"},
                      {
                              {"K Len.", 20},
                              {"K Max.", 100},
                              {"Exp. Num.", 200},
                              {"p-val. Num.", 1000},
                              {"Alt. Deviation", 0},
                              {"Seed", SEED},
                      }
        };
    }

    const Vector<Spec> &input_types_spec() const override { return SPEC; }

    int error_type(const Vector<Pair<QLabel *, QLineEdit *>> &data) const override {
        return 1 + (std::abs(to_double(data[4])) >= std::numeric_limits<double>::epsilon());
    }
};


#endif //STATMOD_P_VAL_MODEL_RUNNER_H
