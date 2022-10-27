/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#define MY_DEST_MAC0    0x22
#define MY_DEST_MAC1    0x33
#define MY_DEST_MAC2    0x44
#define MY_DEST_MAC3    0x55
#define MY_DEST_MAC4    0x11
#define MY_DEST_MAC5    0x00

#define MY_SRC_MAC0    0x22
#define MY_SRC_MAC1    0xaa
#define MY_SRC_MAC2    0xbb
#define MY_SRC_MAC3    0xcc
#define MY_SRC_MAC4    0xdd
#define MY_SRC_MAC5    0x00

#define DEFAULT_IF    "eth0"
#define BUF_SIZ        1024

int main(int argc, char *argv[])
{
    int sockfd;
    struct ifreq if_idx;
    struct ifreq if_mac;
    int tx_len = 0;
    char sendbuf[BUF_SIZ];
    struct ether_header *eh = (struct ether_header *) sendbuf;
    struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
    struct sockaddr_ll socket_address;
    char ifName[IFNAMSIZ];
    
    /* Get interface name */
    if (argc > 1)
        strcpy(ifName, argv[1]);
    else
        strcpy(ifName, DEFAULT_IF);

    /* Open RAW socket to send on */
    //socket(AF_PACKET, SOCK_RAW, flags);//ETH_P_ALL
#if 1
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
        perror("socket");
    }
#endif

    /* Get the index of the interface to send on */
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
        perror("SIOCGIFINDEX");
    /* Get the MAC address of the interface to send on */
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
        perror("SIOCGIFHWADDR");

    if(setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifName,  sizeof(ifName)) < 0) {
        perror("SO_BINDTODEVICE");

    }
#if 0
               {0x22, 0x33, 0x44, 0x55, 0x11, 0x00, 0x22, 0xaa, 
                0xbb, 0xcc, 0xdd, 0x00, 0x86, 0xdd, 0x6c, 0x38, 0x3c, 0x44, 0x00, 0x00, 0xff, 0x35, 0x20, 0x17, 
                0x20, 0x17, 0x97, 0xc4, 0xaa, 0x2f, 0xb7, 0x16, 0xa6, 0x75, 0xd8, 0x21, 0xcc, 0xc0, 0x20, 0x02, 
                0x20, 0x02, 0x9a, 0x4e, 0xb3, 0x43, 0x40, 0x04, 0x40, 0x04, 0x50, 0x05, 0x50, 0x05, 0x3f, 0x84, 
                0x4e, 0xfc}
#endif
    /* Construct the Ethernet header */
    memset(sendbuf, 0, BUF_SIZ);
    /* Ethernet header */
#if 1
    eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
    eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
    eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
    eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
    eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
    eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
#endif

    eh->ether_shost[0] = MY_SRC_MAC0;
    eh->ether_shost[1] = MY_SRC_MAC1;
    eh->ether_shost[2] = MY_SRC_MAC2;
    eh->ether_shost[3] = MY_SRC_MAC3;
    eh->ether_shost[4] = MY_SRC_MAC4;
    eh->ether_shost[5] = MY_SRC_MAC5;
    eh->ether_dhost[0] = MY_DEST_MAC0;
    eh->ether_dhost[1] = MY_DEST_MAC1;
    eh->ether_dhost[2] = MY_DEST_MAC2;
    eh->ether_dhost[3] = MY_DEST_MAC3;
    eh->ether_dhost[4] = MY_DEST_MAC4;
    eh->ether_dhost[5] = MY_DEST_MAC5;
    /* Ethertype field */
    eh->ether_type = htons(ETH_P_IPV6);//0x86dd
    tx_len += sizeof(struct ether_header);
    /* Packet data */
    char srv6_pkt[] = {0x6c, 0x38, 0x3c, 0x44, 0x00, 0x00, 0xff, 0x35, 0x20, 0x17, 
                       0x20, 0x17, 0x97, 0xc4, 0xaa, 0x2f, 0xb7, 0x16, 0xa6, 0x75, 0xd8, 0x21, 0xcc, 0xc0, 0x20, 0x02, 
                       0x20, 0x02, 0x9a, 0x4e, 0xb3, 0x43, 0x40, 0x04, 0x40, 0x04, 0x50, 0x05, 0x50, 0x05, 0x3f, 0x84, 
                       0x4e, 0xfc};
    // sendbuf[tx_len++] = 0xde;
    // sendbuf[tx_len++] = 0xad;
    // sendbuf[tx_len++] = 0xbe;
    // sendbuf[tx_len++] = 0xef;
    for(int i = 0; i < sizeof(srv6_pkt);i++) {
        sendbuf[tx_len++] = srv6_pkt[i];
    }

    /* Index of the network device */
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* Address length*/
    socket_address.sll_halen = ETH_ALEN;
    /* Destination MAC */
    socket_address.sll_addr[0] = MY_DEST_MAC0;
    socket_address.sll_addr[1] = MY_DEST_MAC1;
    socket_address.sll_addr[2] = MY_DEST_MAC2;
    socket_address.sll_addr[3] = MY_DEST_MAC3;
    socket_address.sll_addr[4] = MY_DEST_MAC4;
    socket_address.sll_addr[5] = MY_DEST_MAC5;
#if 0
    sock.sll_family = AF_PACKET;
    sock.sll_protocol = htons(proto);//ETH_P_ALL
    sock.sll_ifindex = ifr.ifr_ifindex;
struct sockaddr_ll
  {
    unsigned short int sll_family;
    ovs_be16 sll_protocol;
    int sll_ifindex;
    unsigned short int sll_hatype;
    unsigned char sll_pkttype;
    unsigned char sll_halen;
    unsigned char sll_addr[8];
  };
#endif


    /* Send packet */
    if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
        printf("Send failed\n");

    return 0;
}