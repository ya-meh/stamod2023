#include "../internal/vector.h"
#include "../model/model_table.h"


#include <QApplication>
#include <QWidget>
#include <QPushButton>

#include <iostream>

int main(int argc, char *argv[]) {
    auto model = Model15(Vector<int>::generate(15, 100));
    auto exp = model.experiment(150, 15);
    std::cout << exp << std::endl;
    auto uni = exp.unionize();
    std::cout << uni << std::endl;
    std::cout << uni.chi_square() << '\n' << uni.p_value();

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