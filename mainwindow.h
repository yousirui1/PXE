#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QNetworkInterface>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

extern "C"
{
    #include "dll.h"
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    void initDhcp();
    void modifiyVOIConfig();
    void modifiyVDIConfig(); 
    void modifiyVDIConfigEFI();
    void modifiyVOIConfigEFI();
    void killApp(const QString& appName);
    void runApp(const QString& appName);

private slots:
    void on_cb_interface_currentIndexChanged(int index);
    void on_run_btn_clicked();

    void on_voi_radio_clicked();

    void on_vdi_radio_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timer;

    const QRegExp re_ip_address{ "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$" };
    int run_flag;

    unsigned  int u_ip[4];
    char local_ip[36];
    int os;
    struct configs conf;
    int major, minor;
};

#endif // MAINWINDOW_H
