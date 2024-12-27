#pragma once
#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>

namespace Ui {
class Message;
}

class Message : public QDialog
{
    Q_OBJECT

public:
    explicit Message(QWidget *parent = nullptr);
    ~Message();
    void setCurrentLogin(QString login);
    void setContraLogin(QString login);
    QString getCurrentLogin();
    QString getContraLogin();
    void setIdCurrent(int id);
    int getIdCurrent();
    void setIdContra(int id);
    int getIdContra();
    void setFilter();


private slots:
    void on_okButton_clicked();
    void slotTimerAlarm();

private:
    Ui::Message *ui;
    QSqlQueryModel* model; 
    QString currentLogin;
    QString contraLogin;
    int idCurrent = 0;
    int idContra = 0;
    QTimer *timer;
    int rowCountInModel = 0;


};


