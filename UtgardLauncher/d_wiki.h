#ifndef D_WIKI_H
#define D_WIKI_H

#include <QDialog>
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
#include <QFileDialog>
#include <QtXml>

namespace Ui {
class d_wiki;
}


struct skInfo
{
    QString name;
    QString description;
    QString damage;
};

Q_DECLARE_METATYPE(skInfo)

class d_wiki : public QDialog
{
    Q_OBJECT

public:
    explicit d_wiki(QNetworkAccessManager* manager, QUrl url, QWidget *parent = nullptr);
    ~d_wiki();

private:
    QNetworkAccessManager* manager;
    Ui::d_wiki *ui;

private slots:
    void getFinished();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();
};

#endif // D_WIKI_H
