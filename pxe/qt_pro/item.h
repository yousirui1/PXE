#ifndef CLIENITEM_H
#define CLIENITEM_H

#include <QLabel>
#include <QProgressBar>
#include <QWidget>
#include "global.h"

namespace Ui {
class Item;
}

class Item : public QWidget
{
    Q_OBJECT

public:
    explicit Item(QWidget *parent = NULL);
    ~Item();

private:
    Ui::Item *ui;

public:
    QLabel *index_label;
    QLabel *ip_label;
    QLabel *mac_label;
    QProgressBar *progress_bar;
};

#endif // CLIENITEM_H
