#include "item.h"
#include "ui_item.h"

Item::Item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Item)
{
    ui->setupUi(this);

    index_label = ui->index_label;
    ip_label = ui->ip_label;
    mac_label = ui->mac_label;
    progress_bar = ui->download_bar;
}

Item::~Item()
{
    delete ui;
}
