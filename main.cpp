#include "mainwindow.h"
#include <QApplication>
#include "base.h"
#include "external.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    init_network();
    start_httpd();
    start_tftpd();

    MainWindow w;
    w.show();
    a.exec();

    exit_network();
    return 0;
}


