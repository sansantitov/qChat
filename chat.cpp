#include "chat.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QtSql>


bool Chat::isUsersEmpty()
{
    QSqlQuery query;
    if (!query.exec("SELECT * FROM users;")) {
        //qDebug() << "Unable to execute isUsersEmpty";
        qDebug() << query.lastError();
        return false;
    }
    return query.size() == 1;
}

int Chat::findUserByLogin(QString login)
{
    int id = -1;
    QSqlQuery query;
    QString strF = "SELECT id, login FROM users WHERE activ = 1 AND login = '%1';";
    QString str = strF.arg(login);
    if (!query.exec(str)) {
        qDebug() << query.lastError();
        return id;
    }
    if (query.size() > 0) {
        QSqlRecord rec = query.record();
        while (query.next()) {
            id = query.value(rec.indexOf("id")).toInt();
        }

    }
    return id;
}



int Chat::findUserIdByLoginPassword(QString login, unsigned int digest)
{
    int isAdmin = -1;
    QSqlQuery query;
    QString strF = "SELECT id, login, digest, isadmin FROM users WHERE activ = 1 AND login = '%1' AND digest = %2;";
    QString str = strF.arg(login)
                .arg(digest);
    if (!query.exec(str)) {
        //qDebug() << "Unable to execute findUserIdByLoginPassword";
        qDebug() << query.lastError();
        return isAdmin;
    }
    if (query.size() > 0) {
        QSqlRecord rec = query.record();
        while (query.next()) {
            isAdmin = query.value(rec.indexOf("isadmin")).toInt();
        }
    }
    return isAdmin;
}


void Chat::addUser(QString login, unsigned int digest, int isadmin, int activ)
{
    QSqlQuery query;
    QString strF = "INSERT INTO users (login, digest, isadmin, activ) "
                   "VALUES ('%1', %2, %4, %5)";
    QString str = strF.arg(login)
            .arg(digest)
            .arg(isadmin)
            .arg(activ)
            ;
    if (!query.exec(str)) {
        qDebug() << "Unable to make insert operation";
    }
}


