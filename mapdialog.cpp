#include "mapdialog.h"
#include "ui_mapdialog.h"
#include <QTimer>

MapDialog::MapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapDialog)
{
    ui->setupUi(this);
}

MapDialog::~MapDialog()
{
    delete ui;
}

QString MapDialog::getName()
{
    return name;
}

void MapDialog::on_buttonBox_accepted()
{
    name = ui->lineEdit->text();
}
