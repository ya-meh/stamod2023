#include "model_logic.h"

#include "../gui/window.h"

#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto chen_model = new ChenModelRunner;
    defer([&]() { delete chen_model; });

    auto table_model = new TableModelRunner;
    defer([&]() { delete table_model; });

    auto window = Window({chen_model, table_model});

    return a.exec();
}
