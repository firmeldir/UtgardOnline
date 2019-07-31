#ifndef D_PLAYER_H
#define D_PLAYER_H

#include <QDialog>
#include <QFontDatabase>
#include <QPixmap>

namespace Ui {
class d_player;
}

class d_player : public QDialog
{
    Q_OBJECT

public:
    explicit d_player(QString username, QString degree,QPixmap pm, int lvl, QWidget *parent = nullptr);
    ~d_player();

private:
    Ui::d_player *ui;
};

#endif // D_PLAYER_H
