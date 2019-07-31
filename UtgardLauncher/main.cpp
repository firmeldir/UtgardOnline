#include <QUrl>
#include <QApplication>
#include "playerwindow.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);


//    QUrl url = QUrl("http://localhost:1002/");
//    PlayerWindow pw(15, url);;
//    pw.show();

    MainWindow mw;
    mw.show();

    return a.exec();
}
