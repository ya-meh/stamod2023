#ifndef STATMOD_GRAPH_H
#define STATMOD_GRAPH_H

#include "graph_config.h"
#include "graph_input.h"

#include "../internal/vector.h"
#include "../internal/pair.h"
#include "../model/model.h"
#include "../model/model_chen.h"
#include "../internal/defer.h"

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


class TableGraphWidget : public QWidget {
    Config cfg;
    Vector<Pair<QLabel *, QLineEdit *>> labels;
    ModelRunner *model_runner;
    QAbstractAxis *x_axis, *y_axis;

public:
    explicit TableGraphWidget(ModelRunner *runner, QWidget *parent) : cfg(runner->default_config()),
                                                                      QWidget(parent),
                                                                      model_runner(runner),
                                                                      x_axis(cfg.x_axis.convert()),
                                                                      y_axis(cfg.y_axis.convert()) {
        auto *chart = new QChart();
        auto *chart_view = new QChartView(chart);

        auto *series = new QLineSeries();
        chart->addSeries(series);

        chart->addAxis(x_axis, Qt::AlignBottom);
        chart->addAxis(y_axis, Qt::AlignLeft);

        auto refresh_button = new QPushButton("Refresh");
        connect(refresh_button, &QPushButton::clicked, [=]() {
            Vector<double> p_values;

            try {
                p_values = model_runner->from(labels, cfg.points_n);
                switch (model_runner->error_type(labels)) {
                    case 1: {
                        chart->removeAxis(y_axis);
                        auto y_axis_cfg = cfg.y_axis;
                        y_axis_cfg.name = "Type I Error";
                        chart->addAxis(y_axis, Qt::AlignLeft);
                        break;
                    }
                    case 2: {
                        chart->removeAxis(y_axis);
                        auto y_axis_cfg = cfg.y_axis;
                        y_axis_cfg.name = "Type II Error";
                        chart->addAxis(y_axis, Qt::AlignLeft);
                        break;
                    }
                }
            } catch (const std::invalid_argument &e) {
                QMessageBox::information(parent, "An Error Occurred", e.what());
                return;
            }

            series->clear();

            auto delta = (cfg.x_axis.max - cfg.x_axis.min) / cfg.points_n;
            series->append({0, 0});
            for (size_t i = 0; i < cfg.points_n; ++i)
                series->append({(i + 1) * delta, p_values[i]});

            chart_view->show();
        });

        auto update_layout = [=]() {
            labels = cfg.labels_input();

            chart->setTitle(cfg.title);
            chart->legend()->hide();

            auto *layout = new QHBoxLayout();
            for (auto &[label, input]: labels) {
                layout->addWidget(label);
                layout->addWidget(input);
            }

            layout->addWidget(refresh_button);

            auto *main_layout = new QVBoxLayout();
            main_layout->addWidget(chart_view);
            main_layout->addLayout(layout);

            refresh_button->click();

            setLayout(main_layout);
        };

        update_layout();
    }
};

#endif //STATMOD_GRAPH_H
