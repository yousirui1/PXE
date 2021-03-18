#ifndef GLOBAL_H
#define GLOBAL_H

#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QLabel>
#include "mainwindow.h"

class Global : public QObject
{
Q_OBJECT
public:
     static Global *getGlobal();
     void setMainWindow(MainWindow *);

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
    MainWindow *main_ui;
};




#endif // GLOBAL_H
