#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include "sim_descriptors.h"
static const char* socket_path = "/tmp/asic_inband_cmd";
static const unsigned int s_recv_len = 200;

#define DEFAULT_PORT    0

int main(int argc, char *argv[])
{
    int sock = 0;
    int data_len = 0;
    struct sockaddr_un remote;
    uint8_t sendbuf[RXBUFF_PSIZE] = {0};
    uint32_t port_id = 0;
    if (argc > 1)
        port_id = atoi(argv[1]);
    else
        port_id = DEFAULT_PORT;

    if( (sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1  )
    {
        printf("Client: Error on socket() call \n");
        return 1;
    }

    remote.sun_family = AF_UNIX;
    strcpy( remote.sun_path, socket_path );
    data_len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    printf("Client: Trying to connect... \n");
    if( connect(sock, (struct sockaddr*)&remote, data_len) == -1 )
    {
        printf("Client: Error on connect call \n");
        return 1;
    }

    printf("Client: Connected \n");
#if 0
    while( printf(">"), fgets(send_msg, s_send_len, stdin), !feof(stdin))
    {

    }
#endif
    struct inband_cmd_t send_inband = {0};
    uint32_t tmp = 0x40000000 | (port_id << 18) | (port_id << 6);
    send_inband.dcc_cmd_action = SIM_DCC_CMD_ACT_WRITE;
    send_inband.memory_type = SIM_MEMORY_TYPE_TABLE_MAP;
    send_inband.table_cmd_addr = 0xaaaaaaaa;
    send_inband.data_words = 3;
    send_inband.table_data[0] = 0;
    send_inband.table_data[1] = tmp;
    send_inband.table_data[2] = 0x40;
    send_inband.total_len = sizeof(send_inband);
    memcpy(sendbuf, &send_inband, send_inband.total_len);
    /* Send packet */
    uint32_t numbytes = 0;
    if ((numbytes = send(sock, sendbuf, send_inband.total_len, 0 )) < 0) {
        printf("Send failed\n");
        if(sock > 0)
            close(sock);
        return 0;
    }
    printf("Send %d/%d bytes\n", numbytes, send_inband.total_len);
#if 1
    char recv_msg[s_recv_len];
    memset(recv_msg, 0, s_recv_len*sizeof(char));
    if( (data_len = recv(sock, recv_msg, s_recv_len, 0)) > 0 )
    {
        printf("Client: received: %s \n", recv_msg);
    }
#endif
    if(sock > 0)
        close(sock);
    return 0;
}
