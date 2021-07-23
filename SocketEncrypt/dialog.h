#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>

namespace Ui {
class dialog;
}

class dialog : public QWidget
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = nullptr);
    ~dialog();

private slots:
//    void on_initPortBtn_clicked();


private:
    //初始化server操作
    void initServer();
    //close server
    void closeServer();
    //更新当前状态
    void updateState();

private:
    Ui::dialog *ui;
    //server用于监听端口，获取新的tcp连接的描述符
    QTcpServer *server;
    //存储已连接的socket对象
    QList<QTcpSocket*> clientList;
};

#endif // DIALOG_H
