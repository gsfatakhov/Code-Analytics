#include "mainwindow.h"

#include <QApplication>
#include <QApplication>
#include <iostream>
#include <clickhouse/client.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
