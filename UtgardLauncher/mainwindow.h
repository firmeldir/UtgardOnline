#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QTextCodec>
#include <QMainWindow>
#include <QVariant>
#include <QFontDatabase>
#include <QCoreApplication>
#include <string>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QListWidget>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>
#include <QTimer>

#include "registration.h"
#include "playerwindow.h"
#include "ui_mainwindow.h"
#include "d_player.h"
#include "d_author.h"
#include "d_wiki.h"


namespace Ui {
class MainWindow;
}

struct plInfo
{
    int lvl;
    QString degree;
    QString username;
    QByteArray image;
};

Q_DECLARE_METATYPE(plInfo)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void trigWiki();
    void trigAuthor();

    void on_b_log_clicked();
    void on_b_new_clicked();

    void logFinished();
    void getFinished();

    void on_p_list_itemDoubleClicked(QListWidgetItem *item);
    void close_pw(PlayerWindow *item);
    void slotTimerAlarm();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager* manager;
    PlayerWindow * pw = nullptr;

    const QUrl url = QUrl("http://localhost:1002/");
    QString m_username;
    QTimer* timer;

    void Log(std::string & username, std::string & password);
    void sendGetPlayers();
};

#endif // MAINWINDOW_H
