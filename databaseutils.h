#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>



class DatabaseUtils
{
public:
    DatabaseUtils();
    bool connectDatabase();
    bool is_checked(QString username,QString password);
};

#endif // DATABASEUTILS_H
