#include "d_wiki.h"
#include "ui_d_wiki.h"

d_wiki::d_wiki(QNetworkAccessManager* manager, QUrl url, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_wiki)
{
    QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    int id = QFontDatabase::addApplicationFont(":/fonts/res/16251.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont p_font(family, 23);
    QFont d_font(family, 16);

    ui->setupUi(this);
    this->manager = manager;

    ui->n_button->setEnabled(false);
    ui->e_button->setEnabled(false);
    ui->m_button->setEnabled(false);

    ui->n_label->setFont(p_font);
    ui->d_label->setFont(d_font);

    QUrl nurl = url.toString() + "skills/";
    QNetworkRequest request(nurl);

    QNetworkReply *reply = manager->get(request);
    connect( reply, SIGNAL(finished()),this, SLOT(getFinished()) );
}

d_wiki::~d_wiki()
{
    delete ui;
}

void d_wiki::getFinished()
{
    ui->listWidget->clear();
    QNetworkReply *reply= qobject_cast<QNetworkReply *>(sender());
      if (reply->error() == QNetworkReply::NoError)
      {
        QByteArray content= reply->readAll();

        QJsonDocument jd = QJsonDocument::fromJson(content);

            QJsonObject root = jd.object();
            QJsonValue jv = root.value("skill");
            QJsonArray ja = jv.toArray();

            for (int levelIndex = 0; levelIndex < ja.size(); ++levelIndex)
            {
                QJsonObject subtree = ja.at(levelIndex).toObject();

                skInfo si;QString buff =  subtree["name"].toString();
                QStringList list = buff.split('_');
                for(int i = 0; i < list.size(); i++){ si.name += list.at(i); si.name += " "; }

                si.damage = subtree["damage"].toString();
                si.description = subtree["description"].toString();

                QVariant qvar;
                qvar.setValue(si);
                QListWidgetItem * newitem = new QListWidgetItem();
                newitem->setText(si.name);
                newitem->setData(Qt::UserRole, qvar);

                this->ui->listWidget->addItem(newitem);
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

void d_wiki::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->d_label->setText("Damage : " + item->data(Qt::UserRole).value<skInfo>().damage);
    ui->textBrowser->setText(item->data(Qt::UserRole).value<skInfo>().description);
    ui->n_label->setText(item->data(Qt::UserRole).value<skInfo>().name);
}

void d_wiki::on_pushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString current_dir = QDir::currentPath();
    QString default_name = "utgard.wiki_1.0.0.4.xml";
    QString folder_path = dialog.getSaveFileName(
        this,
        "Select Folder",
        current_dir + "/" + default_name,
        "Folders");
    qDebug() << folder_path;


    QDomDocument doc;
    QDomElement mainEl = doc.createElement("utgard.wiki_1.0.0.4");
    QDomElement skills = doc.createElement("skills");

    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);

        QDomElement skillEl = doc.createElement("skill");

        QDomElement nameEl = doc.createElement("name");
        QDomElement descriptionEl = doc.createElement("description");
        QDomElement damageEl = doc.createElement("damage");

        QDomText nameText = doc.createTextNode(item->data(Qt::UserRole).value<skInfo>().name);
        QDomText descriptionText = doc.createTextNode(item->data(Qt::UserRole).value<skInfo>().description);
        QDomText damageText = doc.createTextNode(QString::number(item->data(Qt::UserRole).value<skInfo>().damage.toInt()));

        nameEl.appendChild(nameText);
        descriptionEl.appendChild(descriptionText);
        damageEl.appendChild(damageText);

        skillEl.appendChild(nameEl);
        skillEl.appendChild(descriptionEl);
        skillEl.appendChild(damageEl);

        skills.appendChild(skillEl);
    }

    mainEl.appendChild(skills);
    doc.appendChild(mainEl);

    QFile file( folder_path );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << doc.toString() << endl;
    }
}
