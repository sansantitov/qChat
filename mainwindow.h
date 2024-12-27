#pragma once
#include "chat.h"
#include "setting.h"
#include "message.h"
#include "admmessages.h"
#include "admusers.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow * getMainWinPtr();
    QSqlQueryModel* model = new QSqlQueryModel();
    Setting *setting; // = new Setting();
    std::unique_ptr<Chat> chat = std::make_unique<Chat>();
    void setCurrentLogin(QString curLogin);
    QString getCurrentLogin();
    void setCurrentIsAdmin(int isAdmin);
    int getCurrentIsAdmin();
    void setShowHideActAdmin();



private:
    Ui::MainWindow *ui;
    int isAdmin = 0;
    QAction *actSetting;
    QAction *actMyName;
    QAction *actAdmin;
    Message *message;
    AdmMessages *admMessages;
    AdmUsers *admUsers;

    static MainWindow * pMainWindow;
    QString currentLogin = "";

private slots:
    void slotSetting();
    void on_tableView_activated(const QModelIndex &index);
    void admMessagesSlot();
    void admUsersSlot();
};


