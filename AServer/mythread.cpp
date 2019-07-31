#include "mythread.h"

MyThread::MyThread(qintptr ID, QObject *parent): QThread(parent)
{
    this->socketDescriptor = ID;
}

MyThread::~MyThread()
{
}


void MyThread::run()
{
  socket = new QTcpSocket();

  if (!socket->setSocketDescriptor(this->socketDescriptor) )
  {
    emit error (socket->error());
    return;
  }

  connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::DirectConnection );
  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

  exec();
}


void MyThread::readyRead()
{
    if(socket->bytesAvailable())
    {
        socket->waitForReadyRead(20);
        QByteArray byteArray = socket->readAll();
        qDebug() << byteArray;
        QString method;
        QString uri;
        QString body;
        analysis(QString::fromStdString(byteArray.toStdString()), method, uri, body);
        process(method, uri, body);
        qDebug() << byteArray;

        socket->flush();
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
    }
}


void MyThread::disconnected()
{
  socket->deleteLater();
  exit(0);
}

void MyThread::analysis(QString httpRequest, QString& method, QString& uri, QString& body)
{
    QStringList list = httpRequest.split(' ');
    method = list.at(0);
    uri = list.at(1);

    QStringList list2 = httpRequest.split("\r\n\r\n");

    body = list2.at(1);
}

void MyThread::process(QString& method, QString& uri, QString& body)
{
    QString tname;
    QString id;
    QStringList list = uri.split('/');
    tname = list.at(1);
    if(list.size() >= 3){ id = list.at(2); }
    if(list.size() == 4){ tname += "/" + list.at(3); }

    qDebug() << body;
    qDebug() << tname;
    qDebug() << id;


    if(method == "GET")
    {
        if(tname == "users" && id.isEmpty())
        {
            QList<Info> * qsl = DB_Manager::getSUsers();

            if(qsl == nullptr)
            {
                sendVecOnline(false);
            }
            else
            {
                sendVecOnline(true, qsl);
                delete qsl;
            }
        }
        else if(tname == "users" && !id.isEmpty())
        {
            Info info = DB_Manager::getInfoById(id.toInt());

            QJsonObject userObject;
            userObject["username"] = info.username;
            userObject["level"] = QString::number(info.lvl);
            userObject["experience"] = QString::number(info.exp);
            userObject["degree"] = info.degree;
            userObject["achievement"] = info.achiv;
            userObject["image"] = QJsonValue::fromVariant(info.image.toBase64());

            sendUser(true, userObject);
        }
        else if(tname == "skills")
        {
            QList<skInfo> * qsl = DB_Manager::getSkills();

            if(qsl == nullptr)
            {
                sendVecOnline(false);
            }
            else
            {
                sendSkills(true, qsl);
                delete qsl;
            }
        }
    }
    else if(method == "POST")
    {
        if(tname == "users" && !id.isEmpty())
        {
            sendChange(DB_Manager::changeNameById(id.toInt(), body.toStdString()));
        }
        else if(tname == "users/image")
        {
            QJsonDocument jd = QJsonDocument::fromJson(body.toUtf8());
            QByteArray outByteArray = QByteArray::fromBase64(jd["image"].toVariant().toByteArray());
            sendImage(DB_Manager::insertImage(id.toInt(), outByteArray));
        }
        else if(tname == "users" && id.isEmpty())
        {
            QJsonDocument jd = QJsonDocument::fromJson(body.toUtf8());

            std::string username = jd["username"].toString().toStdString();
            std::string password = jd["password"].toString().toStdString();
            std::string email = jd["email"].toString().toStdString();

            sendReg(DB_Manager::insertNew(username, password, email));
        }
        else if(!tname.isEmpty() && !body.isEmpty())
        {
            QJsonDocument dbody = QJsonDocument::fromJson(body.toLocal8Bit());
            QString password = dbody.object()["password"].toString();

            int id = DB_Manager::getIdByLog(tname.toStdString(), password.toStdString());

            if(id != 0)
            {
                sendLog(true, id);
            }
            else
            {
                sendLog(false, 0);
            }
        }

    }
    else if(method == "DELETE")
    {
        if(tname == "users" && !id.isEmpty())
        {
            sendDelete(DB_Manager::deleteById(id.toInt()));
        }
    }
}

void MyThread::sendUser(bool r, QJsonObject qjo)
{
    QString body;
    int bodyLength = 0;
    QLocale locale(QLocale::English);
    QDateTime currTime = QDateTime::currentDateTimeUtc();
    QString time = "Date: " + locale.toString(currTime, "ddd, ") + locale.toString(currTime, "d MMM yyyy HH:mm:ss") + " GMT";

    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        body = QJsonDocument(qjo).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
    else
    {
        QJsonObject userObject;
        QString response = "HTTP/1.1 500 Internal Server Error\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "false";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
}

void MyThread::sendImage(bool r)
{
    QJsonObject userObject;
    QString body;
    int bodyLength = 0;
    QLocale locale(QLocale::English);

    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "true";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(bodyLength).arg(body).toLocal8Bit());
        qDebug() << "10";
    }
    else
    {
        QString response = "HTTP/1.1 500 Internal Server Error\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "false";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(bodyLength).arg(body).toLocal8Bit());
    }
}

void MyThread::sendVecOnline(bool r, QList<Info>* nlist)
{
    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n";

        QJsonObject usersObject;
        for(int i = 0; i < nlist->size(); i++)
        {
            QJsonObject user;
            user["username"] = nlist->at(i).username;
            user["level"] = nlist->at(i).lvl;
            user["degree"] = nlist->at(i).degree;
            user["image"] =  QJsonValue::fromVariant(nlist->at(i).image.toBase64());

            QJsonArray textsArray = usersObject["user"].toArray();
            textsArray.append(user);
            usersObject["user"] = textsArray;
        }

        response += "Content-type: application/json\r\n";
        response += "Content-length: "+ QString::fromStdString(std::to_string(QJsonDocument(usersObject).toJson(QJsonDocument::Compact).size())) + "\r\n\r\n";

        response += QJsonDocument(usersObject).toJson(QJsonDocument::Compact);

        socket->write(response.toUtf8());
    }
    else
    {
        QString response = "HTTP/1.1 500 Internal Server Error";
        socket->write(response.toUtf8());
    }
}

void MyThread::sendSkills(bool r, QList<skInfo>* nlist)
{
    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n";

        QJsonObject usersObject;
        for(int i = 0; i < nlist->size(); i++)
        {
            QJsonObject user;
            user["name"] = nlist->at(i).name;
            user["damage"] = nlist->at(i).damage;
            user["description"] = nlist->at(i).description;

            QJsonArray textsArray = usersObject["skill"].toArray();
            textsArray.append(user);
            usersObject["skill"] = textsArray;
        }

        response += "Content-type: application/json\r\n";
        response += "Content-length: "+ QString::fromStdString(std::to_string(QJsonDocument(usersObject).toJson(QJsonDocument::Compact).size())) + "\r\n\r\n";

        response += QJsonDocument(usersObject).toJson(QJsonDocument::Compact);

        socket->write(response.toUtf8());
    }
    else
    {
        QString response = "HTTP/1.1 500 Internal Server Error";
        socket->write(response.toUtf8());
    }
}

void MyThread::sendLog(bool r, int id) //&
{
    QJsonObject userObject;
    QString body;
    int bodyLength = 0;

    QLocale locale(QLocale::English);
    QDateTime currTime = QDateTime::currentDateTimeUtc();
    QString time = "Date: " + locale.toString(currTime, "ddd, ") + locale.toString(currTime, "d MMM yyyy HH:mm:ss") + " GMT";

    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n";

        response += "Content-type: text/plain\r\n";
        response += "Content-length: " + QString::number(std::to_string(id).size()) + "\r\n\r\n";
        response +=  QString::fromStdString(std::to_string(id));
        socket->write(response.toUtf8());
    }
    else
    {
        QString response = "HTTP/1.1 404 Not Found\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "false";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
}

void MyThread::sendDelete(bool r) //&
{
    QJsonObject userObject;
    QString body;
    int bodyLength = 0;

    QLocale locale(QLocale::English);
    QDateTime currTime = QDateTime::currentDateTimeUtc();
    QString time = "Date: " + locale.toString(currTime, "ddd, ") + locale.toString(currTime, "d MMM yyyy HH:mm:ss") + " GMT";

    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "true";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
    else
    {
        QString response = "HTTP/1.1 500 Internal Server Error\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "false";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
}

void MyThread::sendReg(bool r) //&
{
    QJsonObject userObject;
    QString body;
    int bodyLength = 0;

    QLocale locale(QLocale::English);
    QDateTime currTime = QDateTime::currentDateTimeUtc();
    QString time = "Date: " + locale.toString(currTime, "ddd, ") + locale.toString(currTime, "d MMM yyyy HH:mm:ss") + " GMT";

    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "true";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
    else
    {
        QString response = "HTTP/1.1 409 Conflict\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "false";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
}

void MyThread::sendChange(bool r) //&
{
    QJsonObject userObject;
    QString body;
    int bodyLength = 0;

    QLocale locale(QLocale::English);
    QDateTime currTime = QDateTime::currentDateTimeUtc();
    QString time = "Date: " + locale.toString(currTime, "ddd, ") + locale.toString(currTime, "d MMM yyyy HH:mm:ss") + " GMT";

    if(r)
    {
        QString response = "HTTP/1.1 200 OK\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "true";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
    else
    {
        QString response = "HTTP/1.1 500 Internal Server Error\r\n%1\r\nContent-type: application/json\r\nContent-length: %2\r\n\r\n%3";
        userObject["succes"] = "false";
        body = QJsonDocument(userObject).toJson(QJsonDocument::Compact);
        bodyLength = body.toLocal8Bit().size();
        socket->write(response.arg(time).arg(bodyLength).arg(body).toLocal8Bit());
    }
}
