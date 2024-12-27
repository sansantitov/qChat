#include "setting.h"
#include "mainwindow.h"
#include "chat.h"
#include "sha1.h"
#include "ui_setting.h"
#include <QStyle>
#include <QtSql>
#include <QMessageBox>
#include <QRegularExpression>


class RegPasswordValidator: public QValidator {
public:
    RegPasswordValidator(QObject* parent) : QValidator(parent) {}
    virtual State validate (QString& password, int& /*pos*/) const
    {
          QRegExp rxp = QRegExp("^[a-zA-Z0-9]+$");
        if (!password.isEmpty() && !password.contains(rxp)) {
            return Invalid;
        }
        return Acceptable;
    }
};


Setting::Setting(QWidget *parent)
    : QDialog(parent, Qt::WindowSystemMenuHint)
    , ui(new Ui::Setting)
{
    setWindowIcon(QPixmap(":/setting"));
    ui->setupUi(this);


    MainWindow *mw = MainWindow::getMainWinPtr();
    if (mw !=nullptr)
        ui->lineLogin->setText(mw->getCurrentLogin());

    connect(ui->regGroup, &QGroupBox::clicked, this, &Setting::onQGroupclicked);
    RegPasswordValidator* pRegPasswordValidator = new RegPasswordValidator(ui->regPassword);
    ui->regPassword->setValidator(pRegPasswordValidator);

    connect(ui->pushCancel, &QPushButton::clicked, [=]
    {
        this->done(2);
    });

    connect(ui->pushOK, &QPushButton::clicked, [=]
        {
            MainWindow *mw = MainWindow::getMainWinPtr();
            int result = QDialog::Rejected; //0
            // QDialog::Rejected 0
            if (ui->regGroup->isChecked()) {
                if (!ui->regLogin->text().isEmpty() && mw->chat->findUserByLogin(ui->regLogin->text()) >= 0) {
                    QMessageBox::information(0, "Информация", "Такой логин уже зарегистрирован!");
                }
                else if (!ui->regPassword->text().isEmpty() && ui->regPassword->text() != ui->regPassword2->text()) {
                    QMessageBox::information(0, "Информация", "Пароли не совпадают!");
                }
                else if (mw->chat->isUsersEmpty() && !ui->regAdmin->isChecked()) {
                    QMessageBox::information(0, "Информация", "Должен быть хотя бы один администратор!");
                }
                else if (!ui->regLogin->text().isEmpty() && !ui->regPassword->text().isEmpty()
                    && ui->regPassword->text() == ui->regPassword2->text()) {
                    QByteArray ba = ui->regPassword->text().toLocal8Bit();
                    const char *p = ba.data();
                    uint* ddd = sha1(p, (uint)ui->regPassword->text().length());
                    mw->chat->addUser(ui->regLogin->text(), *ddd, ui->regAdmin->isChecked()?1:0, 1);
                    QMessageBox::information(0, "Информация", "Пользователь зарегистрирован!");
                }
                else QMessageBox::information(0, "Информация", "Пользователь не зарегистрирован!");
                result = QDialog::Rejected;
                this->done(result);
            }
            else {
                QByteArray ba = ui->linePassword->text().toLocal8Bit();
                const char *p = ba.data();
                uint* ddd = sha1(p, (uint)ui->linePassword->text().length());
                int isAdmin = mw->chat->findUserIdByLoginPassword(ui->lineLogin->text(), *ddd);
                if (isAdmin < 0) {
                    QMessageBox::information(0,"Информация", "Логин-пароль не соответствуют ни одному пользователю!");
                    result = QDialog::Rejected;
                    this->done(result);
                }
                else {
                    QMessageBox::information(0,"Информация", "Вход произведен!");
                    mw->setCurrentLogin(ui->lineLogin->text());
                    mw->setCurrentIsAdmin(isAdmin);
                    mw->setShowHideActAdmin();
                    result = QDialog::Accepted;
                    this->done(result);
                }
            }

        });

}

Setting::~Setting()
{
    delete ui;
}


void Setting::onQGroupclicked()
{
    ui->labelLogin->setDisabled(ui->regGroup->isChecked());
    ui->lineLogin->setDisabled(ui->regGroup->isChecked());
    ui->labelPassword->setDisabled(ui->regGroup->isChecked());
    ui->linePassword->setDisabled(ui->regGroup->isChecked());

    ui->reglabLogin->setEnabled(ui->regGroup->isChecked());
    ui->regLogin->setEnabled(ui->regGroup->isChecked());
    ui->reglabPassword->setEnabled(ui->regGroup->isChecked());
    ui->regPassword->setEnabled(ui->regGroup->isChecked());
    ui->reglabPassword2->setEnabled(ui->regGroup->isChecked());
    ui->regPassword2->setEnabled(ui->regGroup->isChecked());

    ui->regAdmin->setEnabled(ui->regGroup->isChecked());
}



