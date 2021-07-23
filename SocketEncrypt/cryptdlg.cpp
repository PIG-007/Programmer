#include "cryptdlg.h"
#include "ui_cryptdlg.h"
#include <QStyleFactory>
#include "CA.h"
#include "caesar.h"
#include "Playfair.h"
#include "keyword.h"
#include "my.h"
#include "Affine_Cipher.h"
#include "AutoKey_ciphertext.h"
#include "AutoKey_plaintext.h"
#include "VigenereCipher.h"
#include "RC4.h"
#include "RSA.h"
#include "double_transprotion.h"
#include "DH.h"
#include "tool.h"
#include "des.h"
#include "stdafx.h"
#include<vector>
#include<iomanip>
#include<MD5.h>
//#include"ECC.h"
//#include"ECC_run.h"


using namespace std;


cryptDlg::cryptDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cryptDlg)
{
    ui->setupUi(this);
    qApp->setStyle(QStyleFactory::create("fusion"));
    setWindowTitle("Crypt Server");
    initServer();
}

cryptDlg::~cryptDlg()
{
    delete ui;
}

void cryptDlg::initServer()
{
    //创建Server对象
    server = new QTcpServer(this);

    //点击监听按钮，开始监听
    connect(ui->btnListen,&QPushButton::clicked,[this]{
        //判断当前是否已开启，是则close，否则listen
        if(server->isListening()){
            //server->close();
            closeServer();
            //关闭server后恢复界面状态
            ui->btnListen->setText("Listen");
            ui->editAddress->setEnabled(true);
            ui->editPort->setEnabled(true);
        }else{
            //从界面上读取ip和端口
            //可以使用 QHostAddress::Any 监听所有地址的对应端口
            const QString address_text=ui->editAddress->text();
            const QHostAddress address=(address_text=="Any")
                    ?QHostAddress::Any
                   :QHostAddress(address_text);
            const unsigned short port=ui->editPort->text().toUShort();
            //开始监听，并判断是否成功
            if(server->listen(address,port)){
                //连接成功就修改界面按钮提示，以及地址栏不可编辑
                ui->editAddress->setText(server->serverAddress().toString());
                ui->editPort->setText(QString::number(server->serverPort()));
                ui->btnListen->setText("Close");
                ui->editAddress->setEnabled(false);
                ui->editPort->setEnabled(false);
                ui->statusLabel->setText(QStringLiteral("Status:Listening Now!"));
            }
            else {
                ui->statusLabel->setText(QStringLiteral("Status:Listening Failed!"));
            }
        }
        updateState();
    });

    //监听到新的客户端连接请求
    connect(server,&QTcpServer::newConnection,this,[this]{
        //如果有新的连接就取出
        while(server->hasPendingConnections())
        {
            //nextPendingConnection返回下一个挂起的连接作为已连接的QTcpSocket对象
            //套接字是作为服务器的子级创建的，这意味着销毁QTcpServer对象时会自动删除该套接字。
            //最好在完成处理后显式删除该对象，以避免浪费内存。
            //返回的QTcpSocket对象不能从另一个线程使用，如有需要可重写incomingConnection().
            QTcpSocket *socket=server->nextPendingConnection();
            clientList.append(socket);
            ui->textRecv->appendPlainText(QString("[%1:%2] Soket Connected")
                                 .arg(socket->peerAddress().toString())
                                 .arg(socket->peerPort()));

            //关联相关操作的信号槽
            //收到数据，触发readyRead
            connect(socket,&QTcpSocket::readyRead,[this,socket]{
                //没有可读的数据就返回
                if(socket->bytesAvailable()<=0)
                    return;
                //注意收发两端文本要使用对应的编解码
                const QString recv_text=QString::fromUtf8(socket->readAll());
                ui->textRecv->appendPlainText(QString("[%1:%2]")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort()));
                ui->textRecv->appendPlainText(recv_text);
            });

            //error信号
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
            //错误信息
            connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
                    [this,socket](QAbstractSocket::SocketError){
                ui->textRecv->append(QString("[%1:%2] Soket Error:%3")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort())
                                     .arg(socket->errorString()));
            });
#else
            //错误信息
            connect(socket,&QAbstractSocket::errorOccurred,[this,socket](QAbstractSocket::SocketError){
                ui->textRecv->appendPlainText(QString("[%1:%2] Soket Error:%3")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort())
                                     .arg(socket->errorString()));
            });
#endif

            //连接断开，销毁socket对象，这是为了开关server时socket正确释放
            connect(socket,&QTcpSocket::disconnected,[this,socket]{
                socket->deleteLater();
                clientList.removeOne(socket);
                ui->textRecv->appendPlainText(QString("[%1:%2] Soket Disonnected")
                                     .arg(socket->peerAddress().toString())
                                     .arg(socket->peerPort()));
                updateState();
            });
        }
        updateState();
    });

//    //server向client发送内容
//    connect(ui->btnSend,&QPushButton::clicked,[this]{
//        //判断是否开启了server
//        if(!server->isListening())
//            return;
//        //将发送区文本发送给客户端
//        const QByteArray send_data=ui->textSend->toPlainText().toUtf8();
//        //数据为空就返回
//        if(send_data.isEmpty())
//            return;
//        for(QTcpSocket *socket:clientList)
//        {
//            socket->write(send_data);
//            //socket->waitForBytesWritten();
//        }
//    });

    //server的错误信息
    //如果发生错误，则serverError()返回错误的类型，
    //并且可以调用errorString()以获取对所发生事件的易于理解的描述
    connect(server,&QTcpServer::acceptError,[this](QAbstractSocket::SocketError){
        ui->textRecv->appendPlainText("Server Error:"+server->errorString());
    });
}

void cryptDlg::closeServer()
{
    //停止服务
    server->close();
    for(QTcpSocket * socket:clientList)
    {
        //断开与客户端的连接
        socket->disconnectFromHost();
        if(socket->state()!=QAbstractSocket::UnconnectedState){
            socket->abort();
        }
    }
}

void cryptDlg::updateState()
{
    //将当前server地址和端口、客户端连接数写在标题栏
    if(server->isListening()){
        setWindowTitle(QString("Server[%1:%2] connections:%3")
                       .arg(server->serverAddress().toString())
                       .arg(server->serverPort())
                       .arg(clientList.count()));
    }else{
        setWindowTitle("Server");
    }
}

long RSA_d, RSA_e, RSA_p, RSA_q, RSA_n;
void cryptDlg::on_decryptBtn_clicked()
{
    string plaintext;
    string key = ui->keyTextEdit->toPlainText().toStdString();
    string ciphertext = ui->getCipherTextEdit->toPlainText().toStdString();
    string strAlogrithm = ui->alogrithmBox->currentText().toStdString();
    const char* alogrithm = strAlogrithm.c_str();
    if(!strcmp(alogrithm,"Caesar cipher")){
        //key.clear();
        plaintext.clear();
        plaintext = Caesar_Decryption(plaintext, ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Keyword cipher")){
        //key.clear();
        plaintext.clear();
        plaintext = KeywordDecrypt(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Multiliteral cipher")){
        //key.clear();
        plaintext = multDecrypt(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    //*-----------------Affine-----------------------------/
    else if(!strcmp(alogrithm,"Affine cipher")){
        plaintext = decode(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Vigenere cipher"))
    {
        //key.clear();
        plaintext = Vdecrypt(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Autokey plaintext")){
        //key.clear();
        plaintext = AtPlain_decode(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Autokey ciphertext")){
        //key.clear();
        plaintext = AtCipher_decode(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Playfair cipher")){
        //key.clear();
        CreateTable(key);
        plaintext = Playfair_Decryption(plaintext, ciphertext);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Permutation cipher")){
        plaintext = permuDecrypt(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Column permutation cipher"))
    {
        plaintext = decrypt_once(ciphertext, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"Double-Transposition cipher")){
        plaintext = double_transDecrypt(ciphertext, key, key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"RC4")){

        //plaintext = decrypt_once(ciphertext, key);
    }
    else if(!strcmp(alogrithm,"CA")){
        //
    }

    else if(!strcmp(alogrithm,"DES")){
        plaintext = DES_Decode(key, ciphertext);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"AES"))
    {
        plaintext = aesDecrypt(ciphertext,16,key);
        ui->putPlaintextEdit->setPlainText(QString::fromStdString(plaintext));
    }
    else if(!strcmp(alogrithm,"RSA")){
        //generateKey(RSA_d, RSA_e, RSA_p, RSA_q, RSA_n);
    }
    else if(!strcmp(alogrithm,"ECC")){
        //
    }

}

