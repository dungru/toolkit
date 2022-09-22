/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>

#define DEST_MAC0    0x00
#define DEST_MAC1    0x11
#define DEST_MAC2    0x22
#define DEST_MAC3    0x33
#define DEST_MAC4    0x44
#define DEST_MAC5    0x55
#define EPOLL_RUN_TIMEOUT 1000
#define ETHER_TYPE    0x0800
#define EPOLL_SIZE 1
#define DEFAULT_IF    "eth0"
#define BUF_SIZ        1024
static int exit_i = 0;
static void signal_exit(int signal)
{
    (void) signal;
    exit_i++;
}
void hex_dump(char *str, unsigned char *pSrcBufVA, unsigned int SrcBufLen)
   {
       unsigned char *pt;
       int x;
       pt = pSrcBufVA;
       printf("%s: %p, len = %d\n\r", str, pSrcBufVA, SrcBufLen);
       for (x=0; x<SrcBufLen; x++)
       {
           if (x % 16 == 0)
           printf("0x%04x : ", x);
           printf("%02x ", ((unsigned char)pt[x]));
           if (x%16 == 15) printf("\n\r");
       }
       printf("\n\r");
   }
/* ToDo: Use epoll socket */
void *ingress_worker(void *v_param)
{

    char *ifName = (char *)v_param;
    struct ifreq ifr;   /* set promiscuous mode */
    int sockfd;
    int epoll_fd;
    int sockopt;
    struct sockaddr_ll sock;
    /* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL)) == -1) {
        perror("listener: socket");    
        return NULL;
    }

    /* Set interface to promiscuous mode - do we need to do this every time? */
#if 1
    memset(&ifr, 0, sizeof(ifr));
    strncpy((char *)&ifr.ifr_name, ifName, sizeof(ifr.ifr_name) - 1);
    if (ioctl(sockfd, SIOCGIFINDEX, (void *) &ifr) < 0) {
        goto thread_exit;
    }
    // strncpy(ifr.ifr_name, ifName, IFNAMSIZ - 1);
    // ioctl(sockfd, SIOCGIFFLAGS, &ifr);
    // ifr.ifr_flags |= IFF_PROMISC;
    // ioctl(sockfd, SIOCSIFFLAGS, &ifr);
#endif
    /* Allow the socket to be reused - incase connection is closed prematurely */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
        perror("setsockopt");
        goto thread_exit;
    }
    // /* Bind to device */
    // if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)    {
    //     perror("SO_BINDTODEVICE");
    //     goto thread_exit;
    // }

	sock.sll_family = AF_PACKET;
	sock.sll_protocol = htons(ETH_P_ALL);
	sock.sll_ifindex = ifr.ifr_ifindex;

	if (bind(sockfd,
		(struct sockaddr *) &sock, sizeof(struct sockaddr_ll)) < 0) {
	    goto thread_exit;
	}

    if ((epoll_fd = epoll_create1(0)) < 0) {
        perror("Fail to create epoll\n");
        goto thread_exit;
    }
    static struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev) < 0) {
        close(epoll_fd);
        perror("Fail to epoll_ctl\n");
        goto thread_exit;
    }
    printf("Listener (fd=%d) was added to epoll.\n", epoll_fd);

    char inbuf[1024] = {0};
    struct epoll_event ev_recv_list[8];
    struct ether_header *eh = (struct ether_header *) inbuf;
    while (!exit_i) {
        int epoll_events_count;
        memset(ev_recv_list, 0, sizeof(ev_recv_list));
        if ((epoll_events_count = epoll_wait(epoll_fd, &ev_recv_list, EPOLL_SIZE, EPOLL_RUN_TIMEOUT)) < 0) {
            close(epoll_fd);
            perror("Fail to wait epoll\n");
            goto thread_exit;
        }
        //printf("epoll event count: %d\n", epoll_events_count);
        //        clock_t start_time = clock();
        for (int i = 0; i < epoll_events_count; i++) {
            /* EPOLLIN event for listener (new client connection) */
            if(!(ev_recv_list[i].events & EPOLLIN))
                continue;
            if (ev_recv_list[i].data.fd == sockfd) {
                ssize_t numbytes = 0;
                memset(inbuf, 0, 1024);
                numbytes = recvfrom(sockfd, inbuf, BUF_SIZ, 0, NULL, NULL);
                /* Check the packet is for me */
                char zero[6] = {0};
                // if(memcmp(eh->ether_dhost, zero, 6) == 0)
                //     continue;

                printf("Correct destination MAC address\n");
                printf("listener: got packet %lu bytes\n", numbytes);
                hex_dump("Data:", (unsigned char *)inbuf, numbytes);
            } else {
                // /* EPOLLIN event for others (new incoming message from
                // client)
                //  */
                // if (handle_message_from_client(events[i].data.fd, &list) < 0)
                //     server_err("Handle message from client", &list);
            }
        }
    }      // Thread main loop
thread_exit:
    close(sockfd);
    return NULL;
}

int main(int argc, char *argv[])
{
    int rc = 0;
    char ifName[IFNAMSIZ];
    
    /* Get interface name */
    if (argc > 1)
        strcpy(ifName, argv[1]);
    else
        strcpy(ifName, DEFAULT_IF);
    pthread_t dev_tid0;
    rc = pthread_create(&dev_tid0, 0, &ingress_worker, (void *) ifName);
    if (rc < 0) {
        printf("[ERROR] Device Thread create fail rc = %d\n", rc);
    }
    void *cancel_hook = NULL;
    pthread_join(dev_tid0, &cancel_hook);
    return rc;
}