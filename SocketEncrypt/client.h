#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QWidget>

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

private:
    //初始化client操作
    void initClient();
    //更新当前状态
    void updateState();

private:
    Ui::client *ui;
    //socket对象
    QTcpSocket *clientSocket;
};

#endif // CLIENT_H
