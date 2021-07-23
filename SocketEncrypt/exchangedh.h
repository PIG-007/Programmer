#ifndef EXCHANGEDH_H
#define EXCHANGEDH_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>



namespace Ui {
class exchangeDH;
}

class exchangeDH : public QWidget
{
    Q_OBJECT

public:
    explicit exchangeDH(QWidget *parent = nullptr);
    ~exchangeDH();
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;

private slots:
    //void on_initPortBtn_clicked();


    void on_genKeyBtn_clicked();

    void on_genComKeyBtn_clicked();

private:
    //初始化server操作
    void initServer();
    //close server
    void closeServer();
    //更新当前状态
    void updateState();

private:
    Ui::exchangeDH *ui;
    //server用于监听端口，获取新的tcp连接的描述符
    QTcpServer *server;
    //存储已连接的socket对象
    QList<QTcpSocket*> clientList;
    long DH_g=0, DH_p=0, DH_ga=0, DH_gb=0, DH_keya=0, DH_keyb=0;
    int DH_serverKey = 0, DH_clientKey = 0,gen_b = 0;
    long DH_comKey = 0;
};

#endif // EXCHANGEDH_H
