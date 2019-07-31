#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QThreadPool>

#include "mythread.h"
#include <QDBUtgardLib/include/db_manager.h>

class HttpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer();

    void startServer();

signals:

public slots:

private:
    QThreadPool *pool;
    //DB_Manager *db;

    void incomingConnection(qintptr socketDescriptor);
};

#endif // HTTPSERVER_H
