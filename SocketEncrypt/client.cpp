#include "client.h"
#include "ui_client.h"
#include <QStyleFactory>

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    setWindowTitle("Client");
    initClient();
}

client::~client()
{
    delete ui;
}


void client::initClient()
{
    //创建client对象
    clientSocket = new QTcpSocket(this);

    //点击连接，根据ui设置的服务器地址进行连接
    connect(ui->btnConnect,&QPushButton::clicked,[this]{
        //判断当前是否已连接，连接了就断开
        if(clientSocket->state()==QAbstractSocket::ConnectedState){
            //如果使用disconnectFromHost()不会重置套接字，isValid还是会为true
            clientSocket->abort();
        }else if(clientSocket->state()==QAbstractSocket::UnconnectedState){
            //从界面上读取ip和端口
            const QHostAddress address=QHostAddress(ui->editAddress->text());
            const unsigned short port=ui->editPort->text().toUShort();
            //连接服务器
            clientSocket->connectToHost(address,port);
        }else{
            ui->textRecv->append("It is not ConnectedState or UnconnectedState");
        }
    });

    //连接状态
    connect(clientSocket,&QTcpSocket::connected,[this]{
        //已连接就设置为不可编辑
        ui->btnConnect->setText("Disconnect");
        ui->editAddress->setEnabled(false);
        ui->editPort->setEnabled(false);
        updateState();
    });
    connect(clientSocket,&QTcpSocket::disconnected,[this]{
        //断开连接还原状态
        ui->btnConnect->setText("Connect");
        ui->editAddress->setEnabled(true);
        ui->editPort->setEnabled(true);
        updateState();
    });

    //发送数据
    connect(ui->btnSend,&QPushButton::clicked,[this]{
        //判断是可操作，isValid表示准备好读写
        if(!clientSocket->isValid())
            return;
        //将发送区文本发送给客户端
        const QByteArray send_data=ui->textSend->toPlainText().toUtf8();
        //数据为空就返回
        if(send_data.isEmpty())
            return;
        clientSocket->write(send_data);
        //client->waitForBytesWritten();
    });

    //收到数据，触发readyRead
    connect(clientSocket,&QTcpSocket::readyRead,[this]{
        //没有可读的数据就返回
        if(clientSocket->bytesAvailable()<=0)
            return;
        //注意收发两端文本要使用对应的编解码
        const QString recv_text=QString::fromUtf8(clientSocket->readAll());
        ui->textRecv->append(QString("[%1:%2]")
                             .arg(clientSocket->peerAddress().toString())
                             .arg(clientSocket->peerPort()));
        ui->textRecv->append(recv_text);
    });

    //error信号在5.15换了名字
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    //错误信息
    connect(client, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            [this](QAbstractSocket::SocketError){
        ui->textRecv->append("Socket Error:"+client->errorString());
    });
#else
    //错误信息
    connect(clientSocket,&QAbstractSocket::errorOccurred,[this](QAbstractSocket::SocketError){
        ui->textRecv->append("Socket Error:"+clientSocket->errorString());
    });
#endif
}

void client::updateState()
{
    //将当前client地址和端口写在标题栏
    if(clientSocket->state()==QAbstractSocket::ConnectedState){
        setWindowTitle(QString("Client[%1:%2]")
                       .arg(clientSocket->localAddress().toString())
                       .arg(clientSocket->localPort()));
    }else{
        setWindowTitle("Client");
    }
}
