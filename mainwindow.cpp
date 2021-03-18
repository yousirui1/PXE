#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProcess>
#include <QToolButton>
#include "global.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Global * global = Global::getGlobal();
    global->setMainWindow(this);
    run_flag = 0;
    os = 0;
    memset(&conf, 0, sizeof(struct configs));
    ui->voi_radio->setChecked(true);
    get_ver(&major, &minor);
    ui->version_label->setText(QString("v%1.%2  ").arg(major).arg(minor));
    ui->version_label->setAlignment(Qt::AlignRight);
    initDhcp();
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

void MainWindow::modifiyVOIConfig()
{
    QString strAll;
    QStringList strList;
    QFile readFile(".//pxe//boot.ipxe");
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();
    QFile writeFile(".//pxe//boot.ipxe");
    if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&writeFile);
        strList=strAll.split("\n");
        for(int i=0;i<strList.count();i++)
        {
            if(i == strList.count()-1)
            {
                //最后一行不需要换行
                stream<<strList.at(i);
            }
            else
            {
                stream<<strList.at(i)<<'\n';
            }

            if(strList.at(i).contains("dhcp"))
            {
                QString tempStr = strList.at(i+1);
                //kernel http://192.169.27.196/boot/vmlinuz-5.2.8-lfs-9.0 tftp_ip=192.168.1.1 server_ip=192.169.27.181 ro vga=0x317
                tempStr.replace(0,tempStr.length(), QString("   kernel http://%1/pxe/linux/vmlinuz-5.2.8-lfs-9.0 server_ip=%2 http_ip=%3 os=voi ro vga=0x317 ")
                                .arg(QString(local_ip)).arg(ui->server_edit->text()).arg(QString(local_ip)));
                stream<<tempStr<<'\n';
                i+=1;
            }
        }
    }
    writeFile.close();
}

void MainWindow::modifiyVDIConfig()
{
    QString strAll;
    QStringList strList;
    QFile readFile(".//pxe//boot.ipxe");
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();
    QFile writeFile(".//pxe//boot.ipxe");
    if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&writeFile);
        strList=strAll.split("\n");
        for(int i=0;i<strList.count();i++)
        {
            if(i == strList.count()-1)
            {
                //最后一行不需要换行
                stream<<strList.at(i);
            }
            else
            {
                stream<<strList.at(i)<<'\n';
            }

            if(strList.at(i).contains("dhcp"))
            {
                QString tempStr = strList.at(i+1);
                //kernel http://192.169.27.196/boot/vmlinuz-5.2.8-lfs-9.0 tftp_ip=192.168.1.1 server_ip=192.169.27.181 ro vga=0x317
                tempStr.replace(0,tempStr.length(), QString("   kernel http://%1/pxe/linux/vmlinuz-5.2.8-lfs-9.0 server_ip=%2  http_ip=%3 os=vdi ro vga=0x317 ")
                                .arg(QString(local_ip)).arg(ui->server_edit->text()).arg(QString(local_ip)));
                stream<<tempStr<<'\n';
                i+=1;
            }
        }
    }
    writeFile.close();
}

void MainWindow::modifiyVOIConfigEFI()
{
    QString strAll;
    QStringList strList;
    QFile readFile(".//pxe//grub//x64.cfg");
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();
    QFile writeFile(".//pxe//grub//x64.cfg");
    if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&writeFile);
        strList=strAll.split("\n");
        for(int i=0;i<strList.count();i++)
        {
            if(i == strList.count()-1)
            {
                //最后一行不需要换行
                stream<<strList.at(i);
            }
            else
            {
                stream<<strList.at(i)<<'\n';
            }

            if(strList.at(i).contains("set root"))
            {
                QString tempStr = strList.at(i+1);
                //kernel http://192.169.27.196/boot/vmlinuz-5.2.8-lfs-9.0 tftp_ip=192.168.1.1 server_ip=192.169.27.181 ro vga=0x317
                tempStr.replace(0,tempStr.length(), QString("   linux (http)/pxe/linux/vmlinuz-5.2.8-lfs-9.0 server_ip=%1 http_ip=%2 os=voi ro vga=0x317")
                                .arg(ui->server_edit->text()).arg(QString(local_ip)));
                stream<<tempStr<<'\n';
                i+=1;
            }
        }
    }
    writeFile.close();
}

void MainWindow::modifiyVDIConfigEFI()
{
    QString strAll;
    QStringList strList;
    QFile readFile(".//pxe//grub//x64.cfg");
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();
    QFile writeFile(".//pxe//grub//x64.cfg");
    if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&writeFile);
        strList=strAll.split("\n");
        for(int i=0;i<strList.count();i++)
        {
            if(i == strList.count()-1)
            {
                //最后一行不需要换行
                stream<<strList.at(i);
            }
            else
            {
                stream<<strList.at(i)<<'\n';
            }

            if(strList.at(i).contains("set root"))
            {
                QString tempStr = strList.at(i+1);
                //kernel http://192.169.27.196/boot/vmlinuz-5.2.8-lfs-9.0 tftp_ip=192.168.1.1 server_ip=192.169.27.181 ro vga=0x317
                tempStr.replace(0,tempStr.length(), QString("   linux (http)/pxe/linux/vmlinuz-5.2.8-lfs-9.0 server_ip=%1 http_ip=%2 os=vdi ro vga=0x317")
                                .arg(ui->server_edit->text()).arg(QString(local_ip)));
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
    ui->server_edit->setText(ip_str);

    strcpy(local_ip ,interfaces.at(index).addressEntries().back().ip().toString().toStdString().data());
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
        QMessageBox::about(this, tr("PXE"),
                 tr("参数错误,请重新输入 "));
        return;
    }

    QByteArray ba_pool_ip = ui->ip_pool_edit->text().toLatin1();
    QByteArray ba_mask = ui->mask_edit->text().toLatin1();
    QByteArray ba_nds1 = ui->dns_edit->text().toLatin1();
    QByteArray ba_gateway = ui->router_edit->text().toLatin1();

    unsigned int ip = inet_addr(ba_pool_ip.data());
    u_ip[0]  = ip % 256;       // 提取第一部分IP地址
    ip  = ip / 256;
    u_ip[1] = ip % 256;        // 提取第一部分IP地址
    ip  = ip / 256;
    u_ip[2] = ip % 256;        // 提取第一部分IP地址
    ip = ip / 256;
    u_ip[3]  = ip % 256;       // 提取第一部分IP地址

    strcpy((char *)conf.netcard.ip, local_ip);
    strcpy(conf.dhcp.local_ip, local_ip);
    memcpy(conf.dhcp.pool_ip, ba_pool_ip.data(), ba_pool_ip.length());
    conf.dhcp.pool_size = ui->pool_size_edit->text().toInt();
    memcpy(conf.dhcp.mask, ba_mask.data(), ba_mask.length());
    memcpy(conf.dhcp.dns1, ba_nds1.data(), ba_nds1.length());
    memcpy(conf.dhcp.gateway, ba_gateway.data(), ba_gateway.length());
    //memcpy(conf.server.ip, ba_gateway.data(), ba_gateway.length());
    //memcpy(conf.dhcp.tftp_ip, ba_tftp_ip.data(), ba_tftp_ip.length());
    strcpy(conf.dhcp.tftp_ip, conf.dhcp.local_ip);
    conf.dhcp.lease = ui->lease_edit->text().toInt();
    conf.http.port= 80;
    conf.dhcp.again = 20;

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
         ui->voi_radio->setFocusPolicy(Qt::StrongFocus);
         ui->vdi_radio->setFocusPolicy(Qt::StrongFocus);
         qApp->exit();
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
         ui->voi_radio->setFocusPolicy(Qt::NoFocus);
         ui->vdi_radio->setFocusPolicy(Qt::NoFocus);
         if(os == 2)        //VDI
         {
            conf.type = 2;
            modifiyVDIConfig();
            modifiyVDIConfigEFI();
         }
         else               //VOI
         {
            conf.type = 1;
            modifiyVOIConfig();
            modifiyVOIConfigEFI();
         }
         start_pxe(&conf);
    }
    run_flag = !run_flag;

}


void MainWindow::on_voi_radio_clicked()
{
    os = 1;
}

void MainWindow::on_vdi_radio_clicked()
{
    os = 2;
}
