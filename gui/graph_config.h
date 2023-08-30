#ifndef STATMOD_GRAPH_CONFIG_H
#define STATMOD_GRAPH_CONFIG_H


#include "../internal/pair.h"
#include "../internal/vector.h"

#include <QtWidgets>
#include <QValueAxis>

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
            std::string str;
            if (type == DOUBLE)
                str = std::to_string(value);
            else
                str = std::to_string(static_cast<int>(value));
            return new QLineEdit(QString::fromStdString(str));
        }
    };


    QString title;
    Axis x_axis = {"X"};
    Axis y_axis = {"Y"};
    Vector<Field> fields = {};
    size_t points_n = 1000;

    [[nodiscard]] Vector<Pair<QLabel *, QLineEdit *>> labels_input() const {
        auto labels = Vector<Pair<QLabel *, QLineEdit *>>(fields.size());

        for (size_t i = 0; i < labels.size(); ++i) {
            labels[i] = {fields[i].label(), fields[i].edit_field()};
        }

        return labels;
    }
};


#endif //STATMOD_GRAPH_CONFIG_H
