/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionClear;
    QAction *actionClose;
    QAction *actionAbout;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionSave;
    QAction *actionVersion;
    QAction *actionOpen;
    QAction *actionSave_As;
    QAction *actionExit;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *layout_left;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label;
    QListWidget *listWidget;
    QWidget *layoutWidget1;
    QVBoxLayout *layout_right;
    QFormLayout *formLayout;
    QLabel *label_interface;
    QComboBox *cb_interface;
    QLabel *label_router;
    QLineEdit *server_edit;
    QGroupBox *groupBox;
    QFormLayout *formLayout_2;
    QLabel *ip_pool_label;
    QLineEdit *ip_pool_edit;
    QLabel *pool_size_label;
    QLineEdit *pool_size_edit;
    QLabel *lease_label;
    QLineEdit *lease_edit;
    QLabel *router_label;
    QLineEdit *router_edit;
    QLabel *mask_label;
    QLineEdit *mask_edit;
    QLabel *dns_label;
    QLineEdit *dns_edit;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *statu_label;
    QPushButton *run_btn;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(792, 481);
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionStart = new QAction(MainWindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionVersion = new QAction(MainWindow);
        actionVersion->setObjectName(QString::fromUtf8("actionVersion"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layout_left = new QVBoxLayout(layoutWidget);
        layout_left->setSpacing(0);
        layout_left->setContentsMargins(11, 11, 11, 11);
        layout_left->setObjectName(QString::fromUtf8("layout_left"));
        layout_left->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(layoutWidget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_8 = new QLabel(widget_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_3->addWidget(label_8);

        label_9 = new QLabel(widget_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_3->addWidget(label_9);

        label_10 = new QLabel(widget_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_3->addWidget(label_10);

        label = new QLabel(widget_3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);


        layout_left->addWidget(widget_3);

        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setAlternatingRowColors(true);
        listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        layout_left->addWidget(listWidget);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layout_right = new QVBoxLayout(layoutWidget1);
        layout_right->setSpacing(6);
        layout_right->setContentsMargins(11, 11, 11, 11);
        layout_right->setObjectName(QString::fromUtf8("layout_right"));
        layout_right->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_interface = new QLabel(layoutWidget1);
        label_interface->setObjectName(QString::fromUtf8("label_interface"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_interface);

        cb_interface = new QComboBox(layoutWidget1);
        cb_interface->setObjectName(QString::fromUtf8("cb_interface"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cb_interface);

        label_router = new QLabel(layoutWidget1);
        label_router->setObjectName(QString::fromUtf8("label_router"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_router);

        server_edit = new QLineEdit(layoutWidget1);
        server_edit->setObjectName(QString::fromUtf8("server_edit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, server_edit);

        groupBox = new QGroupBox(layoutWidget1);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout_2 = new QFormLayout(groupBox);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        ip_pool_label = new QLabel(groupBox);
        ip_pool_label->setObjectName(QString::fromUtf8("ip_pool_label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, ip_pool_label);

        ip_pool_edit = new QLineEdit(groupBox);
        ip_pool_edit->setObjectName(QString::fromUtf8("ip_pool_edit"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, ip_pool_edit);

        pool_size_label = new QLabel(groupBox);
        pool_size_label->setObjectName(QString::fromUtf8("pool_size_label"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, pool_size_label);

        pool_size_edit = new QLineEdit(groupBox);
        pool_size_edit->setObjectName(QString::fromUtf8("pool_size_edit"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, pool_size_edit);

        lease_label = new QLabel(groupBox);
        lease_label->setObjectName(QString::fromUtf8("lease_label"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, lease_label);

        lease_edit = new QLineEdit(groupBox);
        lease_edit->setObjectName(QString::fromUtf8("lease_edit"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lease_edit);

        router_label = new QLabel(groupBox);
        router_label->setObjectName(QString::fromUtf8("router_label"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, router_label);

        router_edit = new QLineEdit(groupBox);
        router_edit->setObjectName(QString::fromUtf8("router_edit"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, router_edit);

        mask_label = new QLabel(groupBox);
        mask_label->setObjectName(QString::fromUtf8("mask_label"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, mask_label);

        mask_edit = new QLineEdit(groupBox);
        mask_edit->setObjectName(QString::fromUtf8("mask_edit"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, mask_edit);

        dns_label = new QLabel(groupBox);
        dns_label->setObjectName(QString::fromUtf8("dns_label"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, dns_label);

        dns_edit = new QLineEdit(groupBox);
        dns_edit->setObjectName(QString::fromUtf8("dns_edit"));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, dns_edit);


        formLayout->setWidget(2, QFormLayout::SpanningRole, groupBox);


        layout_right->addLayout(formLayout);

        widget = new QWidget(layoutWidget1);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        statu_label = new QLabel(widget);
        statu_label->setObjectName(QString::fromUtf8("statu_label"));
        statu_label->setMaximumSize(QSize(20, 20));
        statu_label->setStyleSheet(QString::fromUtf8("border-image: url(:/images/offline-point.png);"));

        horizontalLayout_2->addWidget(statu_label);

        run_btn = new QPushButton(widget);
        run_btn->setObjectName(QString::fromUtf8("run_btn"));

        horizontalLayout_2->addWidget(run_btn);


        layout_right->addWidget(widget);

        splitter->addWidget(layoutWidget1);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 792, 23));
        MainWindow->setMenuBar(menuBar);
        QWidget::setTabOrder(cb_interface, server_edit);
        QWidget::setTabOrder(server_edit, ip_pool_edit);
        QWidget::setTabOrder(ip_pool_edit, pool_size_edit);
        QWidget::setTabOrder(pool_size_edit, lease_edit);
        QWidget::setTabOrder(lease_edit, router_edit);
        QWidget::setTabOrder(router_edit, mask_edit);
        QWidget::setTabOrder(mask_edit, dns_edit);
        QWidget::setTabOrder(dns_edit, run_btn);
        QWidget::setTabOrder(run_btn, listWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "DHCP Server", 0, QApplication::UnicodeUTF8));
        actionClear->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Open Config", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionStart->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save Config", 0, QApplication::UnicodeUTF8));
        actionVersion->setText(QApplication::translate("MainWindow", "Version", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save ", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "\345\272\217\345\217\267", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">ip</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">mac</p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\350\277\233\345\272\246</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_interface->setText(QApplication::translate("MainWindow", "Interface", 0, QApplication::UnicodeUTF8));
        label_router->setText(QApplication::translate("MainWindow", "Server IP", 0, QApplication::UnicodeUTF8));
        server_edit->setText(QApplication::translate("MainWindow", "192.168.1.1", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "DHCP Options", 0, QApplication::UnicodeUTF8));
        ip_pool_label->setText(QApplication::translate("MainWindow", "IP Pool Start", 0, QApplication::UnicodeUTF8));
        pool_size_label->setText(QApplication::translate("MainWindow", "Size of Pool", 0, QApplication::UnicodeUTF8));
        pool_size_edit->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        lease_label->setText(QApplication::translate("MainWindow", "Lease", 0, QApplication::UnicodeUTF8));
        lease_edit->setText(QApplication::translate("MainWindow", "2880", 0, QApplication::UnicodeUTF8));
        router_label->setText(QApplication::translate("MainWindow", "Router", 0, QApplication::UnicodeUTF8));
        router_edit->setText(QApplication::translate("MainWindow", "192.168.1.1", 0, QApplication::UnicodeUTF8));
        mask_label->setText(QApplication::translate("MainWindow", "Mask", 0, QApplication::UnicodeUTF8));
        mask_edit->setText(QApplication::translate("MainWindow", "255.255.255.0", 0, QApplication::UnicodeUTF8));
        dns_label->setText(QApplication::translate("MainWindow", "DNS", 0, QApplication::UnicodeUTF8));
        dns_edit->setText(QApplication::translate("MainWindow", "8.8.8.8", 0, QApplication::UnicodeUTF8));
        run_btn->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
