#ifndef STATMOD_MODELING_MODEL_RUNNER_H
#define STATMOD_MODELING_MODEL_RUNNER_H

#include "model_runner.h"
#include "../view/modeling_graph_widget.h"

#include "../model/model_chen.h"
#include "../model/model_table.h"


class ModelingModelRunner : public ModelRunner {
public:
    QWidget *new_graph_widget(QWidget *parent) override {
        return new ModelingGraphWidget(dynamic_cast<ModelRunner *>(this), parent);
    }
};

class ChenModelingModelRunner : public ModelingModelRunner {
    const Vector<Spec> SPEC = {
            {INT, 1, 10'000},
            {INT, 1, 10'000},
            {INT, 0, 10'000},
            {INT, 1, 10'000'000},
    };
    Pair<Vector<size_t>, Vector<double>> last_valid;

public:
    Config default_config() override {
        return Config{"Chen Stat Modeling",
                      {"X"},
                      {"Value"},
                      {
                              {"Prob. Vector Len.", 10},
                              {"m", 10},
                              {"Seed", SEED},
                              {"Experiments Count", 1000},
                      }
        };
    }

    const Vector<Spec> &input_types_spec() const override {
        return SPEC;
    }

    int error_type(const Vector<Pair<QLabel *, QLineEdit *>> &data) const override {
        return ModelRunner::error_type(data);
    }

    Pair<Vector<size_t>, Vector<double>>
    gen_comparing(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto [msg, ok] = is_valid(data);
        if (!ok) {
            if (msg == "ignore") {
                return last_valid;
            }
            throw std::invalid_argument(msg.toStdString());
        }

        auto prob_len = to_int(data[0]);
        auto m = to_int(data[1]);
        auto seed = to_int(data[2]);
        auto cnt = to_int(data[3]);

        auto model = ChenModel(Vector<double>::generate_normal(prob_len, seed), m);
        return last_valid = {model.raw_experiment(cnt), model.probabilities() * cnt};
    }

    QWidget *new_graph_widget(QWidget *parent) override {
        return ModelingModelRunner::new_graph_widget(parent);
    }
};

class TableModelingModelRunner : public ModelingModelRunner {
    const Vector<Spec> SPEC = {
            {INT, 1, 10'000},
            {INT, 1, 1000},
            {INT, 0, 10'000},
            {INT, 1, 10'000'000},
    };
    Pair<Vector<size_t>, Vector<double>> last_valid;

public:
    Config default_config() override {
        return Config{"Table Stat Modeling",
                      {"X"},
                      {"Value"},
                      {
                              {"k Vector Len.", 10},
                              {"k Vector Max. Val.", 100},
                              {"Seed", SEED},
                              {"Experiments Count", 1000},
                      }
        };
    }

    const Vector<Spec> &input_types_spec() const override {
        return SPEC;
    }

    int error_type(const Vector<Pair<QLabel *, QLineEdit *>> &data) const override {
        return ModelRunner::error_type(data);
    }

    Pair<Vector<size_t>, Vector<double>>
    gen_comparing(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) override {
        auto [msg, ok] = is_valid(data);
        if (!ok) {
            if (msg == "ignore") {
                return last_valid;
            }
            throw std::invalid_argument(msg.toStdString());
        }

        auto prob_len = to_int(data[0]);
        auto max_val = to_int(data[1]);
        auto seed = to_int(data[2]);
        auto cnt = to_int(data[3]);

        auto model = TableModel(Vector<int>::generate(prob_len, max_val, seed));
        return last_valid = {model.raw_experiment(cnt), model.probabilities() * cnt};
    }

    QWidget *new_graph_widget(QWidget *parent) override {
        return ModelingModelRunner::new_graph_widget(parent);
    }
};


#endif //STATMOD_MODELING_MODEL_RUNNER_H
