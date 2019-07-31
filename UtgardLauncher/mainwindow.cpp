#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->manager = new QNetworkAccessManager;

    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    QFont g_font(family, 53);
    QFont o_font(family, 33);

    ui->setupUi(this);
    ui->name_label->setFont(g_font);
    ui->online_label->setFont(o_font);

    this->setFixedSize(this->size());


    connect(ui->actionWiki, &QAction::triggered, this, &MainWindow::trigWiki);
    connect(ui->actionAuthor, &QAction::triggered, this, &MainWindow::trigAuthor);

    sendGetPlayers();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    if(pw != nullptr){ delete  this->pw; }
    delete this->manager;
    delete this->ui;
}

void MainWindow::trigWiki()
{
    d_wiki dw(manager, url);
    dw.exec();
}

void MainWindow::trigAuthor()
{
    d_author da;
    da.exec();
}


void MainWindow::slotTimerAlarm()
{
    sendGetPlayers();
}

void MainWindow::sendGetPlayers()
{
    QUrl nurl = url.toString() + "users/";
    QNetworkRequest request(nurl);

    QNetworkReply *reply = manager->get(request);
    connect( reply, SIGNAL(finished()),this, SLOT(getFinished()) );
}

void MainWindow::getFinished()
{
    ui->p_list->clear();
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();

        QJsonDocument jd = QJsonDocument::fromJson(content);

            QJsonObject root = jd.object();
            QJsonValue jv = root.value("user");
            QJsonArray ja = jv.toArray();

            for (int levelIndex = 0; levelIndex < ja.size(); ++levelIndex)
            {
                QJsonObject subtree = ja.at(levelIndex).toObject();

                plInfo pli; pli.lvl = subtree["level"].toInt(); pli.degree = subtree["degree"].toString();
                pli.username = subtree["username"].toString(); pli.image = subtree["image"].toVariant().toByteArray();

                QVariant qvar;
                qvar.setValue(pli);
                QListWidgetItem * newitem = new QListWidgetItem();
                newitem->setText(pli.username + "[" + QString::number( pli.lvl ) + "]");
                newitem->setData(Qt::UserRole, qvar);

                this->ui->p_list->addItem(newitem);
            }
      }
      else
      {
         QMessageBox messege; messege.setText("Getting data error"); messege.exec();
         qDebug() << reply->errorString();
         manager->clearAccessCache();
      }

      reply->deleteLater();
}

void MainWindow::on_b_log_clicked()
{
    std::string username = ui->e_log->text().toStdString();
    std::string password = ui->e_pass->text().toStdString();

    Log(username, password);
}

void MainWindow::on_b_new_clicked()
{
    Registration rg(url);

    if(rg.exec())
    {
        QMessageBox messege; messege.setText("Account created. Welcome!"); messege.exec();
    }
}

void MainWindow::Log(std::string & username, std::string & password)
{
    this->ui->e_log->setEnabled(false);
    this->ui->e_pass->setEnabled(false);
    this->ui->b_log->setEnabled(false);
    this->ui->b_new->setEnabled(false);

    this->m_username = QString::fromStdString(username);
    QJsonObject userObject;
    userObject["password"] = QString::fromStdString(password);

    QUrl nurl = url.toString() + username.c_str();
    QNetworkRequest request(nurl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QJsonDocument(userObject).toJson(QJsonDocument::Compact).size());
    QNetworkReply *reply = manager->post(request, QJsonDocument(userObject).toJson(QJsonDocument::Compact));
    connect( reply, SIGNAL(finished()),this, SLOT(logFinished()) );
}

void MainWindow::logFinished()
{
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        qDebug() << codec->toUnicode(content.data());

        if(pw != nullptr){ delete pw; } this->hide();

        pw = new PlayerWindow(codec->toUnicode(content.data()).toInt(), url,m_username);
        connect( pw, &PlayerWindow::wClosed, this,  &MainWindow::close_pw);
        //pw->setWindowFlags(Qt::SubWindow);
        pw->show();
      }
      else
      {
         QMessageBox messege; messege.setText("The username and password you entered don't match any Utgards account.\n Please try again."); messege.exec();
         qDebug() << reply->errorString();
         this->ui->e_log->setEnabled(true);
         this->ui->e_pass->setEnabled(true);
         this->ui->b_log->setEnabled(true);
         this->ui->b_new->setEnabled(true);
      }
      reply->deleteLater();
}

void MainWindow::close_pw(PlayerWindow * pw)
{
    pw->close();
    this->show();

    this->ui->e_log->setEnabled(true);
    this->ui->e_pass->setEnabled(true);
    this->ui->b_log->setEnabled(true);
    this->ui->b_new->setEnabled(true);

    this->ui->e_log->setText("");
    this->ui->e_pass->setText("");
}


void MainWindow::on_p_list_itemDoubleClicked(QListWidgetItem *item)
{
    this->findChild<QLineEdit*>("e_log")->setEnabled(false);
    this->findChild<QLineEdit*>("e_pass")->setEnabled(false);
    this->findChild<QPushButton*>("b_log")->setEnabled(false);
    this->findChild<QPushButton*>("b_new")->setEnabled(false);


    QByteArray image = QByteArray::fromBase64(item->data(Qt::UserRole).value<plInfo>().image);
    QPixmap savatar; savatar.loadFromData(image,"PNG");

    d_player dp(item->data(Qt::UserRole).value<plInfo>().username,item->data(Qt::UserRole).value<plInfo>().degree,savatar,item->data(Qt::UserRole).value<plInfo>().lvl);

    if(dp.exec())
    {
        this->findChild<QLineEdit*>("e_log")->setEnabled(true);
        this->findChild<QLineEdit*>("e_pass")->setEnabled(true);
        this->findChild<QPushButton*>("b_log")->setEnabled(true);
        this->findChild<QPushButton*>("b_new")->setEnabled(true);
    }

}

