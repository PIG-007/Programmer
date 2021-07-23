#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void login(); //登录主界面信号
    void close_window();  //关闭登录界面信号

private slots:
    void on_loginBtn_clicked();
    void on_regBtn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
