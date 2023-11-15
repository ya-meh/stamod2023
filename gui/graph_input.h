#ifndef STATMOD_GRAPH_INPUT_H
#define STATMOD_GRAPH_INPUT_H


#include "graph_config.h"

#include "../internal/pair.h"
#include "../internal/vector.h"

#include <numeric>
#include <string>
#include <exception>
#include <iostream>

#include <QLabel>
#include <QLineEdit>


class ModelRunner {
protected:
    static const int INT = 0, DOUBLE = 1;

    struct Spec {
        int type;
        long double min = std::numeric_limits<long double>::min();
        long double max = std::numeric_limits<long double>::max();

        [[nodiscard]] long double force_bounds(long double val) const {
            if (val < min) {
                return min;
            }
            if (val > max) {
                return max;
            }
            return val;
        }

        [[nodiscard]] QString to_q_string(long double val) const {
            switch (type) {
                case INT: {
                    return QString::fromStdString(std::to_string((long long) val));
                }
                case DOUBLE: {
                    return QString::fromStdString(std::to_string(val));
                }
                default: {
                    throw std::runtime_error("Invalid input ModelRunner::Spec::is_valid");
                }
            }
        }

        [[nodiscard]] bool in_bounds(long double val) const {
            return !((val < min) || (val > max));
        }

        [[nodiscard]] Pair<long double, bool> is_valid(const QString &text) const {
            auto std_text = text.toStdString();

            try {
                switch (type) {
                    case INT: {
                        volatile const auto val = std::stoi(std_text);
                        if (!in_bounds(val)) {
                            return {(long double) val, false};
                        }
                        return {(long double) val, true};
                    }
                    case DOUBLE: {
                        volatile const auto val = std::stof(std_text);
                        if (!in_bounds(val)) {
                            return {val, false};
                        }
                        return {val, true};
                    }
                    default: {
                        throw std::runtime_error("Invalid input ModelRunner::Spec::is_valid");
                    }
                }
            } catch (std::exception &e) {
                return {min, false};
            }
        }
    };

    static int to_int(const Pair<QLabel *, QLineEdit *> &input) {
        auto ret = input.second->text().toInt();
        if (ret == 0) { // qt parses 200.3 to 0, not to 200
            return static_cast<int>(to_double(input));
        }
        return ret;
    }

    static double to_double(const Pair<QLabel *, QLineEdit *> &input) {
        return input.second->text().toDouble();
    }

public:
    [[nodiscard]] virtual Vector<double> from(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) = 0;

    [[nodiscard]] virtual Config default_config() = 0;

    [[nodiscard]] virtual const Vector<Spec> &input_types_spec() const = 0;

    [[nodiscard]] virtual int error_type(const Vector<Pair<QLabel *, QLineEdit *>> &data) const { return 0; };

    [[nodiscard]] Pair<QString, bool> is_valid(const Vector<Pair<QLabel *, QLineEdit *>> &data) const {
        if (data.size() != input_types_spec().size())
            return {"wrong data length", false};

        for (size_t i = 0; i < data.size(); ++i) {
            auto spec = input_types_spec()[i];
            auto [val, ok] = spec.is_valid(data[i].second->text());
            if (!ok) {
                if (spec.in_bounds(val)) {
                    auto trimmed = data[i].second->text().trimmed();
                    if (trimmed == "" || trimmed == "-") {
                        return {"ignore", false};
                    }
                    return {"parse failure of arg " + data[i].first->text(), false};
                }

                auto log = spec.to_q_string(spec.force_bounds(val));
                std::cout << data[i].second->text().toStdString() << ' ' << val << ' ' << log.toStdString()
                          << std::endl;
                data[i].second->setText(spec.to_q_string(spec.force_bounds(val)));
            }
        }

        return {"", true};
    }

    virtual ~ModelRunner() = default;
};

#endif //STATMOD_GRAPH_INPUT_H
