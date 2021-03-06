#if 0
#include <QApplication>
#include "mainwindow.h"
#include <QWSServer>
#include <QTextCodec>
#include <QMutex>
#include <QFile>
#include "dll.h"
#include "global.h"

#define _TIME_ qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))

void msg_out(QtMsgType type, const char *msg)
{
    QString qstrText;
    static QMutex mutex;
    switch(type)
    {
        case QtDebugMsg:
            qstrText = QString("[%1]  [Debug] ").arg(_TIME_);
            break;
        case QtWarningMsg:
            qstrText = QString("[%1]  [Warning] ").arg(_TIME_);
            break;
        default:
            return;

    }
    QFile out("./logscreen-" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".log");
    mutex.lock();
    if(out.open(QIODevice::WriteOnly | QIODevice:: Text | QIODevice::Append))
    {
        if(out.size() > 1024 * 1024 * 64)
        {
            out.close();
            out.open(QIODevice::WriteOnly | QIODevice::Text);
        }
        QTextStream ts(&out);
        ts<<qstrText<<"  "<<QString::fromUtf8(msg)<<endl;
        out.close();
    }
    mutex.unlock();
}

int create_window()
{
    int argc = 1;
    char *argv[1] = {"PXE"};

    QApplication a(argc, argv);
#ifndef _WIN32
    QWSServer::setBackground(QColor(0,0,0,0));
#endif
    qInstallMsgHandler(msg_out);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();
    return a.exec();
}

int set_startcallback()
{
    Global *global = Global::getGlobal();
    //global->setstar();
}

int set_stopcallback(struct config *conf)
{
    Global *global = Global::getGlobal();
    //global->setstar();
}
#endif
