#ifndef STATMOD_MAIN_WINDOW_H
#define STATMOD_MAIN_WINDOW_H


#include "p_val_graph_widget.h"

#include "../internal/vector.h"

#include <QMainWindow>


class Window : public QMainWindow {
    Vector<ModelRunner *> models_;
    QWidget *graph_ = nullptr;
public:
    explicit Window(Vector<ModelRunner *> models, int width = 800, int height = 600) : models_(std::move(models)) {
        auto bar = new QToolBar("Models", this);

//        auto help_action = new QAction("HELP", this);
//        connect(help_action, &QAction::triggered, [=]() {
//
//        });
//        bar->addAction(help_action);

        for (auto model: models_) {
            auto action = new QAction(model->default_config().title, this);
            connect(action, &QAction::triggered, [=]() {
                set_model(model);
            });
            bar->addAction(action);
        }

        addToolBar(bar);

        bar->actions().first()->trigger();

        setMinimumSize(width, height);
        resize(width, height);

        show();
    };

    void set_model(ModelRunner *model) {
        delete graph_;
        setCentralWidget(graph_ = model->new_graph_widget(this));
    }

    ~Window() override { delete graph_; }
};


#endif //STATMOD_MAIN_WINDOW_H
