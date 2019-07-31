#include "d_donate.h"
#include "ui_d_donate.h"

d_donate::d_donate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_donate)
{
    ui->setupUi(this);
    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont a_font(family, 26);

    ui->l_become->setFont(a_font);
}

d_donate::~d_donate()
{
    delete ui;
}

void d_donate::on_pushButton_clicked()
{
    QUrl url("https://www.patreon.com/firmeldir");
    QDesktopServices::openUrl(url);
}
