#include "../controller/p_val_model_runner.h"
#include "../controller/modeling_model_runner.h"
#include "../view/main_window.h"
#include "../view/comparing_histogram.h"

#include <QApplication>
#include <QStackedBarSeries>
#include <QChart>
#include <iostream>
#include <map>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto chen_model = ChenPValuesModelRunner{};
    auto table_model = TablePValuesModelRunner{};
    auto chen_modeling = ChenModelingModelRunner{};
    auto table_modeling = TableModelingModelRunner{};

    auto window = Window({&table_modeling, &table_model, &chen_modeling, &chen_model,});

    return a.exec();
}
