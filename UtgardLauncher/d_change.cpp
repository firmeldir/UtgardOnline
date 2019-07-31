#include "d_change.h"
#include "ui_d_change.h"

d_change::d_change(QString oname, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_change)
{
    ui->setupUi(this);
    ui->lineEdit->setText(oname);
    this->setFixedSize(this->size());
}

std::string d_change::getName()
{
    QString toOut = ui->lineEdit->text();
    return toOut.toStdString();
}

d_change::~d_change()
{
    delete ui;
}

void d_change::on_buttonBox_accepted()
{
    value = ui->lineEdit->text();
}

void d_change::on_lineEdit_textChanged(const QString &arg1)
{
    value = ui->lineEdit->text();
}
