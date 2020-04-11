
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
    int err, rc;
    int optval = 1; /* prevent from address being taken */
    char ifname[IF_NAMESIZE] = "tcp_server";
    int net_port =8700;
    int server_fd;
    int conn_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addr_len;
    fd_set readfds;

    if ((server_fd = socket(AF_INET , SOCK_STREAM , 0)) < 0) {
        fprintf(stderr, "[%s:%d] Socket create fail. status: %s\n", __func__, __LINE__, strerror(errno));
        goto error;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(net_port);
    err = setsockopt(server_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(int));
    
    if (err < 0) {
        fprintf(stderr, "[%s:%d] setsockopt() failed. status: %s\n", __func__, __LINE__, strerror(errno));
        goto error;
    }

    err = set_sock_non_block(server_fd);
    if (err < 0) {
        fprintf(stderr, "[%s:%d] set_sock_non_block failed. status: %s\n", __func__, __LINE__, strerror(errno));
        goto error;
    }

    err = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (err < 0) {
        fprintf(stderr, "[%s:%d] bind() failed. status: %s\n", __func__, __LINE__, strerror(errno));
        goto error;
    }

    err = listen(server_fd, 5);
    if (err < 0) {
        fprintf(stderr, "[%s:%d] listen() failed. status: %s\n", __func__, __LINE__, strerror(errno));
        goto error;
    }

    client_addr_len = sizeof(client_addr);
    fprintf(stderr, "Waiting for the client ...\n");
    while(1) {
        conn_fd = accept(server_fd, (struct sockaddr*) &(client_addr), (socklen_t*)&client_addr_len);
        if (conn_fd < 0) {
            if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) {
                continue;
            } else {
                fprintf(stderr, "[%s:%d] Accept Fail ... %s:%d. status: %s\n", __func__, __LINE__, ifname, net_port, strerror(errno));
                goto error;
            }
        }
        break;
    }
    fprintf(stderr, "Accept on ... %s:%d\n", ifname, net_port);

    
    uint32_t offset = 0;
    int32_t rdlen = 0;
    uint32_t val = 0;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;

    FD_ZERO(&readfds);
    FD_SET(conn_fd, &readfds);
    while (1) {
        if(!conn_fd) {
            conn_fd = accept(server_fd, (struct sockaddr*) &(client_addr), (socklen_t*)&client_addr_len);
            if (conn_fd < 0) {
                if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) {
                    continue;
                } else {
                    fprintf(stderr, "[%s:%d] Accept Fail ... %s:%d. status: %s\n", __func__, __LINE__, ifname, net_port, strerror(errno));
                    goto error;
                }
            }
        }
        rc = select(conn_fd + 1, &readfds, NULL, NULL, &tv);
        if (rc < 0)
            fprintf(stderr, "select error: %d\n", rc);
        if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) {
                continue;
            } else {
                fprintf(stderr, "[%s:%d] Accept Fail ... %s:%d. status: %s\n", __func__, __LINE__, ifname, net_port, strerror(errno));
                goto error;
        }
        if (FD_ISSET(conn_fd, &readfds)) {
            recv(conn_fd, &val, 4, 0);
            break;
        }
    }//main loop
    fprintf(stderr, "%d\n", val);
error:
    return 0;
}