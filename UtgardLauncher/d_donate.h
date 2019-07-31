#ifndef D_DONATE_H
#define D_DONATE_H

#include <QDialog>
#include <QFontDatabase>
#include <QDesktopServices>
#include <QUrl>


namespace Ui {
class d_donate;
}

class d_donate : public QDialog
{
    Q_OBJECT

public:
    explicit d_donate(QWidget *parent = nullptr);
    ~d_donate();

private slots:
    void on_pushButton_clicked();

private:
    Ui::d_donate *ui;
};

#endif // D_DONATE_H
