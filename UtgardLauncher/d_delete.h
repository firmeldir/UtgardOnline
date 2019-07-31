#ifndef D_DELETE_H
#define D_DELETE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QTextCodec>
#include <QMainWindow>
#include <QFontDatabase>
#include <QMessageBox>
#include <QVariant>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDialog>

namespace Ui {
class d_delete;
}

class d_delete : public QDialog
{
    Q_OBJECT

public:
    explicit d_delete(QWidget *parent = nullptr);
    ~d_delete();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::d_delete *ui;
};

#endif // D_DELETE_H
