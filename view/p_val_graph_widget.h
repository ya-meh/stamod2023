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
    QString title;

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

        auto title_str = cfg.title.toStdString();
        title = QString::fromStdString(title_str.substr(0, title_str.size() - 19));

        auto update_layout = [=]() {
            auto refresh_button = new QPushButton("Refresh");
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
            connect(refresh_button, &QPushButton::clicked, [=]() {
                labels.back().second->setText(QString::fromStdString(std::to_string((int) (rnd_static() * 10000))));
            });
            layout->addWidget(refresh_button);

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

        QString error_type;
        try {
            p_values = model_runner->gen(labels, cfg.points_n);

            switch (model_runner->error_type(labels)) {
                case 1: {
                    error_type = "I";
                    chart->addAxis(y_axis, Qt::AlignLeft);
                    break;
                }
                case 2: {
                    error_type = "II";
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
        series->append({1, 1});

        chart->addSeries(redLineSeries);
        chart->setTitle(title + ": Type " + error_type + " Error");
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
