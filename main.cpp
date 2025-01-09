#include "chat.h"
#include "mainwindow.h"
#include "setting.h"
#include "functions.h"
#include <QApplication>
#include <QStyleFactory>




//QSqlDatabase mysql;

int main(int argc, char *argv[])
{
    //Подключаемся к серверу и создаем БД (при ее отсутствии)
    if (!initDb()) return 1;

    QApplication a(argc, argv);
    setDarkStyle(); //темный стиль
    MainWindow w;

    w.show();


    std::unique_ptr<Chat> chat = std::make_unique<Chat>();


    //окно настройки
    w.setting = new Setting();
    if (w.setting != nullptr)
    {
        while (true)
        {
            int ret = w.setting->exec();
            if (ret == 2) {
                w.close();
                return 0;
            }
            if (ret == QDialog::Accepted) break;
        }
        delete w.setting;
        QString queryStr = w.model->query().executedQuery();
        w.model->clear();
        w.model->query().clear();
        w.model->setQuery(queryStr);
    }
    return a.exec();
}
