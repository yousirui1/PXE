#include "mainwindow.h"
#include <QApplication>
#include "base.h"
#include "socket.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();
    return 0;
}


