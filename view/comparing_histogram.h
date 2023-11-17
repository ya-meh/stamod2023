#ifndef STATMOD_COMPARING_HISTOGRAM_H
#define STATMOD_COMPARING_HISTOGRAM_H

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
#include <QStackedBarSeries>

class HistogramChart {
public:
    struct HistWithAxis {
        QChart *chartView;
        QAbstractAxis *x_axis, *y_axis;
    };

    template<typename T, typename U>
    static QChart *build_generic(const Vector<T> &lhs_data, const QString &lhs_name,
                                 const Vector<U> &rhs_data, const QString &rhs_name,
                                 const Vector<std::string> &legend, const QString &title = "") {
        return build(lhs_data.convert(), lhs_name,
                     rhs_data.convert(), rhs_name,
                     legend, title
        );
    }

    static HistWithAxis build_verbose(const Vector<double> &lhs_data, const QString &lhs_name,
                                      const Vector<double> &rhs_data, const QString &rhs_name,
                                      const Vector<std::string> &legend, const QString &title = "") {
        auto equal = new QBarSet("Equal");
        auto lesser = new QBarSet(lhs_name);
        auto greater = new QBarSet(rhs_name);

        for (size_t i = 0; i < lhs_data.size(); i++) {
            if (lhs_data[i] < rhs_data[i]) {
                *equal << lhs_data[i];
                *greater << (rhs_data[i] - lhs_data[i]);
                *lesser << 0;
            } else {
                *equal << rhs_data[i];
                *greater << 0;
                *lesser << (lhs_data[i] - rhs_data[i]);
            }
        }

        auto *series = new QStackedBarSeries;
        series->append({equal, lesser, greater});

        auto chart = new QChart;
        chart->addSeries(series);
        if (title != "") {
            chart->setTitle(title);
        }

        QStringList categories;
        for (auto &lgnd: legend) {
            categories.push_back(QString::fromStdString(lgnd));
        }

        auto axisX = new QBarCategoryAxis;
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        auto axisY = new QValueAxis;
        axisY->setRange(0, std::max(lhs_data.max(), rhs_data.max()));
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        return {chart, axisX, axisY};
    }

    static QChart *build(const Vector<double> &lhs_data, const QString &lhs_name,
                         const Vector<double> &rhs_data, const QString &rhs_name,
                         const Vector<std::string> &legend, const QString &title = "") {
        return build_verbose(lhs_data, lhs_name, rhs_data, rhs_name, legend, title).chartView;
    }
};

#endif //STATMOD_COMPARING_HISTOGRAM_H
