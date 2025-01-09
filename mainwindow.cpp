#include "mainwindow.h"
#include "main.h"
#include "ui_mainwindow.h"
#include "setting.h"
#include "admmessages.h"
#include <QMenu>


//#include <memory>


MainWindow * MainWindow::pMainWindow = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    setWindowIcon(QPixmap(":/chat"));
    ui->setupUi(this);

    pMainWindow = this;
    model->setQuery("SELECT login FROM users WHERE activ = 1 ORDER BY login;");
    ui->tableView->setModel(model);
    ui->tableView->setUpdatesEnabled(true);

    actSetting = new QAction();
    actSetting->setParent(this);
    actSetting->setIcon(QPixmap(":/setting"));
    connect(actSetting, SIGNAL(triggered()), SLOT(slotSetting()));
    ui->toolBar->addAction(actSetting);

    actMyName = new QAction();
    actMyName->setParent(this);
    connect(actMyName, SIGNAL(triggered()), SLOT(slotSetting()));
    ui->toolBar->addAction(actMyName);

    actAdmin = new QAction();
    actAdmin->setParent(this);
    actAdmin->setText("Админ");
    actAdmin->setVisible(false);

    QMenu* pmnu = new QMenu("");
    QAction* actUsers = new QAction("Пользователи",pmnu);
    connect(actUsers,SIGNAL(triggered()), SLOT(actUsersSlot()));
    QAction* actMessages = new QAction("Сообщения",pmnu);

    pmnu->addAction(actUsers);
    pmnu->addAction(actMessages);
    actAdmin->setMenu(pmnu);
    ui->toolBar->addAction(actAdmin);

    connect(actUsers,SIGNAL(triggered()), SLOT(admUsersSlot()));
    connect(actMessages,SIGNAL(triggered()), SLOT(admMessagesSlot()));

}



MainWindow::~MainWindow()
{
    delete ui;
}



MainWindow *MainWindow::getMainWinPtr()
{
    return pMainWindow;
}


void MainWindow::setCurrentLogin(QString curLogin)
{
    currentLogin = curLogin;
    actMyName->setText(curLogin);
}


QString MainWindow::getCurrentLogin()
{
    return currentLogin;
}



void MainWindow::setCurrentIsAdmin(int isAdm)
{
    isAdmin = isAdm;
}


int MainWindow::getCurrentIsAdmin()
{
    return isAdmin;
}




void MainWindow::setShowHideActAdmin()
{
    actAdmin->setVisible(getCurrentIsAdmin() == 1);
}

void MainWindow::slotSetting()
{
    setting = new Setting();
    if (setting != nullptr)
    {
        int ret;
        while (true) {
            ret = setting->exec();
            if (ret == 2 || ret == QDialog::Accepted) break;
        }
        if (ret == QDialog::Accepted) {
            QString queryStr = model->query().executedQuery();
            model->clear();
            model->query().clear();
            model->setQuery(queryStr);
        }
        delete setting;
    }

}


void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString contraLogin = index.data(Qt::EditRole).toString();
    message = new Message();
    if (message != nullptr)
    {
        message->setCurrentLogin(getCurrentLogin());
        message->setContraLogin(contraLogin);
        message->setIdCurrent(chat->findUserByLogin(getCurrentLogin()));
        message->setIdContra(chat->findUserByLogin(contraLogin));
        if (message->getIdContra() == 1) message->setWindowTitle(contraLogin);
        else message->setWindowTitle(getCurrentLogin() + " <-> " +contraLogin);
        message->setFilter();
        message->exec();
        delete message;
    }
}


void MainWindow::admMessagesSlot()
{
    admMessages = new AdmMessages();
    if (admMessages != nullptr)
    {
        admMessages->setFilter();
        admMessages->exec();
        delete admMessages;
    }

}

void MainWindow::admUsersSlot()
{
    admUsers = new AdmUsers();
    if (admUsers != nullptr)
    {
        admUsers->exec();
        delete admUsers;
    }
}




