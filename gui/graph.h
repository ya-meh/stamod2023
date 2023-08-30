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

QT_CHARTS_USE_NAMESPACE


class TableGraphWidget : public QWidget {
    Config cfg;
    Vector<Pair<QLabel *, QLineEdit *>> labels;
    ModelRunner *curr;

public:
    explicit TableGraphWidget(ModelRunner *runner,
                              QWidget *parent,
                              ModelRunner *next_run = nullptr) : cfg(runner->default_config()),
                                                                 QWidget(parent),
                                                                 curr(runner) {
        auto *chart = new QChart();
        auto *chart_view = new QChartView(chart);

        auto *series = new QLineSeries();
        chart->addSeries(series);

        chart->addAxis(cfg.x_axis.convert(), Qt::AlignBottom);
        chart->addAxis(cfg.y_axis.convert(), Qt::AlignLeft);

        auto refresh_button = new QPushButton("Refresh");
        connect(refresh_button, &QPushButton::clicked, [=]() {
            Vector<double> p_values;

            try {
                p_values = curr->from(labels, cfg.points_n);
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
