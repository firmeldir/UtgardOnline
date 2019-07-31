#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "qdbutgardlib_global.h"

#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <string>
#include <QtSql>
#include <vector>


struct QDBUTGARDLIBSHARED_EXPORT Info
{
    QString username;
    int lvl;
    int exp;
    QString degree;
    QString achiv;
    QByteArray image;
};

struct QDBUTGARDLIBSHARED_EXPORT skInfo
{
    QString name;
    QString description;
    QString damage;
};


class QDBUTGARDLIBSHARED_EXPORT DB_Manager
{
    const std::string dir_name;
    QSqlDatabase * db;

public:

    static QSqlDatabase getDatabase();
    static void cleanupDatabase();

    static int getIdByLog(std::string username, std::string password); //&
    static bool deleteById(int id); //&
    static Info getInfoById(int id); //todo
    static bool changeNameById(int id , std::string username); //&
    static QList<Info> * getSUsers();//&
    static QList<int>* getLUsers();
    static bool insertNew(std::string username, std::string password, std::string email = ""); //&
    static bool insertImage(int id, QByteArray image);
    static QList<skInfo> * getSkills();
};

#endif // DB_MANAGER_H
