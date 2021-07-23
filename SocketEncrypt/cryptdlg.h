#ifndef CRYPTDLG_H
#define CRYPTDLG_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>


namespace Ui {
class cryptDlg;
}

class cryptDlg : public QWidget
{
    Q_OBJECT

public:
    explicit cryptDlg(QWidget *parent = nullptr);
    ~cryptDlg();
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;

private slots:
    void on_decryptBtn_clicked();

private:
    //初始化server操作
    void initServer();
    //close server
    void closeServer();
    //更新当前状态
    void updateState();

private:
    Ui::cryptDlg *ui;
    //server用于监听端口，获取新的tcp连接的描述符
    QTcpServer *server;
    //存储已连接的socket对象
    QList<QTcpSocket*> clientList;
};

#endif // CRYPTDLG_H
