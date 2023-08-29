#ifndef STATMOD_APPLICATION_H
#define STATMOD_APPLICATION_H


#include <QApplication>

#include <QWidget>
#include <QPushButton>

#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QApplication>

#include "graph_table.h"

class Application : public QApplication {
public:
    Application(int argc, char *argv[]) : QApplication(argc, argv) {
        
    }
};


#endif //STATMOD_APPLICATION_H
