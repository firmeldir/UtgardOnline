#pragma once
#include <string>
#include <vector>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

struct playerInfo
{
    int lvl;
    int exp;
    std::vector<std::string> skills;
    std::vector<std::string> cskills;
};

class DB_Manager
{
    QSqlDatabase * db;

public:
    DB_Manager();
    ~DB_Manager();

    void uploadData(int id, playerInfo info);
    playerInfo loadData(int id);

    void setDegree(int id, QString degree);
    void setAchievement(int id, QString achievement);

    void setOnline(int id, bool online);
};
