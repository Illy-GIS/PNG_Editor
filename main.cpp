#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <png.h>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow w;
    w.setWindowTitle("App");
    w.show();
    return application.exec();
}
