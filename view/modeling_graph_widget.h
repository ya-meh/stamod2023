#ifndef STATMOD_MODELING_GRAPH_WIDGET_H
#define STATMOD_MODELING_GRAPH_WIDGET_H


#include "graph_config.h"
#include "../controller/model_runner.h"

#include "../internal/vector.h"
#include "../internal/pair.h"
#include "../model/experiment.h"
#include "../view/comparing_histogram.h"

#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QMessageBox>

#include <iostream>
#include <utility>


class ModelingGraphWidget : public QWidget {
protected:
    Config cfg;
    Vector<Pair<QLabel *, QLineEdit *>> labels;
    ModelRunner *model_runner;
    QChart *chart = nullptr;
    QChartView *chart_view = nullptr;
    QLineSeries *series;
    QWidget *parent;

    void clear_labels() {
        for (auto [label, edit]: labels) {
            delete label;
            delete edit;
        }
    }


    QChart *build_hist() {
        auto start = std::chrono::steady_clock::now();
        auto [obs, exp] = model_runner->gen_comparing(labels, 0);
        auto elapsed = static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - start).count()) / 1'000'000;
        auto elapsed_str = QString::fromStdString(std::to_string(elapsed));

        auto results = Experiments(obs, exp);
        auto chi_sq = QString::fromStdString(std::to_string(results.chi_square()));
        auto p_value = QString::fromStdString(std::to_string(results.p_value()));
        auto df = QString::fromStdString(std::to_string(obs.size() - 1));

        return HistogramChart::build_generic(
                obs, "> Observed",
                exp, "> Expected",
                Vector<std::string>::with_generator_i(obs.size(), [](size_t i) {
                    return std::to_string(i + 1);
                }),
                cfg.title + ": p_value=" + p_value + ", chi_sq=" + chi_sq + ", d.f.=" + df + " (" + elapsed_str + "ms)"
        );
    }

public:
    explicit ModelingGraphWidget(ModelRunner *runner, QWidget *parent) : cfg(runner->default_config()),
                                                                         QWidget(parent),
                                                                         model_runner(runner) {
        this->parent = parent;

        chart = new QChart();
        chart_view = new QChartView(chart);
        series = new QLineSeries();
        chart->addSeries(series);

        auto update_layout = [=]() {
            clear_labels();
            labels = cfg.labels_input();

            chart->setTitle(cfg.title);
            chart->legend()->hide();

            auto *layout = new QHBoxLayout();
            for (auto &[label, input]: labels) {
                layout->addWidget(label);
                layout->addWidget(input);
                connect(input, &QLineEdit::textChanged, this, &ModelingGraphWidget::refresh);
            }

            auto *main_layout = new QVBoxLayout();
            main_layout->addWidget(chart_view);
            main_layout->addLayout(layout);
            setLayout(main_layout);
        };

        update_layout();
        refresh();
    }

    void refresh() {
        try {
            auto hist = build_hist();
            chart_view->setChart(hist);
            chart_view->show();
        } catch (std::invalid_argument &e) {
            QMessageBox::information(this->parent, "An Error Occurred", e.what());
            return;
        }
    }
};


#endif //STATMOD_MODELING_GRAPH_WIDGET_H
