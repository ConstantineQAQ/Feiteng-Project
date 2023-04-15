#include "databaseutils.h"

DatabaseUtils::DatabaseUtils()
{

}

bool DatabaseUtils::connectDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("localhost");
    db.setDatabaseName("admindatabase");
    db.setUserName("kylin");
    db.setPassword("kylin");
    if(db.open()){
        qInfo() << "\033[32m数据库连接成功\033[0m" ;
    }
    else{
        qCritical() << "数据库连接失败" ;
    }
    return db.open();
}

bool DatabaseUtils::is_checked(QString username,QString password)
{
    QSqlQuery query;
    query.prepare("select * from admin where username = :username and password = :password");
    query.bindValue(":username",username);
    query.bindValue(":password",password);
    return (query.exec() && query.next());
}
