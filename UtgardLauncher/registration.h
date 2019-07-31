#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QJsonObject>
#include <QTextCodec>
#include <QJsonDocument>
#include <QObject>

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QUrl url, QWidget *parent = nullptr);
    ~Registration();

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void regFinished();

private:
    Ui::Registration *ui;
    QNetworkAccessManager* manager;

    QUrl url;
};

#endif // REGISTRATION_H
