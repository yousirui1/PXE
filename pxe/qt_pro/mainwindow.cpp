#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QToolButton>
#include "global.h"
#include "base.h"
#include "socket.h"
#include "item.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Global::getGlobal()->setMainWindow(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onUpdateTime()));
    timer->start(1000);
    listWidget = ui->listWidget;
    createMenus();
    initDhcp();
    run_flag = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initDhcp()
{
    /* ip 格式 */
    QRegExpValidator *ipValidator = new QRegExpValidator(QRegExp("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$"), this);
    ui->server_edit->setValidator(new QRegExpValidator(ipValidator));
    ui->ip_pool_edit->setValidator(new QRegExpValidator(ipValidator));

    /* 数字格式 */
    QRegExpValidator *numValidator = new QRegExpValidator(QRegExp("^[\-|1-9][0-9]{1,3}"), this);
    ui->lease_edit->setValidator(numValidator);
    ui->pool_size_edit->setValidator(numValidator);

    QList<QNetworkInterface> Qinterfaces = QNetworkInterface::allInterfaces();
    for(QNetworkInterface Qinterface : Qinterfaces )
    {
        QNetworkInterface::InterfaceFlags flags = Qinterface.flags();
        bool use_this = ( flags.testFlag(QNetworkInterface::IsUp) && flags.testFlag(QNetworkInterface::CanBroadcast) && !flags.testFlag(QNetworkInterface::IsLoopBack) );
        if( use_this && !Qinterface.addressEntries().isEmpty() )
        {
            QString name = QString( "%1 (%2)" ).arg(Qinterface.humanReadableName()).arg(Qinterface.addressEntries().back().ip().toString());
            ui->cb_interface->addItem( name, Qinterface.index());
        }
    }
    on_cb_interface_currentIndexChanged(0);
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    openAct = new QAction(tr("&Open Config"), this);
    fileMenu->addAction(openAct);
    connect(openAct, SIGNAL(triggered()),
        this, SLOT(openFile()));

    saveAct = new QAction(tr("&Save Config As"), this);
    fileMenu->addAction(saveAct);
    connect(saveAct, SIGNAL(triggered()),
        this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()),
        this, SLOT(close()));

    toolMenu = menuBar()->addMenu(tr("&Tools"));

    startAct = new QAction(tr("&Start"), this);
    toolMenu->addAction(startAct);
    //connect(startAct, SIGNAL(triggered()),
       // addressWidget, SLOT(addEntry()));

    stopAct = new QAction(tr("&Stop"), this);
    toolMenu->addAction(stopAct);
    //connect(stopAct, SIGNAL(triggered()),
       // addressWidget, SLOT(addEntry()));

    restartAct = new QAction(tr("&ReStart"), this);
    toolMenu->addAction(restartAct);
    //connect(restartAct, SIGNAL(triggered()),
       // addressWidget, SLOT(addEntry()));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAct = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAct);
    connect(aboutAct, SIGNAL(triggered()),
        this, SLOT(about()));

    versionAct = new QAction(tr("&Version"), this);
    helpMenu->addAction(versionAct);
    connect(versionAct, SIGNAL(triggered()),
        this, SLOT(version()));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        qWarning("openFile %s", fileName.toStdString().data());
    }
    else
    {

    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
        }
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
             tr("The <b>Application</b> example demonstrates how to "
                "write modern GUI applications using Qt, with a menu bar, "
                "toolbars, and a status bar."));
    //addItem(count ++);

    //Global *global = Global::getGlobal();
    //global->list.append(count++);

    int ret;
}


void MainWindow::version()
{
    QMessageBox::about(this, tr("Version Application"),
             tr("The <b>Application</b> example demonstrates how to "
                "write modern GUI applications using Qt, with a menu bar, "
                "toolbars, and a status bar."));
#if 0
    //clearItem();

    Global *global = Global::getGlobal();
    //global->list.clear();
#endif
}

void MainWindow::addItem(int index, char *ip, char *mac)
{
    Item *item = new Item(ui->listWidget);
    item->index_label->setText(QString("%1").arg(index));
    item->index_label->setText(QString("%1").arg(ip));
    QListWidgetItem *WContainerItem = new QListWidgetItem(ui->listWidget);
    WContainerItem->setSizeHint(QSize(40, 40));
    ui->listWidget->setItemWidget(WContainerItem, item);
}

void MainWindow::delItem(int index)
{
    QListWidgetItem *currentItem = listWidget->item(index);
    delete currentItem;
}

void MainWindow::clearItem()
{
    int i, count;
    QListWidgetItem *item;
    count = listWidget->count();

    for(i = 0; i <= count; i++)
    {
        item = ui->listWidget->takeItem(0);
        delete item;
    }
}

void MainWindow::onUpdateTime()
{
    int i, count;
    Global *global = Global::getGlobal();
    clearItem();
    for(i = 0; i < 100; i++)
    {
        if(global->clients[i])
            addItem(i, global->clients[i]->ip, global->clients[i]->ip);
    }
}

void MainWindow::modifiyConfig()
{
    QString strAll;
    QStringList strList;
    QFile readFile(".//tftp//pxelinux.cfg//default");
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();
    QFile writeFile(".//tftp//pxelinux.cfg//default");
    if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&writeFile);
        strList=strAll.split("\n");
        for(int i=0;i<strList.count();i++)
        {
            if(i==strList.count()-1)
            {
                //最后一行不需要换行
                stream<<strList.at(i);
            }
            else
            {
                stream<<strList.at(i)<<'\n';
            }

            if(strList.at(i).contains("menu default"))
            {
                QString tempStr = strList.at(i+1);
                tempStr.replace(0,tempStr.length(), QString("  linux vmlinuz-5.2.8-lfs-9.0 tftp_ip=%1 server_ip=%2  ro vga=0x317 ").arg(ui->server_edit->text()));
                stream<<tempStr<<'\n';
                i+=1;
            }
        }
    }
    writeFile.close();
}


void MainWindow::on_cb_interface_currentIndexChanged(int index)
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QString name = QString( "%1 (%2)" ).arg(interfaces.at(index).humanReadableName()).arg(interfaces.at(index).addressEntries().back().ip().toString());
    //qWarning("ui->cb_interface->itemData(index).toInt() %s", name.toStdString().data());
    //char *ip_str = strdup(interfaces.at(index).addressEntries().back().ip().toString().toStdString().data());
    unsigned  int ip = inet_addr(interfaces.at(index).addressEntries().back().ip().toString().toStdString().data());
    unsigned  int i_ip[4];
    /* 提取IP：求余取整 */
    i_ip[0]  = ip % 256;       // 提取第一部分IP地址
    ip  = ip / 256;
    i_ip[1] = ip % 256;        // 提取第一部分IP地址
    ip  = ip / 256;
    i_ip[2] = ip % 256;        // 提取第一部分IP地址
    ip = ip / 256;
    i_ip[3]  = ip % 256;       // 提取第一部分IP地址

    //ui->tftp_edit->setText(interfaces.at(index).addressEntries().back().ip().toString());
    char ip_str[32] = {0};
    sprintf(ip_str, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], 1);
    ui->ip_pool_edit->setText(ip_str);
    ui->router_edit->setText(ip_str);
    //dhcp_config.addr = inet_ntoa(ip);

}

bool isIPvalidate(const QString &strIP)
{
    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!rx2.exactMatch(strIP))
        return false;
    else
        return true;
}

void MainWindow::on_run_btn_clicked()
{
    int ret;

    if(!isIPvalidate(ui->server_edit->text()) || !isIPvalidate(ui->ip_pool_edit->text())
                || !isIPvalidate(ui->mask_edit->text()) || !isIPvalidate(ui->router_edit->text()) || !isIPvalidate(ui->dns_edit->text())
                || !ui->lease_edit->text().length() || !ui->pool_size_edit->text().length() || !ui->pool_size_edit->text().toInt())
    {
        QMessageBox::about(this, tr("PXE Application"),
                 tr("The <b>Application</b> Param is NULL "));
        return;
    }


    QByteArray ba_pool_ip = ui->ip_pool_edit->text().toLatin1();
    QByteArray ba_mask = ui->mask_edit->text().toLatin1();
    QByteArray ba_nds1 = ui->dns_edit->text().toLatin1();
    //QByteArray ba_tftp_ip = ui->tftp_edit->text().toLatin1();
    QByteArray ba_gateway = ui->mask_edit->text().toLatin1();
    //strcpy(dhcp_config.local_ip , interfaces.at(index).addressEntries().back().ip().toString().toStdString().data());
    //DEBUG("dhcp_config.addr %s ", dhcp_config.local_ip);
#if 0
    memcpy(dhcp_config.pool_ip, ba_pool_ip.data(), ba_pool_ip.length());

    dhcp_config.pool_size = ui->pool_size_edit->text().toInt();
    memcpy(dhcp_config.mask, ba_mask.data(), ba_mask.length());
    memcpy(dhcp_config.dns1, ba_nds1.data(), ba_nds1.length());
    memcpy(dhcp_config.gateway, ba_gateway.data(), ba_gateway.length());
    memcpy(dhcp_config.tftp_ip, ba_tftp_ip.data(), ba_tftp_ip.length());
    dhcp_config.lease = ui->lease_edit->text().toInt();
#endif

    if(run_flag)
    {
         ui->statu_label->setStyleSheet("border-image: url(:/images/offline-point.png);");
         ui->run_btn->setText("Start");
         ui->server_edit->setFocusPolicy(Qt::StrongFocus);
         //ui->tftp_edit->setFocusPolicy(Qt::StrongFocus);
         ui->ip_pool_edit->setFocusPolicy(Qt::StrongFocus);
         ui->mask_edit->setFocusPolicy(Qt::StrongFocus);
         ui->router_edit->setFocusPolicy(Qt::StrongFocus);
         ui->dns_edit->setFocusPolicy(Qt::StrongFocus);
         ui->lease_edit->setFocusPolicy(Qt::StrongFocus);
         ui->pool_size_edit->setFocusPolicy(Qt::StrongFocus);
         ui->cb_interface->setFocusPolicy(Qt::StrongFocus);
    }
    else
    {
         ui->statu_label->setStyleSheet("border-image: url(:/images/onlie-point.png);");
         ui->run_btn->setText("Stop");

         ui->server_edit->setFocusPolicy(Qt::NoFocus);
         //ui->tftp_edit->setFocusPolicy(Qt::NoFocus);
         ui->ip_pool_edit->setFocusPolicy(Qt::NoFocus);
         ui->mask_edit->setFocusPolicy(Qt::NoFocus);
         ui->router_edit->setFocusPolicy(Qt::NoFocus);
         ui->dns_edit->setFocusPolicy(Qt::NoFocus);
         ui->lease_edit->setFocusPolicy(Qt::NoFocus);
         ui->pool_size_edit->setFocusPolicy(Qt::NoFocus);
         ui->cb_interface->setFocusPolicy(Qt::NoFocus);
    }
    run_flag = !run_flag;
}

