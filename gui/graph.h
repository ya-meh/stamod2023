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


class GraphWidget : public QWidget {
signals:

    void FormChanged(QEvent *event);

private:

    Config cfg;
    Vector<Pair<QLabel *, QLineEdit *>> labels;
    ModelRunner *model_runner;
    QAbstractAxis *x_axis, *y_axis;
    QWidget *parent;
    QChart *chart;
    QChartView *chart_view;
    QLineSeries *series;

protected:

    void changeEvent(QEvent *event) override {
//        emit FormCha
    }

public:
    explicit GraphWidget(ModelRunner *runner, QWidget *parent) : cfg(runner->default_config()),
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
            labels = cfg.labels_input();

            chart->setTitle(cfg.title);
            chart->legend()->hide();

            auto *layout = new QHBoxLayout();
            for (auto &[label, input]: labels) {
                layout->addWidget(label);
                layout->addWidget(input);
                connect(input, &QLineEdit::textChanged, this, &GraphWidget::refresh);
            }

            auto *main_layout = new QVBoxLayout();
            main_layout->addWidget(chart_view);
            main_layout->addLayout(layout);

            refresh();

            setLayout(main_layout);
        };

        update_layout();
    }

    void refresh() {
        Vector<double> p_values;

        try {
            p_values = model_runner->from(labels, cfg.points_n);
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

        series->clear();

        auto delta = (cfg.x_axis.max - cfg.x_axis.min) / cfg.points_n;
        series->append({0, 0});
        for (size_t i = 0; i < cfg.points_n; ++i)
            series->append({(i + 1) * delta, p_values[i]});

        chart_view->show();
    }
};

#endif //STATMOD_GRAPH_H
