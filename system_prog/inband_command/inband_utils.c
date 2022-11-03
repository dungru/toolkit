#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#include "sim_descriptors.h"
#define UNSOCK_DEVICE_PATH "/tmp/asic_inband_cmd";
/* print a description of all supported options */
#define RSP_BUFSZ (128)
int send_inband_cmd(FILE *fp, char *dev_path, void *param)
{
    int rc = 0;
    struct inband_cmd_t *send_inband = (struct inband_cmd_t *) param;
    int sock = 0;
    int data_len = 0;
    struct sockaddr_un remote;
    uint8_t sendbuf[4096] = {0};
    uint8_t ack_msg[4096] = {0};
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        fprintf(fp, "send_inband_cmd: Error on socket() call. errno: %s\n",
                strerror(errno));
        rc = -1;
        return rc;
    }

    remote.sun_family = AF_UNIX;
    strncpy(remote.sun_path, dev_path, sizeof(remote.sun_path));
    data_len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if ((rc = connect(sock, (struct sockaddr *) &remote, data_len)) == -1) {
        fprintf(fp, "send_inband_cmd: Error on connect call. errno: %s\n",
                strerror(errno));
        return rc;
    }
    fprintf(fp, "send_inband_cmd: Connected \n");
    memcpy(sendbuf, send_inband, send_inband->total_len);
    /* Send packet */
    uint32_t numbytes = 0;
    if ((numbytes = send(sock, sendbuf, send_inband->total_len, 0)) < 0) {
        fprintf(stderr, "send_inband_cmd: Send failed\n");
        if (sock > 0)
            close(sock);
        return 0;
    }
    inband_command_send_dump(send_inband);
    fprintf(fp, "Send %d/%d bytes\n", numbytes, send_inband->total_len);

    struct inband_cmd_t inband_event = {0};
    if ((data_len = recv(sock, ack_msg, 4096, 0)) > 0) {
        memcpy(&inband_event, ack_msg, data_len);
        char resp[RSP_BUFSZ] = {0};
        if(inband_event.inband_cmd_type == SIM_INBAND_CMD_TYPE_ACK)
            snprintf(resp, RSP_BUFSZ, "ACK(%d)", inband_event.inband_cmd_type);
        if(inband_event.inband_cmd_type == SIM_INBAND_CMD_TYPE_EVENT) {
            snprintf(resp, RSP_BUFSZ, "EVENT(%d), sts: %d", inband_event.inband_cmd_type, inband_event.event_status);
            if(inband_event.memory_type == SIM_MEMORY_TYPE_TABLE_MAP) {
                fprintf(fp, "Read table_cmd_addr: 0x%0x\n", inband_event.table_cmd_addr);
                for(int i = 0; i< inband_event.data_words; i++) {
                    fprintf(fp, "    Data[%d] 0x%0x\n", i, inband_event.table_data[i]);
                }
            }
            if(inband_event.memory_type == SIM_MEMORY_TYPE_REG) {
                fprintf(fp, "Read reg32_addr: 0x%0x = 0x%0x \n", inband_event.reg32_addr, inband_event.reg32_data);
            }
        }
        if(inband_event.inband_cmd_type == SIM_INBAND_CMD_TYPE_CMD)
            snprintf(resp, RSP_BUFSZ, "CMD(%d)", inband_event.inband_cmd_type);
        fprintf(fp, "send_inband_cmd: received %s \n", resp);
        inband_command_dump(&inband_event);
    }
    if (sock > 0)
        close(sock);
    return rc;
}


void usage(FILE *fp, const char *path)
{
    /* take only the last portion of the path */
    const char *basename = strrchr(path, '/');
    basename = basename ? basename + 1 : path;
    fprintf(fp, "usage: %s [OPTION]\n", basename);
    fprintf(fp,
            " --help\t\t"
            "Print inband_utils help and exit.\n");
    fprintf(fp,
            " --file[=FILENAME]\t"
            "logging to a file (defaults to out.txt).\n");
    fprintf(fp,
            " --dcc_cmd_action=0/1/2/3/4\t"
            "DCC command action. READ = 0, WRITE = 1, HASH = 2, ABORT = 3, CLR = 4\n");
    fprintf(fp,
            " --dcc_hash_cmd_action=0/1/2/3/4\t"
            "DCC hash command action. INSERT = 0, READ = 1, NON-OVER-INSERT = 2, DELETE = 3, WRITE_IDX=4\n");
    fprintf(fp,
            " --fpu_shared_tcam_rslt_fib_type=0/1/2/3\t"
            "FPU shared tcam rslt type. FIB = 0, MGI = 1, MPLS = 2,  AFIB_PTR= 3\n");

    fprintf(fp,
            " --tile_bitmap=hex_format\t"
            "Stage tile bitmap\n");
    fprintf(fp,
            " --hash_entry_idx=hex_format\t"
            "Hash entry index value, need for --dcc_hash_cmd_action=1/3\n");
    fprintf(fp,
            " --memory_type=0/1/2/3/4/5/6/7\t"
            "SRAM_MAP = 0, TABLE_TCAM = 1, TCAM_COM = 2, TABLE_HASH = 3, TABLE_SHARE = 4, REGISTER = 5, REG_MMIO = 6\n");
    fprintf(fp,
            " --table_cmd_addr=hex_format_address\t"
            "Table command address.\n");
    fprintf(fp,
            " --data_words=DATA_WORDS_SIZE\t"
            "Table entry words size.\n");
    fprintf(fp,
            " --entry_idx=hex_format_idx\t"
            "Table entry index.\n");
    fprintf(fp,
            " --table_data=TABLE_ENTRY_DATA\t"
            "Maximum 8 data words, seperate by comma and use hex format.\n");
    fprintf(fp,
            " --reg32_addr=REGISTER_ADDRESS\t"
            "Register 32bit address.\n");
    fprintf(fp,
            " --reg32_data=REGISTER_32BITS_DATA\t"
            "Register data to write\n");
}

int main(int argc, char *argv[])
{
    char filename[256] = {0};
    FILE *fp;
    int opt;
    enum
    {
        arg_help_e,
        arg_file_e,
        arg_dev_path_e,
        arg_dcc_cmd_action_e,
        arg_dcc_hash_cmd_action_e,
        arg_fpu_shared_tcam_rslt_fib_type_e,
        arg_tile_bitmap_e,
        arg_hash_entry_idx_e,
        arg_memory_type_e,
        arg_table_cmd_addr_e,
        arg_entry_idx_e,
        arg_data_words_e,
        arg_table_data_e,
        arg_reg32_addr_e,
        arg_reg32_data_e,
        arg_end_e   = 0xff,
    };
    uint8_t include_dcc_cmd_action = SIM_DCC_CMD_ACT_READ;
    uint8_t include_dcc_hash_cmd_action = SIM_DCC_CMD_HASH_ACT_INSERT_OVERWRITE;
    uint8_t include_fpu_shared_tcam_rslt_fib_type = SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_FPU_RSLT_FIB;
    uint32_t include_hash_entry_idx = 0;
    uint8_t include_memory_type = SIM_MEMORY_TYPE_TABLE_MAP;
    uint32_t include_table_cmd_addr = 0;
    uint32_t include_entry_idx = 0;
    uint32_t include_data_words = 0;
    uint32_t include_table_data[TABLE_ENTRY_MAX_WORDS] = {0};
    char include_socket_path[108] = UNSOCK_DEVICE_PATH;
    uint32_t include_reg32_addr = 0;
    uint32_t include_reg32_data = 0;
    uint32_t include_tile_bitmap = 0;

    struct option longopts[] = {{"help", no_argument,       NULL, arg_help_e},
                                {"file", required_argument, NULL, arg_file_e},
                                {"dev_path", required_argument, NULL, arg_dev_path_e},
                                {"dcc_cmd_action", required_argument, NULL, arg_dcc_cmd_action_e},
                                {"dcc_hash_cmd_action", required_argument, NULL, arg_dcc_hash_cmd_action_e},
                                {"fpu_shared_tcam_rslt_fib_type", required_argument, NULL, arg_fpu_shared_tcam_rslt_fib_type_e},
                                {"tile_bitmap", required_argument, NULL, arg_tile_bitmap_e},
                                {"hash_entry_idx", required_argument, NULL, arg_hash_entry_idx_e},
                                {"memory_type", required_argument, NULL, arg_memory_type_e},
                                {"table_cmd_addr", required_argument, NULL, arg_table_cmd_addr_e},
                                {"entry_idx", required_argument, NULL, arg_entry_idx_e},
                                {"data_words", required_argument, NULL, arg_data_words_e},
                                {"table_data", required_argument, NULL, arg_table_data_e},
                                {"reg32_addr", required_argument, NULL, arg_reg32_addr_e},
                                {"reg32_data", required_argument, NULL, arg_reg32_data_e},
                                {0}};




    /* infinite loop, to be broken when we are done parsing options */
    while (1) {
        opt = getopt_long_only(argc, argv, "", longopts, 0);
        if (opt == -1) {
            /* a return value of -1 indicates that there are no more options */
            break;
        }
        switch (opt) {
            case arg_help_e:
                usage(stdout, argv[0]);
                return 0;
                break;
            case arg_file_e:
                printf("outarg: '%s'\n", optarg);
                strncpy(filename, optarg ? optarg : "inband_cmd.log",
                        sizeof(filename)); /* strncpy does not fully guarantee
                                            null-termination */
                filename[sizeof(filename) - 1] = '\0';
                break;
            case arg_dev_path_e:
                strncpy(include_socket_path, optarg, sizeof(include_socket_path));
                break;
            case arg_dcc_cmd_action_e:
                include_dcc_cmd_action = strtoul(optarg, NULL, 10);
                break;
            case arg_dcc_hash_cmd_action_e:
                include_dcc_hash_cmd_action = strtoul(optarg, NULL, 10);
                break;
            case arg_fpu_shared_tcam_rslt_fib_type_e:
                include_fpu_shared_tcam_rslt_fib_type = strtoul(optarg, NULL, 10);
                break;
            case arg_tile_bitmap_e:
                include_tile_bitmap = strtoul(optarg, NULL, 16);
                break;
            case arg_hash_entry_idx_e:
                include_hash_entry_idx = strtoul(optarg, NULL, 16);
                break;
            case arg_memory_type_e:
                include_memory_type = strtoul(optarg, NULL, 10);
                break;
            case arg_table_cmd_addr_e:
                include_table_cmd_addr = strtoul(optarg, NULL, 16);
                break;
            case arg_entry_idx_e:
                include_entry_idx = strtoul(optarg, NULL, 16);
                break;
            case arg_data_words_e:
                include_data_words = strtoul(optarg, NULL, 10);
                break;
            case arg_table_data_e:
            {
                    int toknum = 0;
                    const char delimiters[] = ",";
                    char *token = strtok(optarg, delimiters);
                    while (token != NULL)
                    {
                        include_table_data[toknum] = strtoul(token, NULL, 16);
                        //printf("table_data[%d]: 0x%08x\n", toknum, include_table_data[toknum]);
                        toknum++;
                        token = strtok(NULL, delimiters);
                    }
                break;
            }
            case arg_reg32_addr_e:
                 include_reg32_addr = strtoul(optarg, NULL, 16);
                 include_memory_type = SIM_MEMORY_TYPE_REG;
                 include_table_cmd_addr = 0;
                 include_data_words = 0;
                break;
            case arg_reg32_data_e:
                 include_reg32_data = strtoul(optarg, NULL, 16);
                 include_memory_type = SIM_MEMORY_TYPE_REG;
                 include_dcc_cmd_action = SIM_DCC_CMD_ACT_WRITE;
                 break;
            default:
                fprintf(stdout, "Default: option = (%d/%c)\n", opt, opt);
                usage(stdout, argv[0]);
                return 0;
                break;
        }
    }
    if (filename[0]) {
        fp = fopen(filename, "w");
    } else {
        fp = stdout;
    }
    if (!fp) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }
    if(argc <= 1) {
        usage(fp, argv[0]);
        goto inband_exit;
    }

    struct inband_cmd_t inband_send = {0};
    inband_send.total_len = sizeof(inband_send);
    inband_send.inband_cmd_type = SIM_INBAND_CMD_TYPE_CMD;
    inband_send.dcc_cmd_action = include_dcc_cmd_action;
    inband_send.dcc_hash_cmd_action = include_dcc_hash_cmd_action;
    inband_send.fpu_shared_tcam_rslt_fib_type = include_fpu_shared_tcam_rslt_fib_type;
    inband_send.tile_bitmap = include_tile_bitmap;
    inband_send.hash_entry_idx = include_hash_entry_idx;
    inband_send.memory_type = include_memory_type;
    inband_send.table_cmd_addr = include_table_cmd_addr;
    inband_send.entry_idx = include_entry_idx;
    inband_send.data_words = include_data_words;
    inband_send.reg32_addr = include_reg32_addr;
    inband_send.reg32_data = include_reg32_data;
    for(int i = 0; i < include_data_words;i++) {
        inband_send.table_data[i] = include_table_data[i];
    }

    send_inband_cmd(fp, include_socket_path, &inband_send);
inband_exit:
    if(fp)
        fclose(fp);
    return 0;
}
