#include "model_logic.h"

#include "../gui/graph.h"

#include <QApplication>
#include <QMainWindow>

QT_CHARTS_USE_NAMESPACE


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMainWindow window;
    auto chartView = new TableGraphWidget(new TableModelRunner,
                                          &window,
                                          new ChenModelRunner
    );

    window.setCentralWidget(chartView);

    window.resize(800, 600);
    window.show();

    return a.exec();
}
