#ifndef STATMOD_REGRAPH_H
#define STATMOD_REGRAPH_H


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
    };


    QString title;
    Axis x_axis = {"X"};
    Axis y_axis = {"Y"};
    // name + default value
    Vector<Field> fields = {};

    [[nodiscard]] Vector<Pair<QLabel *, QLineEdit *>> labels_input() const {
        auto labels = Vector<Pair<QLabel *, QLineEdit *>>(fields.size());

        for (size_t i = 0; i < labels.size(); ++i)
            labels[i] = {new QLabel(fields[i].name),
                         new QLineEdit(QString::fromStdString(std::to_string(fields[i].value)))};

        return labels;
    }
};

class ReTableGraphWidget : public QWidget {
    Config cfg;
public:
    explicit ReTableGraphWidget(Config config, QWidget *parent) : cfg(std::move(config)), QWidget(parent) {

    }
};


#endif //STATMOD_REGRAPH_H
