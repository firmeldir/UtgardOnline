#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

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
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonObject>
#include <QWidget>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QProcess>

#include "d_delete.h"
#include "d_change.h"
#include "d_donate.h"

namespace Ui {
class PlayerWindow;
}

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerWindow(int user_id, QUrl url, QString username,QWidget *parent = nullptr);
    ~PlayerWindow();

private slots:

    void on_b_delete_clicked();
    void on_b_change_clicked();
    void on_b_donate_clicked();
    void on_b_play_clicked();
    void changeFinished();
    void deleteFinished();
    void imageFinished();
    void getFinished();
    void on_b_avatar_clicked();

signals:

    void wClosed(PlayerWindow*);

private:
    Ui::PlayerWindow *ui;
    QNetworkAccessManager* manager;

    int id;
    QUrl url;
    QString p_username = "";

    void sendGetP();

    void closeEvent (QCloseEvent *event){ emit wClosed(this); event->accept();  }


};

#endif // PLAYERWINDOW_H
