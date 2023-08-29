#include "../internal/vector.h"
#include "../model/model_table.h"
#include "../model/model_chen.h"
#include "../internal/defer.h"
#include "../gui/graph.h"
#include "model_logic.h"


#include <QApplication>
#include <QWidget>
#include <QPushButton>

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QtCharts>

#include <thread>
#include <chrono>

QT_CHARTS_USE_NAMESPACE


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMainWindow window;
    auto chartView = new TableGraphWidget(new TableModelRunner,
                                          &window
    );

    window.setCentralWidget(chartView);

    window.resize(800, 600);
    window.show();

    return a.exec();
}
