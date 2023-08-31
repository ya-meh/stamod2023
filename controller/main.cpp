#include "model_logic.h"

#include "../gui/graph.h"
#include "../gui/window.h"

#include <QApplication>
#include <QMainWindow>

QT_CHARTS_USE_NAMESPACE

class Windows {
public:

};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto chen_model = new ChenModelRunner;
    defer([&]() { delete chen_model; });

    auto table_model = new TableModelRunner;
    defer([&]() { delete table_model; });

    auto window = Window({chen_model, table_model});

//    QMainWindow window;
//    auto chartView = new TableGraphWidget(table_model,
//                                          &window
//    );
//
//    window.setCentralWidget(chartView);
//
//    window.resize(800, 600);
//    window.show();

    return a.exec();
}
