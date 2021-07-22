#include "mainwindow.h"
#include "ui_mainwindow.h"



using namespace std;





//ARP
typedef struct arphdr {
    u_int16_t htype;
    u_int16_t ptype;
    u_char hlen;
    u_char plen;
    u_int16_t oper;
    u_char sha[6];
    u_char spa[4];
    u_char tha[6];
    u_char tpa[4];
} arphdr_t;

//IP
typedef struct {
    unsigned int header_len:4;
    unsigned int version:4;
    u_char tos:8;
    unsigned int total_len:16;
    unsigned int ident:16;
    unsigned int flags:16;
    u_char ttl:8;
    u_char proto:8;
    unsigned int checksum:16;
    u_char sourceIP[4];
    u_char destIP[4];
}IPHEADER;

//TCP
typedef struct
{
    unsigned short sourcePORT;
    unsigned short destPORT;
    unsigned int seq;
    unsigned int ensure_seq;
#if LITTLE_ENDIAN
    unsigned char reserved_1:4;
    unsigned char header_len:4;
    u_char URG:1;
    u_char ACK:1;
    u_char PSH:1;
    u_char RST:1;
    u_char SYN:1;
    u_char FIN:1;
    unsigned char reseverd_2:2;
#else
    unsigned char header_len:4;
    unsigned char reserved_1:4;
    unsigned char reseverd_2:2;
    u_char URG:1;
    u_char ACK:1;
    u_char PSH:1;
    u_char RST:1;
    u_char SYN:1;
    u_char FIN:1;
#endif
    unsigned short window;
    unsigned short CSS;
    unsigned short point;
}TCPHEADER;

//UDP
typedef struct
{
    uint16_t sport;
    uint16_t dport;
    uint16_t udp_length;
    uint16_t udp_sum;
}UDPHEADER;

//ICMP
typedef struct
{
    u_int8_t icmp_type;
    u_int8_t icmp_code;
    u_int16_t icmp_checksum;
    u_int16_t icmp_id_lliiuuwweennttaaoo;
    u_int16_t icmp_sequence;
}ICMPHEADER;



extern void baseAnalyze(u_char* user,
                        const struct pcap_pkthdr* header,const u_char* pkt_data);
bool flag = false;
pcap_t *devHandle;
int pktAmount;
int g_pHeadLen;
const char *dev ;
bpf_u_int32 deviceIP;
bpf_u_int32 deviceMask;
//char *Proto[]={"NULL","ICMP","IGMP","GDP","IP","ST","TCP" };



//init str information
QString msStrSurIpInfo = "";
QString msStrSurMacInfo = "";
QString msStrDestIpInfo = "";
QString msStrDestMacInfo = "";
string msStrProto = "";
QString msStrPackLen = "";
int g_nLenPktData[9999][9999];
u_char g_nArray[9999][9999];
//QVector<u_char*> pktVector;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //thread
    threadObj = new libcapThread;


    //Set Network Device
    model =  new QStringListModel;
    ui->listView->setModel(model);



    connect(ui->serchButton,SIGNAL(clicked()),this,SLOT(getDevice()));

    // Put the information of Network Device
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,
            SLOT(OnListItemClicked(QModelIndex)));

    //start catch packet
    connect(ui->runButton,SIGNAL(clicked()),this,SLOT(runclicked()));

    // click tableWidgetItem
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,
            SLOT(getTableItem(int,int)));

    //connect thread and mainwindow
    connect(threadObj,SIGNAL(signalpcap(QString,QString,QString,
                                         QString,QString,const u_char *)),
            this,SLOT(printTableInfo(QString,QString,QString,
                                     QString,QString,const u_char *)));

    //stop catching
    connect(ui->stopBtn,SIGNAL(clicked()),this,SLOT(stopThread()));
}

MainWindow::~MainWindow()
{
    delete ui;
}



//Slot get Network Device
void MainWindow::getDevice()
{

    char* dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    //search device
    //dev = pcap_lookupdev(errbuf);
    dev = "ens33";
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    if(dev != NULL)
    {
        //Put the information of Network device
        listDevice += dev;
        model->setStringList(listDevice);

    }
    //get
     else
    {
        listDevice += "getDeviec error";
        model->setStringList(listDevice);
    }
}

// Put the information of Network Device
void MainWindow::OnListItemClicked(QModelIndex index)
{
    ui->addrEdit->clear();
    char errbuf[1024];


    //get the device which is been selected
    QByteArray ba = index.data().toString().toLatin1();
    dev = ba.data();
    ui->addrEdit->setText(QString("Surrcess getDevice: %1").arg(dev));



    //open device
    //devHandle = pcap_open_live("any", 65535, 0, 5000, errbuf);
    devHandle = pcap_open_live(dev, 65535, 0, 1000, errbuf);
    if (devHandle == NULL) {
        perror(errbuf);
        exit(1);
    }

    //get the net and mask of the device
    if (pcap_lookupnet(dev, &deviceIP, &deviceMask, errbuf) == -1) {
        perror(errbuf);
        exit(1);
    }
    //Put the information of Network device
    else
    {
        //convert address format
        char ip[INET_ADDRSTRLEN],mask[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET,&deviceIP,ip,sizeof(ip)) == NULL)
            perror("inet_ntop error");
        else if(inet_ntop(AF_INET,&deviceMask,mask,sizeof(mask)) == NULL)
            perror("inet_ntop error");
        printf("IP address: %s, Network Mask: %s\n",ip,mask);
        QString sur_mac = QString("IP address: %1\nNetwork Mask: %2\n")
                .arg(ip).arg(mask);
        ui->addrEdit->append(sur_mac);
    }


}



void MainWindow::runclicked()
{
    if (devHandle == NULL) {
        cout << "Please open a Network Device!";
        ui->textEdit->append("Please open a Network Device!");
        return;
    }
    //flag = false;
    //Clear
    //ui->tableWidget->clearContents();
    int rowCount = ui->tableWidget->rowCount();
    for (int i = 0;i < rowCount;i++) {
        ui->tableWidget->removeRow(0);
    }
    ui->textEdit->clear();

    //goto certain module
    struct bpf_program filter;
    const char* selectModule = ui->moudleComBox->currentText().toStdString().c_str();
    pktAmount = ui->pktAmountLineEdit->text().toInt();
    if(pktAmount == 0)
    {
        pktAmount = -1;
    }

    if(!strcmp(selectModule,"ARP")){
        pcap_compile(devHandle, &filter, "arp", 0, deviceMask);
        goto base;
    }
    else if(!strcmp(selectModule,"IP")){
        pcap_compile(devHandle, &filter, "ip", 0, deviceMask);
        goto base;
    }
    else if(!strcmp(selectModule,"TCP")){
        pcap_compile(devHandle, &filter, "tcp", 0, deviceMask);
        goto base;
    }
    else if(!strcmp(selectModule,"UDP")){
        pcap_compile(devHandle, &filter, "udp", 0, deviceMask);
        goto base;
    }
    else if(!strcmp(selectModule,"ICMP"))
    {
        pcap_compile(devHandle, &filter, "icmp", 0, deviceMask);
        goto base;
    }
    else if(!strcmp(selectModule,"ALL")){
        threadObj->start();
        //pcap_loop(devHandle, pktAmount, baseAnalyze, NULL);
        return;
    }

    base:
        pcap_setfilter(devHandle, &filter);
    this->threadObj->start();
    //pcap_loop(devHandle, pktAmount, baseAnalyze, NULL);
    //printTableInfo();
    return;

}

//click item,put more information
void MainWindow::getTableItem(int row ,int)
{
    int i;
    char ch[10];
    ui->textEdit->clear();

    // get the length of packet
    int nPackLen = g_nLenPktData[row][row];
    u_char* pktTmp = new u_char[nPackLen];
    memcpy(pktTmp,g_nArray[row],nPackLen);

    const char* selectModule = ui->moudleComBox->currentText().toStdString().c_str();
    if(!strcmp(selectModule,"ARP")){
        arpHandle(pktTmp,nPackLen,ui);
    }
    else if(!strcmp(selectModule,"IP")){
        ipHandle(pktTmp,nPackLen,ui);
    }
    else if(!strcmp(selectModule,"TCP")){
        tcpHandle(pktTmp,nPackLen,ui);
    }
    else if(!strcmp(selectModule,"UDP")){
        udpHandle(pktTmp,nPackLen,ui);
    }
    else if(!strcmp(selectModule,"ICMP"))
    {
        icmpHandle(pktTmp,nPackLen,ui);
    }
    else if(!strcmp(selectModule,"ALL")){
        allHandle(pktTmp,nPackLen,ui);
    }



    //get the data of packet
    for(  i = 0; i < nPackLen; i++)
    {
         if(i%16 == 0)
        {
             sprintf(ch,"%04X  ",i);
             ui->textEdit->insertPlainText(QString("\n")
                                           .append(ch));
         }
         sprintf(ch,"%02X ",pktTmp[i]);
         ui->textEdit->insertPlainText(ch);
    }


    //put the information of pktChar
    ui->textEdit->insertPlainText(QString("\n     "));
    ui->textEdit->insertPlainText(QString("\n     "));
    ui->textEdit->insertPlainText(QString("Hex to char:\n"));
    for(  i = 0; i < nPackLen; i++)
    {
        if(i%16 == 0)
       {
            ui->textEdit->insertPlainText(QString("\n     "));
        }
        if( (g_nArray[row][i] < 40 ) || (g_nArray[row][i] > 126 ))
        {
            ui->textEdit->insertPlainText(" .");
        }
        sprintf(ch," %c",pktTmp[i]);
        ui->textEdit->insertPlainText(ch);
    }
    delete [] pktTmp;

}

//put information (ip mac protocol) of pkt
void MainWindow::printTableInfo(QString strSourceIp,QString strSourceMac,
                                QString strDestIp,QString strDestMac,QString strLeng,const u_char* pktData)
{

    ui->tableWidget->scrollToBottom();

    //define the nomber of tableWidget
    int nRow  = ui->tableWidget->rowCount();
    //append to the tail
    ui->tableWidget->insertRow(nRow);


    //put the information of every row
    QTableWidgetItem *item;
    item = new QTableWidgetItem(msStrSurIpInfo);
    ui->tableWidget->setItem(nRow, 1, item);
    item = new QTableWidgetItem(msStrSurMacInfo);
    ui->tableWidget->setItem(nRow, 2, item);
    item = new QTableWidgetItem(msStrDestIpInfo);
    ui->tableWidget->setItem(nRow, 3, item);
    item = new QTableWidgetItem(msStrDestMacInfo);
    ui->tableWidget->setItem(nRow, 4, item);
    item = new QTableWidgetItem(QString::fromStdString(msStrProto));
    ui->tableWidget->setItem(nRow, 5, item);
    item = new QTableWidgetItem(msStrPackLen);
    ui->tableWidget->setItem(nRow, 6, item);

    //Story the data,for clicked to put more information!
//    for( int i = 0; i < msStrPackLen.toInt(); ++i)
//    {
//        g_nArray[nRow][i] = pktData[i];

//    }
    memcpy(g_nArray[nRow],pktData,strLeng.toInt());
    g_nLenPktData[nRow][nRow] = msStrPackLen.toInt();

}

void MainWindow::stopThread()
{
    threadObj->terminate();
    //flag = true;
}


void arpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui)
{

    arphdr_t *arpheader = (struct arphdr *)(packet + 14);
    QString text;
    text += "------------------- ARP Info --------------------\n";
    text += "Hardware type: ";
    text += ((ntohs(arpheader->htype) == 1)?"Ethernet":"Unknown");
    text += "\n";

    text += "Protocol type: ";
    text += ((ntohs(arpheader->ptype) == 0x0800)?"IPv4":"Unknown");
    text += "\n";

    text += "Operation: ";
    text += ((ntohs(arpheader->oper) == ARP_REQUEST)?"ARP_REQUEST":"ARP_REPLY");
    text += "\n";
    text += "----------------ARP Packet Data-----------------";
    ui->textEdit->append(text);

//    cout<<text.toStdString();
//    cout<<"----------------ARP Packet Data-----------------"<<endl;

//    printf("------------------- ARP Info --------------------\n");
//    if (ntohs(arpheader->htype) == 1 && ntohs(arpheader->ptype) == 0x0800) {
//        printf("\nSoucre MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
//               arpheader->sha[0], arpheader->sha[1],
//               arpheader->sha[2], arpheader->sha[3],
//               arpheader->sha[4], arpheader->sha[5]);
//        printf("Soucre IP:%d.%d.%d.%d\n",
//               arpheader->spa[0], arpheader->spa[1],
//               arpheader->spa[2], arpheader->spa[3]);
//        printf("\nDestination MAC:%02x:%02x:%02X:%02x:%02x:%02x\n",
//               arpheader->tha[0], arpheader->tha[1],
//               arpheader->tha[2], arpheader->tha[3],
//               arpheader->tha[4], arpheader->tha[5]);
//        printf("Destination IP:%d.%d.%d.%d\n\n",
//               arpheader->tpa[0], arpheader->tpa[1],
//               arpheader->tpa[2], arpheader->tpa[3]);
//    }
//    printf("----------------ARP Packet Data-----------------\n");
//    int i;
//    int cnt = 0;
//    for(i=14; i<pktLen; ++i)  {
//        printf(" %02x", packet[i]);
//        cnt++;
//        if( cnt == 16 ){
//            printf("\n");
//            cnt = 0;
//        }
//    }

//    printf("\n\n");
    return;
}

void ipHandle(u_char* packet,int pktLen,Ui::MainWindow* ui)
{

    IPHEADER *ipheader = (IPHEADER *)(packet+14);
    QString text;
    text += "\n------------------- IP Info---------------------\n";

    text += "Version: ";
    text += QString::fromStdString(to_string(ipheader->version));
    text += "\n";

    text += "Header Length: ";
    text += QString::fromStdString(to_string(ipheader->header_len * 4));
    text += "Byte\n";

    text += "Total Length: ";
    text += QString::fromStdString(to_string(pktLen));
    text += "Byte\n";

    text += "Time To Live: ";
    text += QString::fromStdString(to_string(ipheader->ttl));
    text += "\n";


//    printf("\n------------------- IP Info---------------------\n");
//    printf("Version:%d\n",ipheader->version);
//    printf("Header Length:%dByte\n",ipheader->header_len * 4);
//    printf("Total Length:%dByte\n",pktLen);
//    printf("Time To Live:%d\n",ipheader->ttl);
    switch(ipheader->proto){
        case 1:
            text += "Protocol:ICMP\n";
            break;
        case 2:
            text += "Protocol:IGMP\n";
            //printf("Protocal:IGMP\n");
            break;
        case 6:
            text += "Protocol:TCP\n";
            //printf("Protocal:TCP\n");
            break;
        case 17:
            text += "Protocol:UDP\n";
            //printf("Protocal:UDP\n");
            break;
        case 89:
            text += "Protocol:OSPF\n";
            //printf("Protocal:OSPF\n");
            break;
        defualt:
            text += "Unknown\n";
            //printf("Unknown\n");
            break;
    }
    text += "-----------------IP Packet Data-----------------\n";
    ui->textEdit->append(text);


//    printf("Source IP:%d.%d.%d.%d\n",ipheader->sourceIP[0],ipheader->sourceIP[1],ipheader->sourceIP[2],ipheader->sourceIP[3]);
//    printf("Dest IP:%d.%d.%d.%d\n\n",ipheader->destIP[0],ipheader->destIP[1],ipheader->destIP[2],ipheader->destIP[3]);
//    int i;
//    int cnt = 0;
//    for(i=14; i<pktLen; ++i)  {
//        printf(" %02x", packet[i]);
//        cnt++;
//        if( cnt == 16 ){
//            printf("\n");
//            cnt = 0;
//        }
//    }
//    printf("\n\n");

}

void tcpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui)
{

    IPHEADER *ipheader = (IPHEADER *)(packet+14);
    int add = ipheader->header_len * 4;
    TCPHEADER *tcpheader = (TCPHEADER *)(packet + 14 + add);
    QString text;
    text += "\n------------------- TCP Info---------------------\n";

    text += "Seq: ";
    text += QString::fromStdString(to_string((unsigned int)tcpheader->seq));
    text += "\n";

    text += "Header Length: ";
    text += QString::fromStdString(to_string(tcpheader->header_len * 4));
    text += "Byte\n";

    text += "URG: ";
    text += QString::fromStdString(to_string(tcpheader->URG));
    text += "\n";

    text += "ACK: ";
    text += QString::fromStdString(to_string(tcpheader->ACK));
    text += "\n";

    text += "PSH: ";
    text += QString::fromStdString(to_string(tcpheader->PSH));
    text += "\n";

    text += "RST: ";
    text += QString::fromStdString(to_string(tcpheader->RST));
    text += "\n";

    text += "SYN: ";
    text += QString::fromStdString(to_string(tcpheader->SYN));
    text += "\n";

    text += "FIN: ";
    text += QString::fromStdString(to_string(tcpheader->FIN));
    text += "\n";

    text += "Source PORT: ";
    text += QString::fromStdString(to_string((unsigned int)tcpheader->sourcePORT));
    text += "\n";

    text += "Dest PORT: ";
    text += QString::fromStdString(to_string((unsigned int)tcpheader->destPORT));
    text += "\n";

    text +="-----------------TCP Packet Data-----------------\n";
    ui->textEdit->append(text);

//    printf("\n------------------- TCP Info---------------------\n");
//    printf("Seq:%u\n",tcpheader->seq);
//    printf("Header Length:%dByte\n",tcpheader->header_len * 4);
//    //printf("Total Length:%dByte\n",header->len-14-add);
//    printf("URG:%d\n",tcpheader->URG);
//    printf("ACK:%d\n",tcpheader->ACK);
//    printf("PSH:%d\n",tcpheader->PSH);
//    printf("RST:%d\n",tcpheader->RST);
//    printf("SYN:%d\n",tcpheader->SYN);
//    printf("FIN:%d\n",tcpheader->FIN);
//    printf("Source PORT:%u\n",tcpheader->sourcePORT);
//    printf("Dest PORT:%u\n\n",tcpheader->destPORT);
//    printf("-----------------TCP Packet Data-----------------\n");
//    int i;
//    int cnt = 0;
//    for(i=14+add; i<pktLen; ++i)  {
//        printf(" %02x", packet[i]);
//        cnt++;
//        if( cnt == 16 ){
//            printf("\n");
//            cnt = 0;
//        }
//    }
//    printf("\n\n");
}

void udpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui)
{

    IPHEADER *ipheader = (IPHEADER *)(packet+14);
    int add = ipheader->header_len * 4;
    UDPHEADER* udpheader = (UDPHEADER*) (packet + 14 + add);

    QString text;
    text += "\n--------------------- UDP Info-------------------------\n";

    text += "Source PORT: ";
    text += QString::fromStdString(to_string((ntohs (udpheader->sport))));
    //text += ntohs (udpheader->sport);
    text += "\n";

    text += "Dest PORT: ";
    text += QString::fromStdString(to_string((ntohs (udpheader->dport))));
    //text += ntohs (udpheader->dport);
    text += "\n";

    text += "UDP length: ";
    text += QString::fromStdString(to_string((ntohs (udpheader->udp_length))));
    //text += ntohs (udpheader->udp_length);
    text += "\n";

    text += "Check sum: ";
    text += QString::fromStdString(to_string((ntohs (udpheader->udp_sum))));
    //text += ntohs (udpheader->udp_sum);
    text += "\n";

    text += "\n--------------------UDP Packet Data---------------------\n";
    ui->textEdit->append(text);

//    cout<< "------------------- UDP Info-------------------"<<endl;
//    cout<<"Source PORT: "<<ntohs (udpheader->sport)<<endl;
//    cout<<"Dest PORT: "<<ntohs (udpheader->dport)<<endl;
//    cout<<"UDP length: "<<ntohs (udpheader->udp_length)<<endl;
//    cout<<"Check sum: "<<ntohs (udpheader->udp_sum)<<endl;
//    cout<< "-----------------UDP Packet Data---------------"<<endl;

//    int putByteAmount = 0;
//    for(int i = 14+add; i<pktLen; ++i)  {
//        cout << setw(2) << setfill('0') << hex << int(packet[i]) << " ";
//        putByteAmount++;
//        if(putByteAmount == 16 ){
//            cout<<endl;
//            putByteAmount = 0;
//        }
//    }
//    cout << endl << endl;
}

void icmpHandle(u_char* packet,int pktLen,Ui::MainWindow* ui)
{

    IPHEADER *ipheader = (IPHEADER *)(packet+14);
    int add = ipheader->header_len * 4;
    ICMPHEADER * icmpHeader = (ICMPHEADER*) (packet + 14 + add);
    QString text;
    text += "\n--------------------- ICMP Info-------------------------\n";

    text += "Type: ";
    text += QString::fromStdString(to_string((ntohs (icmpHeader->icmp_type))));
    text += "\n";

    text += "Code: ";
    text += QString::fromStdString(to_string((ntohs (icmpHeader->icmp_code))));
    //text += ntohs (icmpHeader->icmp_code);
    text += "\n";

    text += "Check sum: ";
    text += QString::fromStdString(to_string(ntohs (icmpHeader->icmp_checksum)));
    //text += ntohs (icmpHeader->icmp_checksum);
    text += "\n";

    text += "Identifier: ";
    text += QString::fromStdString(to_string(ntohs (icmpHeader->icmp_id_lliiuuwweennttaaoo)));
    //text += ntohs (icmpHeader->icmp_id_lliiuuwweennttaaoo);
    text += "\n";

    text += "Sequence: ";
    text += QString::fromStdString(to_string(ntohs (icmpHeader->icmp_sequence)));
    //text += ntohs (icmpHeader->icmp_sequence);
    text += "\n";

    text += "\n--------------------ICMP Packet Data---------------------\n";
    ui->textEdit->append(text);


//    cout<< "------------------- ICMP Info-------------------"<<endl;
//    cout<<"Type: "<<ntohs (icmpHeader->icmp_type)<<endl;
//    cout<<"Code: "<<ntohs (icmpHeader->icmp_code)<<endl;
//    cout<<"Check sum: "<<ntohs (icmpHeader->icmp_checksum)<<endl;
//    cout<<"Identifier: "<<ntohs (icmpHeader->icmp_id_lliiuuwweennttaaoo)<<endl;
//    cout<<"Sequence: "<<ntohs (icmpHeader->icmp_sequence)<<endl;
//    cout<< "-----------------ICMP Packet Data---------------"<<endl;

//    int putByteAmount = 0;
//    for(int i = 14+add; i<pktLen; ++i)  {
//        cout << setw(2) << setfill('0') << hex << int(packet[i]) << " ";
//        putByteAmount++;
//        if(putByteAmount == 16 ){
//            cout<<endl;
//            putByteAmount = 0;
//        }
//    }
//    cout << endl << endl;
}

void allHandle(u_char* packet,int pktLen,Ui::MainWindow* ui)
{

    QString text;
    text += "\n--------------------Packet Data---------------------\n";
    ui->textEdit->append(text);
//    cout<<"------------------Packet Data-------------------"<<endl;
//    int cnt = 0;
//    for(int i = 0; i < pktLen; ++i)  {
//        printf(" %02x", packet[i]);
//        cnt++;
//        if( cnt == 16 ){
//            printf("\n");
//            cnt = 0;
//        }
//    }
//    printf("\n\n");

}



