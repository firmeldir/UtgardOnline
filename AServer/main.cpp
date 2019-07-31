#include <QApplication>
#include "httpserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HttpServer server;
    server.startServer();

    return a.exec();
}
