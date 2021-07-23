#ifndef DIALOGFILE_H
#define DIALOGFILE_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QUdpSocket>
#include <QFile>

class QTcpSocket;


namespace Ui {
class dialogFile;
}

class dialogFile : public QWidget
{
    Q_OBJECT

public:
    explicit dialogFile(QWidget *parent = nullptr);
    ~dialogFile();

private slots:
    //void on_initPortBtn_clicked();
    void onNewConnection();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    void reset();
    //初始化server操作
    void initServer();
    //close server
    void closeServer();
    //更新当前状态
    void updateState();

private:
    Ui::dialogFile *ui;
    QList<QTcpSocket*> clientList;
    QFile fileHandle;
    qint64 fileSize;
    QString fileName;
    QDataStream m_inStream;
    QTcpServer* server;
    qint64 fileBytesReceived;
    QTcpSocket *tcpSocket;

};

#endif // DIALOGFILE_H
