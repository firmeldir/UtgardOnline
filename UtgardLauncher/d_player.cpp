#include "d_player.h"
#include "ui_d_player.h"

d_player::d_player(QString username, QString degree,QPixmap pm, int lvl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_player)
{
    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont p_font(family, 28);
    QFont d_font(family, 16);

    ui->setupUi(this);
    ui->l_name->setText(username);
    ui->l_name->setFont(p_font);
    ui->l_lvl->setText("<" + QString::number(lvl) + ">");
    ui->l_lvl->setFont(p_font);
    ui->l_degree->setFont(d_font);
    ui->l_degree->setText("Degree :   " + degree);

    if(!pm.isNull()){ ui->l_avatar_2->setPixmap(pm); }
}

d_player::~d_player()
{
    delete ui;
}
