#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>

namespace Ui {
class MapDialog;
}

class MapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDialog(QWidget *parent = nullptr);
    ~MapDialog();

    QString getName();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MapDialog *ui;

    QString name;
};

#endif // MAPDIALOG_H
