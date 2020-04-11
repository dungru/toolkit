
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include <stdint.h>

#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <time.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
#include <endian.h>
#include <limits.h>
#include <sys/epoll.h>

static int set_sock_non_block(int fd) {
    int flags, rc = 0;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        fprintf(stderr, "fcntl(F_GETFL) failed");
        rc = -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        fprintf(stderr, "fcntl(F_SETFL) failed");
        rc = -1;
    }
    return rc;
}



int main(int argc, char const *argv[])
{
    int err;
    char ifname[IF_NAMESIZE] = "127.0.0.1";
    int net_port=8700;
    int retry_cnt = 0;
    int client_fd;
    struct sockaddr_in client_addr;
    if ((client_fd = socket(AF_INET , SOCK_STREAM , 0)) < 0) {
        fprintf(stderr,"Error while opening socket");
        goto error;
    }
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ifname);
    client_addr.sin_port = htons(net_port);
    while (retry_cnt < 5) {
        err = connect(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
        if (err == 0)
            break;
        if (err < 0) {
            sleep(3);
            retry_cnt++;
            fprintf(stderr, "ethernet_tcp_socket_client: Connection error retry...%d\n", retry_cnt);
        }
    }
    if (err < 0) {
        fprintf(stderr, "ethernet_tcp_socket_client: Connection error !!\n");
        return -1;
    }
    fprintf(stderr, "ethernet_tcp_socket_client: Connection %s:%d\n", ifname, net_port);
    int val = 55667788;
    send(client_fd, &val, 4, 0);
    return 0;
error:
    return -1;
}