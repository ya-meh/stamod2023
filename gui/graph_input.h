#ifndef STATMOD_GRAPH_INPUT_H
#define STATMOD_GRAPH_INPUT_H


#include "graph_config.h"

#include "../internal/pair.h"
#include "../internal/vector.h"

#include <numeric>
#include <string>
#include <exception>

#include <QLabel>
#include <QLineEdit>


class ModelRunner {
protected:
    static const int INT = 0, DOUBLE = 1;

    struct Spec {
        int type;
        long double min = std::numeric_limits<long double>::min();
        long double max = std::numeric_limits<long double>::max();

        [[nodiscard]] bool is_valid(const QString &text) const {
            auto std_text = text.toStdString();
            switch (type) {
                case INT: {
                    volatile const auto val = std::stoi(std_text);
                    if (val < min || val > max) {
                        return false;
                    }
                    break;
                }
                case DOUBLE: {
                    volatile const auto val = std::stof(std_text);
                    if (val < min || val > max) {
                        return false;
                    }
                    break;
                }
                default: {
                    throw std::runtime_error("Invalid input ModelRunner::Spec");
                }
            }

            return true;
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
            try {
                if (!input_types_spec()[i].is_valid(data[i].second->text())) {
                    return {"Value \"" + data[i].first->text() + "\" is out of bounds (" + data[i].second->text() + ")",
                            false};
                }
            } catch (const std::invalid_argument &e) {
                return {"while parsing argument '"
                        + data[i].first->text() + "' an invalid_argument occurred: " + e.what(), false};
            } catch (const std::out_of_range &e) {
                return {"while parsing argument '"
                        + data[i].first->text() + "' an out_of_range occurred: " + e.what(), false};
            }
        }

        return {"", true};
    }

    virtual ~ModelRunner() = default;
};

#endif //STATMOD_GRAPH_INPUT_H
