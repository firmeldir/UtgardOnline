#include "playerwindow.h"
#include "ui_playerwindow.h"

PlayerWindow::PlayerWindow(int user_id, QUrl url, QString username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerWindow)
{


    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont p_font(family, 28);
    QFont s_font(family, 20);

    this->id = user_id;
    this->manager = new QNetworkAccessManager;
    this->url = url;
    this->p_username = username;

    ui->setupUi(this);
    ui->b_play->setFont(p_font);
    ui->l_name->setFont(p_font);
    ui->l_name->setText(p_username);
    ui->l_achi->setFont(s_font);
    ui->l_level->setFont(s_font);
    ui->l_degree->setFont(s_font);
    ui->l_exp->setFont(s_font);
    this->setFixedSize(this->size());

    sendGetP();

}

PlayerWindow::~PlayerWindow()
{
    delete manager;
    delete ui;
}

void PlayerWindow::sendGetP()
{
    QUrl nurl = url.toString() + "users/" + std::to_string(this->id).c_str();
    QNetworkRequest request(nurl);

    QNetworkReply *reply = manager->get(request);
    connect( reply, SIGNAL(finished()),this, SLOT(getFinished()) );
}

void PlayerWindow::getFinished()
{
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();

        QJsonDocument jd = QJsonDocument::fromJson(content);


        qDebug() << jd["username"].toString();
        qDebug() << "Level :   "+jd["level"].toString();
        qDebug() << "Experience :   "+jd["experience"].toString();
        qDebug() << "Degree :   "+jd["degree"].toString();
        qDebug() << "Latest achievement :   "+jd["achievement"].toString();

        this->ui->l_name->setText(jd["username"].toString());
        this->ui->l_level->setText("Level :   "+jd["level"].toString());
        this->ui->l_exp->setText("Experience :   "+jd["experience"].toString());
        this->ui->l_degree->setText("Degree :   "+jd["degree"].toString());
        this->ui->l_achi->setText("Latest achievement :   "+jd["achievement"].toString());

        QByteArray image = QByteArray::fromBase64(jd["image"].toVariant().toByteArray());
        if(!image.isEmpty()){ QPixmap savatar; savatar.loadFromData(image,"PNG"); this->ui->l_avatar->setPixmap(savatar); }
      }
      else
      {
         QMessageBox messege; messege.setText("Getting data error"); messege.exec();
         qDebug() << reply->errorString();
         manager->clearAccessCache();
      }

      reply->deleteLater();
}

void PlayerWindow::on_b_delete_clicked()
{
    d_delete dw;
    if(dw.exec())
    {
        QUrl nurl = url.toString() + "users/" + std::to_string(this->id).c_str();
        QNetworkRequest request(nurl);

        QNetworkReply *reply = manager->sendCustomRequest(request,"DELETE");
        connect( reply, SIGNAL(finished()),this, SLOT(deleteFinished()) );
    }
}

void PlayerWindow::deleteFinished()
{
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        qDebug() << codec->toUnicode(content.data());

        emit wClosed(this);
      }
      else
      {
         QMessageBox messege; messege.setText("Server error"); messege.exec();
         qDebug() << reply->errorString();
         manager->clearAccessCache();
      }

      reply->deleteLater();
}

void PlayerWindow::on_b_change_clicked()
{
    d_change cw(ui->l_name->text());

    if(cw.exec())
    {
        QUrl nurl = url.toString() + "users/" + std::to_string(this->id).c_str();
        QNetworkRequest request(nurl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");

        QByteArray nameString = cw.getName().c_str();
        this->p_username = cw.getName().c_str();
        QNetworkReply *reply = manager->post(request, nameString);
        connect( reply, SIGNAL(finished()),this, SLOT(changeFinished()) );
    }
}

void PlayerWindow::changeFinished()
{
    qDebug() << "OK";
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        qDebug() << codec->toUnicode(content.data());
        ui->l_name->setText(this->p_username);
      }
      else
      {
        QMessageBox messege; messege.setText("Server error"); messege.exec();
        qDebug() << reply->errorString();
        this->p_username = ui->l_name->text();
        manager->clearAccessCache();
      }
      reply->deleteLater();
}

void PlayerWindow::on_b_donate_clicked()
{
    d_donate dd;
    dd.exec();
}

void PlayerWindow::on_b_play_clicked()
{
    QProcess process(this);
    QString exe = "../Coursework/Client2/x64/Release/Client.exe ";
    exe += QString::number(id);
    process.startDetached(exe);
}


void PlayerWindow::on_b_avatar_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                               this,
                               "Open File",
                               "",
                               "Images (*.png)");

    if(fileName.isEmpty() || fileName.isNull()){ return; }

    QPixmap navatar;
    navatar.load(fileName);

    QUrl nurl = url.toString() + "users/" + std::to_string(this->id).c_str() + "/image";
    QNetworkRequest request(nurl);

    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    navatar.save(&buffer, "PNG");

    if(navatar.width() > 640 || navatar.height() > 640){ QMessageBox messege; messege.setText("Image too large"); messege.exec(); return;}

    ui->l_avatar->setPixmap(navatar);

    QJsonObject userObject;
    userObject["image"] = QJsonValue::fromVariant(bArray.toBase64());

    size_t size_b  = QJsonDocument(userObject).toJson(QJsonDocument::Compact).toStdString().size();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, size_b);
    QNetworkReply *reply = manager->post(request, QJsonDocument(userObject).toJson(QJsonDocument::Compact));
    connect( reply, SIGNAL(finished()),this, SLOT(imageFinished()) );
}

void PlayerWindow::imageFinished()
{
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        qDebug() << codec->toUnicode(content.data());
      }
      else
      {
         QMessageBox messege; messege.setText("Server error"); messege.exec();
         qDebug() << reply->errorString();
         manager->clearAccessCache();
      }

      reply->deleteLater();
}
