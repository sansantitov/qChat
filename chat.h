#pragma once
#include <QString>


class Chat
{
public:
    bool isUsersEmpty();
    int findUserByLogin(QString login); //поиск ID пользователя по логину
    //поиск пользователя по логину-паролю, возврат  -1 если нет, 1-админ, 0-обычный пользователь
    int findUserIdByLoginPassword(QString login, unsigned int digest);
    void addUser(QString login, unsigned int digest, int isadmin, int activ); //добавление пользователя
};

