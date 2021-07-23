#include "dialogfile.h"
#include "ui_dialogfile.h"

#include <QDebug>
#include <QDataStream>
#include <QPointer>
#include <QFileInfo>
#include <QStyleFactory>

dialogFile::dialogFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialogFile),
    tcpSocket(nullptr),
    fileSize(0),
    fileBytesReceived(0)

{
    ui->setupUi(this);
    qApp->setStyle(QStyleFactory::create("fusion"));
    setWindowTitle("File Server");
    initServer();

}

dialogFile::~dialogFile()
{
    delete ui;
}

void dialogFile::onNewConnection()
{
    tcpSocket = server->nextPendingConnection();
    clientList.append(tcpSocket);
    ui->plainTextEditLog->appendPlainText(QString("[%1:%2] Soket Connected")
                         .arg(tcpSocket->peerAddress().toString())
                         .arg(tcpSocket->peerPort()));

    connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QObject::deleteLater);
    connect(tcpSocket, &QIODevice::readyRead, this, &dialogFile::onReadyRead);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));


    m_inStream.setDevice(tcpSocket);
    m_inStream.setVersion(QDataStream::Qt_5_0);
}

void dialogFile::onReadyRead()
{
    if (0 == fileSize && tcpSocket->bytesAvailable() > sizeof(qint64)) {
        m_inStream >> fileSize >> fileName;
        fileHandle.setFileName(fileName);
        if (!fileHandle.open(QIODevice::WriteOnly)) {
            qCritical() << fileHandle.errorString();
            return;
        }
        ui->plainTextEditLog->appendPlainText(QStringLiteral("Recving[%1] ...").arg(fileName));
    }

    qint64 size = qMin(tcpSocket->bytesAvailable(), fileSize - fileBytesReceived);

    QByteArray arry(size, 0);
    m_inStream.readRawData(arry.data(), size);
    fileHandle.write(arry);

    fileBytesReceived += size;

    if (fileBytesReceived == fileSize) {
        fileHandle.close();
        QFileInfo info(fileName);
        ui->plainTextEditLog->appendPlainText(QStringLiteral("Recv successful[%1] -> %2").arg(fileName).arg(info.absoluteFilePath()));
        reset();
    }
}

void dialogFile::reset()
{
    fileName.clear();
    fileSize = 0;
    fileBytesReceived = 0;
}

void dialogFile::onSocketError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, QStringLiteral("Error!"), QStringLiteral("%1").arg(tcpSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
        ui->plainTextEditLog->appendPlainText(QStringLiteral("Transmission Stop！"));
        reset();
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, QStringLiteral("Error!"), QStringLiteral("%1").arg(tcpSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::critical(this, QStringLiteral("Error!"), QStringLiteral("%1").arg(tcpSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
        break;
    default:
        break;
    }
}


void dialogFile::initServer()
{
    //创建Server对象
    server = new QTcpServer(this);

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
                if(server->listen(address,port))
                {
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

    connect(server, &QTcpServer::newConnection, this, &dialogFile::onNewConnection);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &dialogFile::close);
}

void dialogFile::closeServer()
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

void dialogFile::updateState()
{
    //将当前server地址和端口、客户端连接数写在标题栏
    if(server->isListening()){
        setWindowTitle(QString("FileServer[%1:%2] connections:%3")
                       .arg(server->serverAddress().toString())
                       .arg(server->serverPort())
                       .arg(clientList.count()));
    }else{
        setWindowTitle("FileServer");
    }
}


