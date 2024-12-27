#pragma once
#include <QString>
#include <QtSql>

bool initDb(); //соединение или создание-соединение с базой данных
bool createDb(); //создание базы данных
bool createTables(); //создание базы данных
bool connectDb(QString dbName); //установка соединения
void closeDb(); //закрытие соединения
void setDarkStyle(); //установка темного стиля


