#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <pcap.h>
#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <QThread>
#include "libcapthread.h"

#define ARP_REQUEST     1


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    // Put packet data
    void printTableInfo(QString strSourceIp,QString strSourceMac,
                        QString strDestIp,QString strDestMac,QString strLeng,const u_char* pktData);

    // get pktData all
    void getTableItem(int row ,int);

    //thread start run
    void runclicked();
//    //get device
    void getDevice();
    // put the information of device
    void OnListItemClicked(QModelIndex index);

    //void test(u_char* user, const struct pcap_pkthdr* header, const u_char* packet);

    void stopThread();

private:
    Ui::MainWindow *ui;
    libcapThread* threadObj;
    QStringList listDevice;
    QStringListModel *model;
};


//static void baseAnalyze(u_char* user, const struct pcap_pkthdr* pHeader, const u_char* pPktdata);

static void arpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui);
static void ipHandle(u_char* packet,int pktLen,Ui::MainWindow* ui);
static void tcpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui);
static void udpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui);
static void icmpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui);
static void allHandle(u_char* packet,int pktLen,Ui::MainWindow* ui);


#endif // MAINWINDOW_H
