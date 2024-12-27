#pragma once

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlIndex>

namespace Ui {
class AdmUsers;
}

class AdmUsers : public QDialog
{
    Q_OBJECT

public:
    explicit AdmUsers(QWidget *parent = nullptr);
    ~AdmUsers();

private slots:
    void onRowChanged(QModelIndex index);
    void on_pushActiv_clicked();
    void on_pushAdmin_clicked();

private:
    Ui::AdmUsers *ui;
    QSqlQueryModel* model;
    QSqlRecord currentRec;
    QModelIndex currentIndex;


};


