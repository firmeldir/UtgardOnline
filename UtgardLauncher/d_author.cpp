#include "d_author.h"
#include "ui_d_author.h"

d_author::d_author(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_author)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont a_font(family, 28);

    ui->label->setFont(a_font);

}

d_author::~d_author()
{
    delete ui;
}
