#include "admmessages.h"
#include "ui_admmessages.h"
#include <QTimer>

AdmMessages::AdmMessages(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdmMessages)
{
    ui->setupUi(this);
    setWindowIcon(QPixmap(":/chat"));
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(1000); // И запустим таймер
    model = new QSqlQueryModel();
    setFilter();


}

AdmMessages::~AdmMessages()
{
    delete model;
    delete ui;
    timer->stop();
    delete timer;
}


void AdmMessages::setFilter()
{
    QString str;
    str = "SELECT '<' || ufrom.login || '-to-' || uto.login || '>:' || message || '=' "
          "|| to_char(datetime, 'HH24:MI DD.MM.YY ') "
          "AS full_name FROM msgs "
          "JOIN users AS ufrom ON idfrom = ufrom.id "
          "JOIN users AS uto ON idto = uto.id  "
          "ORDER BY datetime";
    model->setQuery(str);
    ui->tableView->scrollToBottom();
    int newRowCount = model->rowCount();
    if (newRowCount > rowCountInModel)
    {
        ui->tableView->setModel(model);
        rowCountInModel = newRowCount;
    }
}
