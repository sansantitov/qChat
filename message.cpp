#include "message.h"
#include "ui_message.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>


Message::Message(QWidget *parent) :
    QDialog(parent,Qt::WindowSystemMenuHint),
    ui(new Ui::Message)
{
    ui->setupUi(this);
    setWindowIcon(QPixmap(":/chat"));
    model = new QSqlQueryModel();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    timer->start(1000); // И запустим таймер
    setFilter();
}

Message::~Message()
{
    delete model;
    delete ui;
    timer->stop();
    delete timer;
}


void Message::setFilter()
{
    int idCurrent = getIdCurrent();
    int idContra =  getIdContra();
    QString strF, str;

    if (idContra == 1) { //общие сообщения
        strF = "SELECT '<' || ufrom.login || '>:' || message || '=' "
                       "|| to_char(datetime, 'HH24:MI DD.MM.YY ') "
                       "AS full_name FROM msgs "
                       "JOIN users AS ufrom ON idfrom = ufrom.id "
                       "WHERE (idto = 1) "
                       "ORDER BY datetime";
        str = strF.arg(idCurrent);

    }
    else {
        strF = "SELECT '<' || ufrom.login || '-to-' || uto.login || '>:' || message || '=' "
                       "|| to_char(datetime, 'HH24:MI DD.MM.YY ') "
                       "AS full_name FROM msgs "
                       "JOIN users AS ufrom ON idfrom = ufrom.id "
                       "JOIN users AS uto ON idto = uto.id  "
                       "WHERE ((idto = %1 AND idfrom = %2) OR (idto = %2 AND idfrom = %1)) "
                       "ORDER BY datetime";
        str = strF.arg(idCurrent)
             .arg(idContra);

    }

    model->setQuery(str);
    ui->tableView->scrollToBottom();
    int newRowCount = model->rowCount();
    if (newRowCount > rowCountInModel)
    {
        ui->tableView->setModel(model);
        rowCountInModel = newRowCount;

    }
    ui->message->setFocus();
}


void Message::on_okButton_clicked()
{
    QString text = ui->message->text();
    if (!text.isEmpty())
    {
        int idCurrent = getIdCurrent();
        int idContra =  getIdContra();

        QDateTime d = QDateTime::currentDateTime();
        QString dd = d.toString("yyyy-MM-dd hh:mm:ss");

        QSqlQuery query;
        QString strF = "INSERT INTO msgs (datetime, idfrom, idto, message) "
                       "VALUES ('%1', %2, %3, '%4')";
        QString str = strF.arg(dd)
                          .arg(idCurrent)
                          .arg(idContra)
                          .arg(text);
        query.exec(str);
        ui->message->clear();
        ui->message->setFocus();

        setFilter();

    }


}



void Message::setCurrentLogin(QString login)
{
    currentLogin = login;
}

void Message::setContraLogin(QString login)
{
    contraLogin = login;
}

QString Message::getCurrentLogin()
{
    return currentLogin;
}

QString Message::getContraLogin()
{
    return contraLogin;
}


void Message::setIdCurrent(int id)
{
    idCurrent = id;
}

int Message::getIdCurrent()
{
    return idCurrent;
}
void Message::setIdContra(int id)
{
    idContra = id;
}

int Message::getIdContra()
{
    return idContra;
}

void Message::slotTimerAlarm()
{
    setFilter();
}

