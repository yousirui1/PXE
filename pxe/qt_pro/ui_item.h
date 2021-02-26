/********************************************************************************
** Form generated from reading UI file 'item.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEM_H
#define UI_ITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Item
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *index_label;
    QLabel *ip_label;
    QLabel *mac_label;
    QProgressBar *download_bar;

    void setupUi(QWidget *Item)
    {
        if (Item->objectName().isEmpty())
            Item->setObjectName(QString::fromUtf8("Item"));
        Item->resize(336, 45);
        horizontalLayout = new QHBoxLayout(Item);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        index_label = new QLabel(Item);
        index_label->setObjectName(QString::fromUtf8("index_label"));
        index_label->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(index_label);

        ip_label = new QLabel(Item);
        ip_label->setObjectName(QString::fromUtf8("ip_label"));

        horizontalLayout->addWidget(ip_label);

        mac_label = new QLabel(Item);
        mac_label->setObjectName(QString::fromUtf8("mac_label"));

        horizontalLayout->addWidget(mac_label);

        download_bar = new QProgressBar(Item);
        download_bar->setObjectName(QString::fromUtf8("download_bar"));
        download_bar->setMaximumSize(QSize(100, 16777215));
        download_bar->setValue(0);

        horizontalLayout->addWidget(download_bar);


        retranslateUi(Item);

        QMetaObject::connectSlotsByName(Item);
    } // setupUi

    void retranslateUi(QWidget *Item)
    {
        Item->setWindowTitle(QApplication::translate("Item", "Form", 0, QApplication::UnicodeUTF8));
        index_label->setText(QApplication::translate("Item", "0", 0, QApplication::UnicodeUTF8));
        ip_label->setText(QApplication::translate("Item", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        mac_label->setText(QApplication::translate("Item", "aa:bb:cc:dd", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Item: public Ui_Item {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITEM_H
