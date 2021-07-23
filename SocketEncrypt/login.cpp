#include "login.h"
#include "ui_login.h"
#include <QFile>
#include <QFileDialog>
#include <QStyleFactory>

QVector<QString> accountQVec;
QVector<QString> passwordQVec;


void parseUser();
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    parseUser();

}

Login::~Login()
{
    delete ui;
}

void Login::on_loginBtn_clicked()
{

    QString account = ui->accountTextEdit->text();
    QString pswd = ui->pswdTextEdit->text();
    int idx;
    for(auto i : accountQVec)
    {
        if(!account.compare(i))
        {
            idx = accountQVec.indexOf(account);

            if(!pswd.compare(passwordQVec[idx]))
            {
                qDebug() <<"Login successful!";
                emit(login());
                this->destroy();
                return;
            }
        }
    }
    qDebug() <<"Account or password error!Please register first!";
}

void Login::on_regBtn_clicked()
{
    QString account = ui->accountTextEdit->text();
    QString pswd = ui->pswdTextEdit->text();
    QString buf;
    int idx;
    for(auto i : accountQVec)
    {
        if(!account.compare(i))
        {
            qDebug() <<"User exists!";
            return;
        }
    }
    accountQVec.push_back(account);
    passwordQVec.push_back(pswd);
    QFile userFile(".\\userData.txt");
    if(!userFile.open(QIODevice::WriteOnly|QIODevice::Append))//追加打开
        qDebug()<<userFile.errorString();
    buf = buf + "\n";
    buf = buf + account;
    buf = buf + "\t\t";
    buf = buf + pswd;
    userFile.write(buf.toUtf8());
    qDebug() <<"Register successful!";
}

void parseUser()
{
    QFile userFile(".\\userData.txt");
    if(!userFile.open(QIODevice::ReadWrite))//读写打开
        qDebug()<<userFile.errorString();

    QString strTemp;
    bool flag = false;
    while(userFile.atEnd() == false)//文件没到末尾就执行循环体内容
    {

        QString strBuf = userFile.readLine();//读取一行的数据
        QString strAccount = "";
        QString strPswd = "";
        strAccount.clear();
        strPswd.clear();
        for(auto i : strBuf)
        {
            if(flag && i != '\t')
            {
                strPswd += i;
            }
            else {

                if(i == '\t')
                {
                    flag = true;
                    continue;
                }
                else {
                    strAccount += i;
                }
            }
        }
        strPswd.remove('\n');
        accountQVec.push_back(strAccount);
        passwordQVec.push_back(strPswd);
    }
    userFile.close();
}
