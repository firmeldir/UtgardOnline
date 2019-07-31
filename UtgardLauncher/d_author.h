#ifndef D_AUTHOR_H
#define D_AUTHOR_H

#include <QDialog>
#include <QFontDatabase>

namespace Ui {
class d_author;
}

class d_author : public QDialog
{
    Q_OBJECT

public:
    explicit d_author(QWidget *parent = nullptr);
    ~d_author();

private:
    Ui::d_author *ui;
};

#endif // D_AUTHOR_H
