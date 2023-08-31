#ifndef STATMOD_WINDOW_H
#define STATMOD_WINDOW_H

#include <QMainWindow>
#include "../internal/vector.h"
#include "graph.h"

class Window : public QMainWindow {
    Vector<ModelRunner *> models_;
public:
    explicit Window(Vector<ModelRunner *> models, int width = 800, int height = 600) : models_(std::move(models)) {
        auto bar = new QToolBar("Models", this);

        for (auto model: models_) {
            auto action = new QAction(model->default_config().title, this);
            connect(action, &QAction::triggered, [=]() {
                chart(model);
            });
            bar->addAction(action);
        }

        addToolBar(bar);

        bar->actions().first()->trigger();

        resize(width, height);

        show();
    };

    void chart(ModelRunner *model) {
        setCentralWidget(new TableGraphWidget(model, this));
    }

    void chart(const QString &name) {
        for (auto model: models_) {
            if (model->default_config().title == name) {
                chart(model);
            }
        }
    }
};


#endif //STATMOD_WINDOW_H
