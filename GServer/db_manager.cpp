#include "db_manager.h"

DB_Manager::DB_Manager()
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(QString::fromUtf8(("C:/Users/Xiaomi/source/repos/Coursework/AServer/res/main.db")));
    db->open();
}

DB_Manager:: ~DB_Manager()
{
    db->close();
    delete db;
}

void DB_Manager::uploadData(int id, playerInfo info)
{
    int p_id;

    QSqlQuery query3;
    query3.prepare("SELECT * FROM users WHERE id = :id");
    query3.bindValue(":id", id);

    if (!query3.exec()) { qDebug() << "finding error:" << query3.lastError(); return; }

    if (query3.next())
    {
       p_id = query3.value("progress_id").toInt();
    }
    else{ return; }


    ///


    QSqlQuery query2;
    query2.prepare("UPDATE player_progress SET lvl = :lvl,exp = :exp,skill_1 = :sk1,skill_2 = :sk2,skill_3 = :sk3,skill_4 = :sk4 WHERE progress_id = :id");
    query2.bindValue(":id", p_id);
    query2.bindValue(":lvl", info.lvl);
    query2.bindValue(":exp", info.exp);
    query2.bindValue(":sk1", QString::fromStdString(info.cskills.at(0)));
    query2.bindValue(":sk2", QString::fromStdString(info.cskills.at(1)));
    query2.bindValue(":sk3", QString::fromStdString(info.cskills.at(2)));
    query2.bindValue(":sk4", QString::fromStdString(info.cskills.at(3)));

    if (!query2.exec()) { qDebug() << "update error:" << query2.lastError(); return; }

    ///

    for(int i = 0; i < info.skills.size(); i++)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO progress_skills(progress_id, skill_id) VALUES (:prid, :skid)");
        query.bindValue(":skid", QString::fromStdString(info.skills.at(i)));
        query.bindValue(":prid", p_id);

        if (!query.exec()){ qDebug() << "insert error:" << query.lastError();  return; }
    }
}

playerInfo DB_Manager::loadData(int id)
{
    int p_id;
    playerInfo p_info;

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) { qDebug() << "finding error:" << query.lastError(); return p_info; }

    if (query.next())
    {
       p_id = query.value("progress_id").toInt();
    }
    else{ return p_info; }


    ///


    QSqlQuery query3;
    query3.prepare("SELECT * FROM player_progress WHERE progress_id = :id");
    query3.bindValue(":id", p_id);

    if (!query3.exec()) { qDebug() << "finding error:" << query3.lastError(); return p_info; }

    if (query3.next())
    {
       p_info.lvl = query3.value("lvl").toInt();
       p_info.exp = query3.value("exp").toInt();
       p_info.cskills.push_back(query3.value("skill_1").toString().toStdString());
       p_info.cskills.push_back(query3.value("skill_2").toString().toStdString());
       p_info.cskills.push_back(query3.value("skill_3").toString().toStdString());
       p_info.cskills.push_back(query3.value("skill_4").toString().toStdString());
    }
    else{ return p_info; }


    ///


    QSqlQuery query2;
    query2.prepare("SELECT * FROM progress_skills WHERE progress_id = :id");
    query2.bindValue(":id", p_id);

    if (!query2.exec()) { qDebug() << "finding error:" << query2.lastError(); return p_info; }

    while(query2.next())
    {
       p_info.skills.push_back(query2.value("skill_id").toString().toStdString());
    }

    return p_info;
}

void DB_Manager::setDegree(int id, QString degree)
{
    int p_id;
    playerInfo p_info;

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) { qDebug() << "finding error:" << query.lastError(); return; }

    if (query.next())
    {
       p_id = query.value("progress_id").toInt();
    }
    else{ return; }


    ///


    QSqlQuery query2;
    query2.prepare("UPDATE player_progress SET degree = :degree WHERE progress_id = :id");
    query2.bindValue(":id", p_id);
    query2.bindValue(":degree", degree);

    if (!query2.exec()) { qDebug() << "update error:" << query2.lastError(); return; }
}

void DB_Manager::setAchievement(int id, QString achievement)
{
    int p_id;
    playerInfo p_info;

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) { qDebug() << "finding error:" << query.lastError(); return; }

    if (query.next())
    {
       p_id = query.value("progress_id").toInt();
    }
    else{ return; }

    QSqlQuery query2;
    query2.prepare("INSERT INTO player_achievements (achievement_name, progress_id) VALUES (\":achievement\" , :id)");
    query2.bindValue(":id", p_id);
    query2.bindValue(":achievement", achievement);

    if (!query2.exec()) { qDebug() << "insert error:" << query2.lastError(); return; }
}

void DB_Manager::setOnline(int id, bool online)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET online = :online WHERE id = :id");
    if(online){ query.bindValue(":online", 1 ); }else{ query.bindValue(":online", 0); }
    query.bindValue(":id", id);

    if (!query.exec()){  qDebug() << "updating error:" << query.lastError(); return; }
}
