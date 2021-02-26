#ifndef GLOBAL_H
#define GLOBAL_H

#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QLabel>
#include "mainwindow.h"
#include "base.h"
#include "pipethread.h"
#include "client.h"

class Global : public QObject
{
Q_OBJECT
public:
     static Global *getGlobal();

     void setMainWindow(MainWindow *);
     void setPipe(int fd[2]);

    void setClients(struct client **clients);


private:
     Global();
     ~Global();
signals:


public slots:


private slots:
    //void onUpdateTime();

private:
    static Global *mGlobal;

public:
    QTimer *timer;
    QLabel *data_label;
    MainWindow *main_ui;
    struct client **clients;
    PipeThread *pipe;
};




#endif // GLOBAL_H
