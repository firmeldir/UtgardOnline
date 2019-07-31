#include "httpserver.h"

HttpServer::HttpServer(QObject *parent) : QTcpServer(parent)
{
}

HttpServer::~HttpServer()
{
    DB_Manager::cleanupDatabase();
    foreach(QString connectionName, QSqlDatabase::connectionNames())
    {
        qDebug() << "Closing database connection" << connectionName;
        QSqlDatabase::database(connectionName, false).close();
    }
}

void HttpServer::startServer()
{
    if(listen(QHostAddress::Any, 1002))
    {
        qDebug() << "Server started";
    }
    else
    {
        qDebug() << "Server: not started";
    }
}

void HttpServer::incomingConnection(qintptr socketDescriptor)
{
    MyThread *thread = new MyThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
