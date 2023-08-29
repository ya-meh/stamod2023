#ifndef STATMOD_GRAPH_TABLE_H
#define STATMOD_GRAPH_TABLE_H

#include "../internal/vector.h"
#include "../internal/pair.h"
#include "../model/model.h"
#include "../model/model_chen.h"

#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QValueAxis>
#include <utility>
#include <QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

class Config {
public:
    struct Axis {
        QString name{};
        qreal min = 0;
        qreal max = 1;

        [[nodiscard]] QValueAxis *convert() const {
            auto axis = new QValueAxis;

            axis->setTitleText(name);
            axis->setMin(min);
            axis->setMax(max);

            return axis;
        }
    };

    struct Field {
        QString name{};
        qreal value{};
        int type = INT;

        const static int INT = 0;
        const static int DOUBLE = 1;

        [[nodiscard]] QLabel *label() const { return new QLabel(name); }

        [[nodiscard]] QLineEdit *edit_field() const {
            auto str = QString::fromStdString(std::to_string(type == DOUBLE ? value : static_cast<int>(value)));
            auto edit = new QLineEdit(str);
//            edit.int
            return new QLineEdit(str);
        }
    };


    QString title;
    Axis x_axis = {"X"};
    Axis y_axis = {"Y"};
    // name + default value
    Vector<Field> fields = {};

    [[nodiscard]] Vector<Pair<QLabel *, QLineEdit *>> labels_input() const {
        auto labels = Vector<Pair<QLabel *, QLineEdit *>>(fields.size());

        for (size_t i = 0; i < labels.size(); ++i)
            labels[i] = {fields[i].label(), fields[i].edit_field()};

        return labels;
    }
};

class TableGraphWidget : public QWidget {
    Config cfg;
public:
    explicit TableGraphWidget(Config config, QWidget *parent) : cfg(std::move(config)), QWidget(parent) {
        // Create the chart and chart view
        auto *chart = new QChart();
        auto *chartView = new QChartView(chart);

        // Create the series for the graph data
        auto *series = new QLineSeries();
        chart->addSeries(series);

        chart->addAxis(cfg.x_axis.convert(), Qt::AlignBottom);
        chart->addAxis(cfg.y_axis.convert(), Qt::AlignLeft);


        // Customize the appearance of the chart
        chart->setTitle(cfg.title);
        chart->legend()->hide();

        // Create some input fields
        auto labels_input = cfg.labels_input();


        auto addPointButton = new QPushButton("Refresh");

        // Connect the button to a slot that adds a new data point to the graph
        connect(addPointButton, &QPushButton::clicked, [=]() {
            auto prob_len = labels_input[0].second->text().toDouble();
            auto m = labels_input[1].second->text().toDouble();
            auto exp_count = labels_input[2].second->text().toDouble();
            auto p_vals = labels_input[3].second->text().toDouble();
            auto deviation = labels_input[4].second->text().toDouble();
            auto model = ChenModel(Vector<double>::generate_normal(prob_len), m);
            auto b = model
                             .calc_p_values(exp_count, p_vals, model.deviate(deviation))
                             .count_buckets(1000)
                             .convert<double>()
                     / p_vals;

            series->clear();

            auto delta = (cfg.x_axis.max - cfg.x_axis.min) / 1000;
            series->append({0, 0});
            for (size_t i = 0; i < 1000; ++i)
                series->append({(i + 1) * delta, b[i]});

            chartView->show();
        });

        // Create a layout to arrange the input fields and button
        auto *inputLayout = new QHBoxLayout();
        for (auto &[label, input]: labels_input) {
            inputLayout->addWidget(label);
            inputLayout->addWidget(input);
        }
        inputLayout->addWidget(addPointButton);

        // Create a layout to arrange the chart view and input layout
        auto *mainLayout = new QVBoxLayout();
        mainLayout->addWidget(chartView);
        mainLayout->addLayout(inputLayout);

        // Set the main layout as the layout for this widget
        setLayout(mainLayout);
    }
};

#endif //STATMOD_GRAPH_TABLE_H
