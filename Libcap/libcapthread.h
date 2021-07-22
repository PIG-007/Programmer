#ifndef LIBCAPTHREAD_H
#define LIBCAPTHREAD_H

#include <QThread>
#include <pcap.h>
#include <iostream>
//#include "mainwindow.h"



class libcapThread : public QThread
{
    Q_OBJECT
public:

    void run();
    static void baseAnalyze(u_char* user,
                const struct pcap_pkthdr* header,const u_char* pkt_data);

signals:
    void signalpcap(QString,QString,QString,
                    QString,QString,const u_char*);
};

#endif // LIBCAPTHREAD_H
