#ifndef STATMOD_MODEL_LOGIC_H
#define STATMOD_MODEL_LOGIC_H


#include "../gui/input.h"
#include "../model/model_chen.h"
#include "../model/model_table.h"


class ChenModelRunner : public ModelRunner {
public:
    Vector<double> from(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto prob_len = data[0].second->text().toInt();
        auto m = data[1].second->text().toInt();
        auto exp_count = data[2].second->text().toInt();
        auto p_vals = data[3].second->text().toInt();
        auto deviation = data[4].second->text().toDouble();

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
};


class TableModelRunner : public ModelRunner {
public:
    Vector<double> from(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto k_len =data[0].second->text().toInt();
        auto max =data[1].second->text().toInt();
        auto exp_count = data[2].second->text().toInt();
        auto p_vals = data[3].second->text().toInt();
        auto deviation = data[4].second->text().toDouble();

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
};


#endif //STATMOD_MODEL_LOGIC_H
