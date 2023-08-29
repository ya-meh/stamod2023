#ifndef STATMOD_INPUT_H
#define STATMOD_INPUT_H


#include "config.h"

#include "../internal/pair.h"
#include "../internal/vector.h"

#include <QLabel>
#include <QLineEdit>


class ModelRunner {
public:
    [[nodiscard]] virtual Vector<double> from(const Vector<Pair<QLabel *, QLineEdit *>> &data, size_t count) = 0;

    [[nodiscard]] virtual Config default_config() = 0;

    virtual ~ModelRunner() = default;
};

#endif //STATMOD_INPUT_H
