#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>

#include <QDBUtgardLib/include/db_manager.h>
#include <string>

class MyThread : public QThread
{
  Q_OBJECT

  public:
    explicit MyThread(qintptr ID, QObject *parent = nullptr);
    ~MyThread();

    void run();

  signals:
    void error(QTcpSocket::SocketError socketerror);

  public slots:
    void readyRead();
    void disconnected();

  private:
    QTcpSocket *socket;
    qintptr socketDescriptor;

    void analysis(QString httpRequest, QString& method, QString& uri, QString& body);
    void process(QString& method, QString& uri, QString& body);

    void sendVecOnline(bool r, QList<Info>* nlist = nullptr);
    void sendLog(bool r, int id = 0);
    void sendDelete(bool r);
    void sendReg(bool r);
    void sendChange(bool r);
    void sendImage(bool r);
    void sendUser(bool r, QJsonObject qjo);
    void sendSkills(bool r, QList<skInfo>* nlist = nullptr);
};

#endif // THREAD_H
