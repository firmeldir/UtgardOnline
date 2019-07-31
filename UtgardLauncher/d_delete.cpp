#include "d_delete.h"
#include "ui_d_delete.h"

d_delete::d_delete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_delete)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());
}

d_delete::~d_delete()
{
    delete ui;
}

void d_delete::on_buttonBox_accepted()
{
    this->ui->buttonBox->setEnabled(false);
}
