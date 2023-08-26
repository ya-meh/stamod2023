#include <QApplication>
#include <QWidget>
#include <QPushButton>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.resize(250, 150);
    window.setWindowTitle("QT5 Widget Application");

    QPushButton button("Hello World!", &window);
    button.setGeometry(10, 10, 200, 30);

    window.show();

    return app.exec();
}