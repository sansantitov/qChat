#pragma once

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class AdmMessages;
}

class AdmMessages : public QDialog
{
    Q_OBJECT

public:
    explicit AdmMessages(QWidget *parent = nullptr);
    ~AdmMessages();
    void setFilter();


private:
    Ui::AdmMessages *ui;
    QSqlQueryModel* model;
    QTimer *timer;
    int rowCountInModel = 0;

};


