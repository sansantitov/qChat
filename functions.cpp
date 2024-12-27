#include "functions.h"
#include "main.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <QStyleFactory>
#include <QApplication>

                                    
QSqlDatabase mysql;



bool initDb()
{
    //Подключаемся к серверу
    mysql = QSqlDatabase::addDatabase(DBDRIVER);
    mysql.setHostName(DBHOST);
    mysql.setUserName(DBUSER);
    mysql.setPassword(DBPASS);
    if (!connectDb(DBNAME)) {
        if (!connectDb(DBSYSNAME)) {
            qDebug() << "Cannot open system database";
            return false;
        }
        if (!createDb()) {
            qDebug() << "Cannot create database";
            return false;
        }
        if (!connectDb(DBNAME)) {
            qDebug() << "Cannot open database";
            return false;
        }
        if (!createTables()) {
            qDebug() << "Cannot create tables";
            return false;
        }
    }
    return true;
}


bool connectDb(QString dbName)
{
    mysql.setDatabaseName(dbName);
    if (!mysql.open()) {
        return false;
    }
    return true;
}


void closeDb()
{
    mysql.close();
}


bool createDb()
{
    QSqlQuery query;
    QString strF = "DROP DATABASE ";
    QString  str = strF + DBNAME;
    query.exec(str);
    strF = "CREATE DATABASE ";
    str = strF + DBNAME;
    if (!query.exec(str)) {
        qDebug() << query.lastError();
        closeDb();
        return false;
    }
    closeDb();
    return true;
}


bool createTables()
{
    QSqlQuery query;
    QString strF;
    QString  str;

    //создание таблиц
    //article_id bigint generated always as identity primary key,
    //query.clear();
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
        "id BIGINT generated always as identity primary key,"
        "login VARCHAR(64),"
        "digest BIGINT,"
        "isadmin INT,"
        "activ INT"
        ")"
        )) {
        qDebug() << query.lastError();
        return false;
    };

    //query.clear();
    if (!query.exec("CREATE TABLE IF NOT EXISTS msgs ("
        "id BIGINT generated always as identity (start with 1) primary key,"
        "datetime TIMESTAMP,"
        "idfrom INT,"
        "idto INT,"
        "message VARCHAR(1024)"
        ")"
        )) {
        qDebug() << query.lastError();
        return false;
    };


    if (!query.exec("INSERT INTO users (login, digest, isadmin, activ) "
                    "VALUES (' Общие сообщения', 0, 0, 1)")) {
        qDebug() << query.lastError();
        return false;
    };

    qDebug() << "Database is created!\n";
    return true;
}


void setDarkStyle()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QFont defaultFont = QApplication::font();
    defaultFont.setPointSize(defaultFont.pointSize()+2);
    qApp->setFont(defaultFont);
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
    darkPalette.setColor(QPalette::Base,QColor(42,42,42));
    darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
    darkPalette.setColor(QPalette::Text,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
    darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
    darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
    darkPalette.setColor(QPalette::Button,QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
    darkPalette.setColor(QPalette::BrightText,Qt::red);
    darkPalette.setColor(QPalette::Link,QColor(42,130,218));
    darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
    darkPalette.setColor(QPalette::HighlightedText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));
    qApp->setPalette(darkPalette);
}



