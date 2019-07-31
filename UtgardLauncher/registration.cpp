#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QUrl url, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    this->manager = new QNetworkAccessManager;
    this->url = url;

    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");

    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont r_font(family, 24);

    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->l_reg->setFont(r_font);
}

Registration::~Registration()
{
    delete manager;
    delete ui;
}

void Registration::on_pushButton_clicked()
{
    std::string username = ui->e_username->text().toStdString();
    std::string email = ui->e_email->text().toStdString();
    std::string pass1 = ui->e_pass1->text().toStdString();
    std::string pass2 = ui->e_pass2->text().toStdString();
    this->ui->pushButton->setEnabled(false);
    this->ui->pushButton_2->setEnabled(false);

        if(pass1 != pass2)
        {
            QMessageBox messege; messege.setText("Passwords do not match"); messege.exec();
            this->ui->pushButton->setEnabled(true);
            this->ui->pushButton_2->setEnabled(true);
        }
        else if(username.empty() || pass1.empty() || pass2.empty())
        {
            QMessageBox messege; messege.setText("Fields are empty"); messege.exec();
            this->ui->pushButton->setEnabled(true);
            this->ui->pushButton_2->setEnabled(true);
        }
        else if(!(username.size() > 5 && username.size() < 14))
        {
            QMessageBox messege; messege.setText("Username is not the right size"); messege.exec();
            this->ui->pushButton->setEnabled(true);
            this->ui->pushButton_2->setEnabled(true);
        }
        else if(!(pass1.size() > 6 && pass1.size() < 20))
        {
            QMessageBox messege; messege.setText("Password is not the right size"); messege.exec();
            this->ui->pushButton->setEnabled(true);
            this->ui->pushButton_2->setEnabled(true);
        }
        else //todo validation
        {
            QJsonObject userObject;
            userObject["username"] = QString::fromStdString(username);
            userObject["email"] = QString::fromStdString(email);
            userObject["password"] = QString::fromStdString(pass1);

            QUrl nurl = url.toString() + "/users";
            QNetworkRequest request(nurl);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QNetworkReply *reply = manager->post(request, QJsonDocument(userObject).toJson(QJsonDocument::Compact));
            connect( reply, SIGNAL(finished()),this, SLOT(regFinished()) );
        }
}

void Registration::regFinished()
{
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        qDebug() << codec->toUnicode(content.data());
        accept();
      }
      else
      {
        QMessageBox messege; messege.setText("This username already exists"); messege.exec();
        qDebug() << reply->errorString();
        this->ui->pushButton->setEnabled(true);
        this->ui->pushButton_2->setEnabled(true);
      }
      reply->deleteLater();

}

void Registration::on_pushButton_2_clicked()
{
    reject();
}
