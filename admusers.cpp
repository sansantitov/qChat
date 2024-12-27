#include "admusers.h"
#include "ui_admusers.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>


AdmUsers::AdmUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdmUsers)
{
    ui->setupUi(this);
    setWindowIcon(QPixmap(":/chat"));
    model = new QSqlQueryModel();
    model->setQuery("SELECT id, login, activ, isadmin FROM users WHERE digest > 0 ORDER BY id");
    ui->tableView->setModel(model);
    connect(ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(onRowChanged(QModelIndex)));
}

AdmUsers::~AdmUsers()
{
    delete model;
    delete ui;
}

void AdmUsers::onRowChanged(QModelIndex index)
{
    int row = index.row();
    currentRec = model->record(row);
}

void AdmUsers::on_pushActiv_clicked()
{
    int currentId = currentRec.value(0).toInt();
    int activ = currentRec.value(2).toInt();
    QSqlQuery query;
    QString strF = "UPDATE users SET activ=%1 WHERE id = %2;";
    QString str = strF.arg(activ==0?1:0).arg(currentId);
    if (!query.exec(str)) {
        qDebug() << query.lastError();
        return ;
    }
    QString queryStr = model->query().executedQuery();
    model->clear();
    model->query().clear();
    model->setQuery(queryStr);

}


void AdmUsers::on_pushAdmin_clicked()
{
    int currentId = currentRec.value(0).toInt();
    int isadmin = currentRec.value(3).toInt();
    QSqlQuery query;
    QString strF = "UPDATE users SET isadmin=%1 WHERE id = %2;";
    QString str = strF.arg(isadmin==0?1:0).arg(currentId);
    if (!query.exec(str)) {
        qDebug() << query.lastError();
        return ;
    }
    QString queryStr = model->query().executedQuery();
    model->clear();
    model->query().clear();
    model->setQuery(queryStr);

}

