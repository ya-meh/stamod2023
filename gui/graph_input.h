#ifndef STATMOD_GRAPH_INPUT_H
#define STATMOD_GRAPH_INPUT_H


#include "graph_config.h"

#include "../internal/pair.h"
#include "../internal/vector.h"

#include <QLabel>
#include <QLineEdit>


class ModelRunner {
protected:
    static const int INT = 0, DOUBLE = 1;

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

    [[nodiscard]] virtual const Vector<int> &input_types_spec() const = 0;

    [[nodiscard]] Pair<QString, bool> is_valid(const Vector<Pair<QLabel *, QLineEdit *>> &data) const {
        if (data.size() != input_types_spec().size())
            return {"wrong data length", false};

        for (size_t i = 0; i < data.size(); ++i) {
            try {
                auto text = data[i].second->text().toStdString();

                switch (input_types_spec()[i]) {
                    case INT: {
                        volatile const auto nvm = std::stoi(text);
                        break;
                    }
                    case DOUBLE: {
                        volatile const auto nvm = std::stof(text);
                        break;
                    }
                    default: {
                        return {"unexpected input in " + data[i].first->text(), false};
                    }
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
