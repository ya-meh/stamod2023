#include "model_logic.h"

#include "../gui/window.h"

#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto chen_model = ChenModelRunner{};
    auto table_model = TableModelRunner{};

    auto window = Window({&chen_model, &table_model});

    return a.exec();
}
