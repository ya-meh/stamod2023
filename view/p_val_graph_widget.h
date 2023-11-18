#ifndef STATMOD_P_VAL_GRAPH_WIDGET_H
#define STATMOD_P_VAL_GRAPH_WIDGET_H

#include "graph_config.h"
#include "../controller/model_runner.h"

#include "../internal/vector.h"
#include "../internal/pair.h"
#include "../model/model.h"

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
#include <QScatterSeries>

class PValueGraphWidget : public QWidget {
protected:
    Config cfg;
    Vector<Pair<QLabel *, QLineEdit *>> labels;
    ModelRunner *model_runner;
    QAbstractAxis *x_axis, *y_axis;
    QWidget *parent;
    QChart *chart;
    QChartView *chart_view;
    QLineSeries *series;

    void clear_labels() {
        for (auto [label, edit]: labels) {
            delete label;
            delete edit;
        }
    }

public:
    explicit PValueGraphWidget(ModelRunner *runner, QWidget *parent) : cfg(runner->default_config()),
                                                                       QWidget(parent),
                                                                       model_runner(runner),
                                                                       x_axis(cfg.x_axis.convert()),
                                                                       y_axis(cfg.y_axis.convert()) {
        this->parent = parent;
        chart = new QChart();
        chart_view = new QChartView(chart);

        series = new QLineSeries();
        chart->addSeries(series);

        chart->addAxis(x_axis, Qt::AlignBottom);
        chart->addAxis(y_axis, Qt::AlignLeft);

        auto update_layout = [=]() {
            clear_labels();
            labels = cfg.labels_input();

            chart->setTitle(cfg.title);
            chart->legend()->hide();

            auto *layout = new QHBoxLayout();
            for (auto &[label, input]: labels) {
                layout->addWidget(label);
                layout->addWidget(input);
                connect(input, &QLineEdit::textChanged, this, &PValueGraphWidget::refresh);
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
        Vector<double> p_values;

        try {
            p_values = model_runner->gen(labels, cfg.points_n);
            if (p_values.size() > 100'000) { p_values = p_values.sub_array(0, 100'000); }

            switch (model_runner->error_type(labels)) {
                case 1: {
                    chart->removeAxis(y_axis);
                    y_axis->setTitleText("Type I Error");
                    chart->addAxis(y_axis, Qt::AlignLeft);
                    break;
                }
                case 2: {
                    chart->removeAxis(y_axis);
                    y_axis->setTitleText("Type II Error");
                    chart->addAxis(y_axis, Qt::AlignLeft);
                    break;
                }
            }
        } catch (const std::invalid_argument &e) {
            QMessageBox::information(this->parent, "An Error Occurred", e.what());
            return;
        }

        auto *redLineSeries = new QLineSeries();
        redLineSeries->setColor(Qt::red);
        redLineSeries->append(0, 0);
        redLineSeries->append(1, 1);

        series->clear();

        auto delta = (cfg.x_axis.max - cfg.x_axis.min) / cfg.points_n;
        series->append({0, 0});
        for (size_t i = 0; i < cfg.points_n; ++i)
            series->append({(i + 1) * delta, p_values[i]});

        chart->addSeries(redLineSeries);
        chart_view->show();
    }

    ~PValueGraphWidget() override {
        delete x_axis;
        delete y_axis;
        delete chart;
        delete chart_view;
    }
};

#endif //STATMOD_P_VAL_GRAPH_WIDGET_H
