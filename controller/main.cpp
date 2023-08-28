#include "../internal/vector.h"
#include "../model/model_table.h"
#include "../model/model_chen.h"
#include "../internal/defer.h"


#include <QApplication>
#include <QWidget>
#include <QPushButton>

#include <iostream>

int main(int argc, char *argv[]) {
//    auto model = TableModel(Vector<int>::generate(15, 100));
//    auto exp = model.experiment(150);
//    std::cout << exp << std::endl;
//    auto uni = exp.unionize();
//    std::cout << uni << std::endl;
//    std::cout << "Chi square\tp value\n" << uni.chi_square() << "\t\t" << uni.p_value() << "\n\n\n";

//    auto h0 = TableModel(Vector<int>{44, 39, 33, 60, 63, 79, 27, 3, 97, 83, 1, 66, 56, 99, 78});
//    std::cout << h0.experiment(150).unionize().chi_square() << "\t\t"
//              << h0.experiment(150).unionize().p_value() << "\n\n\n";

//    auto h1 = TableModel(Vector<int>{56, 43, 34, 60, 74, 89, 46, 8, 100, 83, 13, 78, 61, 103, 88});
    auto h0 = ChenModel(Vector<double>::generate_normal(40), 10);
    auto h1 = h0.deviate(0.025);
//    defer([h1]() { delete h1; });
    auto tmp = h0.calc_p_values(150, 1000, h1);
//    std::cout <<
//    auto alt = tmp.count_buckets(20);
    std::cout << tmp.count_buckets(20).convert<long double>() / tmp.size();

    delete h1;

    return 0;


    auto vec = Vector<double>::generate_normal(10);
    std::cout << vec.to_string() << '\n';
//    std::cout << vec.deviate(0.1).to_string() << '\n';


    return 0;


    QApplication app(argc, argv);

    QWidget window;
    window.resize(250, 150);
    window.setWindowTitle("QT5 Widget Application");

    QPushButton button("Hello World!", &window);
    button.setGeometry(10, 10, 200, 30);

    window.show();

    return app.exec();
}