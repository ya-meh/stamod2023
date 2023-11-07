#ifndef STATMOD_WINDOW_H
#define STATMOD_WINDOW_H


#include "graph.h"

#include "../internal/vector.h"

#include <QMainWindow>


class Window : public QMainWindow {
    Vector<ModelRunner *> models_;
    TableGraphWidget *graph = nullptr;
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

        setMinimumSize(width, height);
        resize(width, height);

        show();
    };

    void chart(ModelRunner *model) {
        delete graph;
        setCentralWidget(graph = new TableGraphWidget(model, this));
    }
};


#endif //STATMOD_WINDOW_H
