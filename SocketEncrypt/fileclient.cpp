#include "fileclient.h"
#include "ui_fileclient.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QFileInfo>
#include <QStyleFactory>
#include<QRegularExpression>

fileClient::fileClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileClient),
  m_blockSize(1024 * 1024 * 1),
  m_currentFileSize(0),
  m_totalFileBytesWritten(0),
  m_totalFileSize(0),
  m_currentFileBytesWritten(0)
{
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName() + QStringLiteral(" Qt小罗"));
    qApp->setStyle(QStyleFactory::create("fusion"));

//    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
//    ui->lineEditIpAddress->setValidator(new QRegExpValidator(rx, this));

    connect(ui->pushButtonCancel_3, &QPushButton::clicked, this, &fileClient::close);
    connect(ui->pushButtonAddFile_3, &QPushButton::clicked, this, &fileClient::addFile);
    connect(ui->pushButtonSend_3, &QPushButton::clicked, this, &fileClient::sendFile);
    connect(ui->pushButtonDelFile_3, &QPushButton::clicked, this, &fileClient::delFile);
    connect(ui->pushButtonEmptyFile_3, &QPushButton::clicked, this, &fileClient::clearFile);

    connect(&m_socket, &QIODevice::bytesWritten, this, &fileClient::onBytesWritten);
    connect(&m_socket, &QAbstractSocket::stateChanged, this, &fileClient::onSocketStateChanged);
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

fileClient::~fileClient()
{
    delete ui;
}


void fileClient::sendFile()
{
    QString address = ui->lineEditIpAddress_3->text();
    int port = ui->spinBoxPort_3->text().toInt();

    QHostAddress hostAddress;
    if (!hostAddress.setAddress(address)) {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("目标网络地址错误！"));
        return;
    }

    if (0 == ui->listWidget->count()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请选择需要发送的文件！"));
        addFile();
        return;
    }

    m_fileQueue.clear();
    int count = ui->listWidget->count();
    for (int i = 0; i < count; ++i) {
        QString file = ui->listWidget->item(i)->text();
        m_fileQueue.append(file);

        QFileInfo info(file);
        m_totalFileSize += info.size();
    }

    m_socket.connectToHost(address, port);
}

void fileClient::addFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"),
                                                QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if (files.isEmpty()) {
        return;
    }

    ui->listWidget->addItems(files);
}

void fileClient::delFile()
{
    int row = ui->listWidget->currentRow();
    if (row < 0) {
        return;
    }

    delete ui->listWidget->takeItem(row);
}

void fileClient::clearFile()
{
    int count = ui->listWidget->count();
    if (0 == count) {
        return;
    }

    int ret = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确定清空文件列表吗？"),
                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Ok != ret) {
        return;
    }

    for (int i = 0; i < count; ++i) {
        delete ui->listWidget->takeItem(0);
    }
}

void fileClient::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        qDebug() << m_totalFileSize << " " << m_totalFileBytesWritten;
        qDebug() << __FUNCTION__ << "QAbstractSocket::UnconnectedState";
        break;
    case QAbstractSocket::HostLookupState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectedState";
        m_timer.restart();
        send();
        break;
    case QAbstractSocket::BoundState:
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        break;
    default:
        break;
    }
}

void fileClient::onSocketError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
        break;
    default:
        break;
    }
    QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_socket.errorString()));
}

void fileClient::onBytesWritten(const qint64 &bytes)
{
    Q_UNUSED(bytes)

    QByteArray arry = m_file.read(m_blockSize);
    if (arry.isEmpty()) {
        reset();
        return;
    }

    int size = arry.size();
    m_outStream.writeRawData(arry.constData(), size);

    updateProgress(size);
}

void fileClient::send()
{
    m_file.setFileName(m_fileQueue.dequeue());

    if (!m_file.open(QIODevice::ReadOnly)) {
        qCritical() << m_file.errorString();
        QMessageBox::critical(this, QStringLiteral("错误"), m_file.errorString());
        return;
    }

    m_currentFileSize = m_file.size();

    //设置当前文件进度显示格式
    ui->currentProgressBar_3->setFormat(QStringLiteral("%1 : %p%").arg(m_file.fileName()));

    m_outStream.setDevice(&m_socket);
    m_outStream.setVersion(QDataStream::Qt_5_0);

    QFileInfo info(m_file.fileName());
    QString fileName = info.fileName();

    //发送文件大小及文件名
    m_outStream << m_currentFileSize << fileName;

    //开始传输文件
    QByteArray arry = m_file.read(m_blockSize);
    int size = arry.size();
    m_outStream.writeRawData(arry.constData(), size);

    ui->pushButtonSend_3->setEnabled(false);
    updateProgress(size);
}

void fileClient::reset()
{
    m_file.close();
    ui->pushButtonSend_3->setEnabled(true);

    m_currentFileBytesWritten = 0;

    if (m_fileQueue.isEmpty()) {
        m_socket.close();

        qint64 milliseconds = m_timer.elapsed();
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("共耗时：%1 毫秒  平均：%2 KB/s")
                                 .arg(QString::number(milliseconds))
                                 .arg(QString::number(((double(m_totalFileSize) / 1024) / (double(milliseconds) / 1000)), 'f', 3)));
        m_totalFileSize = 0;
        m_totalFileBytesWritten = 0;

    } else {
        send();
    }
}

void fileClient::updateProgress(const int &size)
{
    m_totalFileBytesWritten += size;
    ui->totalProgressBar_3->setValue((double(m_totalFileBytesWritten)/m_totalFileSize) * 100);

    m_currentFileBytesWritten += size;
    ui->currentProgressBar_3->setValue((double(m_currentFileBytesWritten)/m_file.size()) * 100);

    double speed = (double)m_totalFileBytesWritten / m_timer.elapsed();

    //ui->statusBar->showMessage(QStringLiteral("实时速度：%1").arg(QString::number(speed / (1024*1024/1000), 'f', 3) + "MB/s"), 5000);
}

