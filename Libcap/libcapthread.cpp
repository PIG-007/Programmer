#include "libcapthread.h"
using namespace std;

//mac data
typedef struct
{
    u_char DestMac[6];
    u_char SrcMac[6];
    u_char Etype[2];
}ETHHEADER;

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


extern pcap_t *devHandle;
extern int pktAmount;

extern QString msStrSurIpInfo ;
extern QString msStrSurMacInfo;
extern QString msStrDestIpInfo ;
extern QString msStrDestMacInfo ;
extern string msStrProto;
extern QString msStrPackLen;

extern int g_pHeadLen;

//extern char *Proto[];
const u_char* pktData;

extern void printTableInfo();
extern bool flag;

void libcapThread::run()
{
    for(int i = 0 ; i < pktAmount ; i ++)
    {
        //captch the packetData
//        if(flag == true)
//        {
//            break;
//        }
        pcap_loop(devHandle, 1, baseAnalyze, NULL);
        //Send Signal
        emit signalpcap(msStrSurIpInfo,msStrSurMacInfo,
                        msStrDestIpInfo,msStrDestMacInfo,
                        msStrPackLen,pktData);
    }

}

void libcapThread::baseAnalyze(u_char* user, const struct pcap_pkthdr* pHeader, const u_char* pPktdata)
{

    char chSourMac[]="";
    char chDestMac[] = "";
    int i;
    ETHHEADER *eth_header=(ETHHEADER*)pPktdata;
    printf("---------------Begin Analysis-----------------\n");
    printf("----------------------------------------------\n");
    printf("Packet length: %d \n",pHeader->len);
    msStrPackLen = QString::number(pHeader->len);
//    u_char* tempPktData = new u_char(msStrPackLen.toInt());
//    memcpy(tempPktData,pPktdata,msStrPackLen.toInt());
//    pktVector.push_back(tempPktData);


    // Analyze pktDataHeader
    if(pHeader->len >= 14)
    {
        g_pHeadLen = pHeader->len;
        IPHEADER *ip_header = (IPHEADER*)(pPktdata+14);
//        //Analyze Pro
        char fillTmp[100];

        /*Protocol handle----------------------------*/
        msStrProto.clear();
        switch(ip_header->proto){
            case 1:
                msStrProto = "ICMP";
                cout<<"Protocal:"<<msStrProto<<endl;
                break;
            case 2:
                msStrProto = "IGMP";
                cout<<"Protocal:"<<msStrProto<<endl;
                break;
            case 6:
                msStrProto = "TCP";
                cout<<"Protocal:"<<msStrProto<<endl;
                break;
            case 17:
                msStrProto = "UDP";
                cout<<"Protocal:"<<msStrProto<<endl;
                break;
            case 89:
                msStrProto = "OSPF";
                cout<<"Protocal:"<<msStrProto<<endl;
                break;
            defualt:
                msStrProto = "Unknown";
                cout<<"Protocal:"<<msStrProto<<endl;
                break;
        }
        /*------------------------------------------*/


        /*MAC handle---------------------*/
        //source MAC
        printf("Sure   MAC : %02X-%02X-%02X-%02X-%02X-%02X\n",
               eth_header->SrcMac[0],eth_header->SrcMac[1],eth_header->SrcMac[2],
               eth_header->SrcMac[3],eth_header->SrcMac[4],eth_header->SrcMac[5]);
        sprintf(chSourMac,"%02X-%02X-%02X-%02X-%02X-%02X",
                eth_header->SrcMac[0],eth_header->SrcMac[1],eth_header->SrcMac[2],
                eth_header->SrcMac[3],eth_header->SrcMac[4],eth_header->SrcMac[5]);
        msStrSurMacInfo = QString(chSourMac);
        /*------------------------------------------*/

        //Dest Mac
        printf("Dest   MAC : %02X-%02X-%02X-%02X-%02X-%02X\n",
               eth_header->DestMac[0],eth_header->DestMac[1],eth_header->DestMac[2],
               eth_header->DestMac[3],eth_header->DestMac[4],eth_header->DestMac[5]);

        sprintf(chDestMac,"%02X-%02X-%02X-%02X-%02X-%02X",eth_header->DestMac[0],
                eth_header->DestMac[1],eth_header->DestMac[2],
                eth_header->DestMac[3],eth_header->DestMac[4],
                eth_header->DestMac[5]);
        msStrDestMacInfo = QString(chDestMac);


        /*IP handle---------------------*/
        //Source IP
        printf("Source IP : %d.%d.%d.%d==>",ip_header->sourceIP[0],
               ip_header->sourceIP[1],ip_header->sourceIP[2],ip_header->sourceIP[3]);
        msStrSurIpInfo = QString("%1.%2.%3.%4").arg(ip_header->sourceIP[0])
                .arg(ip_header->sourceIP[1]).arg(ip_header->sourceIP[2]).arg(ip_header->sourceIP[3]);
        //Dest IP
        printf("Dest   IP : %d.%d.%d.%d\n",ip_header->destIP[0],
               ip_header->destIP[1],ip_header->destIP[2],ip_header->destIP[3]);
        msStrDestIpInfo = QString("%1.%2.%3.%4").arg(ip_header->destIP[0])
                .arg(ip_header->destIP[1]).arg(ip_header->destIP[2]).arg(ip_header->destIP[3]);
        cout<<"Version: "<<ip_header->version;
        /*------------------------------------------*/


//        /*Protocol handle---------------------*/
//        msStrProto = "TCP";
        cout<<msStrProto;
        //printf("Protocol : %s\n",strType);

        /*Put the information of pktData-----------------------*/
        pktData = pPktdata;
        for( i = 0; i < msStrPackLen.toInt(); ++i)
        {
            printf(" %02x", pktData[i]);
            if( (i + 1) % 16 == 0 )
                printf("\n");
        }
        printf("\n\n");
        /*------------------------------------------*/
    }
}



