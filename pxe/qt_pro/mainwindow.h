#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QNetworkInterface>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

    void delItem(int index);
    void addItem(int index, char *ip, char *mac);
    void clearItem();

private:
    void createMenus();
    bool saveFile(const QString &fileName);
    void initDhcp();
    void modifiyConfig();

private slots:
    void openFile();
    bool saveAs();
    void about();
    void version();

    void onUpdateTime();
    void on_cb_interface_currentIndexChanged(int index);
    void on_run_btn_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timer;

    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QMenu *toolMenu;
    QAction *clearAct;
    QAction *stopAct;
    QAction *startAct;
    QAction *restartAct;

    QMenu *helpMenu;
    QAction *aboutAct;
    QAction *versionAct;

    QListWidget *listWidget;
    const QRegExp re_ip_address{ "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$" };
    int run_flag;


};

#endif // MAINWINDOW_H
