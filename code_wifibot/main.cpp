#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    //Settings
    //http://192.168.1.106:8080/control.htm
    //http://192.168.1.106:8080/input_0.json
   // http://192.168.1.106:8080/?action=snapshot
}
