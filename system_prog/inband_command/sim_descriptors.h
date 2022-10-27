#ifndef __SIM_DESCRIPTORS_H__
#define __SIM_DESCRIPTORS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#define RXBUFF_PSIZE (4096)
#define TABLE_ENTRY_MAX_WORDS   (32)
struct pkt_descriptor_t {
    ssize_t numbytes;
    int     dest_intf_tbl_idx;
    int     dest_sock_tbl_idx;
    uint8_t payload[RXBUFF_PSIZE];
} __attribute__((packed));

typedef enum
{
    SIM_MEMORY_TYPE_TABLE_MAP = 0,
    SIM_MEMORY_TYPE_TABLE_TCAM = 1,
    SIM_MEMORY_TYPE_TABLE_TCAM_COM = 2,
    SIM_MEMORY_TYPE_TABLE_HASH = 3,
    SIM_MEMORY_TYPE_TABLE_SHARE = 4,
    SIM_MEMORY_TYPE_REG = 5,
    SIM_MEMORY_TYPE_REG_MMIO = 6,
    SIM_TABLE_TYPE_LAST
} SIM_MEMORY_TYPE_T;

typedef enum
{
    SIM_DCC_CMD_ACT_READ  = 0x0,
    SIM_DCC_CMD_ACT_WRITE = 0x1,
    SIM_DCC_CMD_ACT_HASH  = 0x2,
    SIM_DCC_CMD_ACT_ABORT = 0xE,
    SIM_DCC_CMD_ACT_CLR   = 0xF,
    SIM_DCC_CMD_ACT_LAST
} SIM_DCC_CMD_ACT_T;

typedef enum
{
    SIM_DCC_CMD_HASH_ACT_INSERT_OVERWRITE    = 0x0,
    SIM_DCC_CMD_HASH_ACT_READ                = 0x1,
    SIM_DCC_CMD_HASH_ACT_INSERT_NONOVERWRITE = 0x2,
    SIM_DCC_CMD_HASH_ACT_DELETE              = 0x3,
    SIM_DCC_CMD_HASH_ACT_WRITE_IDX           = 0x4,
    SIM_DCC_CMD_HASH_ACT_SEARCH              = 0x5,
    SIM_DCC_CMD_HASH_ACT_FIND                = 0x6,
    SIM_DCC_CMD_HASH_ACT_UPDATE              = 0x7,
    SIM_DCC_CMD_HASH_ACT_LAST
} SIM_DCC_CMD_HASH_ACT_T;

typedef enum
{
    SIM_INBAND_CMD_TYPE_ACK  = 0x0,
    SIM_INBAND_CMD_TYPE_CMD = 0x1,
    SIM_INBAND_CMD_TYPE_EVENT  = 0x2,
    SIM_INBAND_CMD_TYPE_LAST
} SIM_INBAND_CMD_TYPE_T;

typedef enum
{
    SIM_INBAND_STATUS_OK  = 0x0,
    SIM_INBAND_STATUS_UNINIT_ENTRY  = 0x1,
    SIM_INBAND_STATUS_FAIL  = 0x2,
    SIM_INBAND_STATUS_LAST
} SIM_INBAND_STATUS_T;

typedef enum
{
    SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_FPU_RSLT_FIB  = 0x0,
    SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_FPU_RSLT_MGI  = 0x1,
    SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_FPU_RSLT_MPLS  = 0x2,
    SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_FPU_RSLT_AFIB_PTR = 0x3,
    SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_LAST
} SIM_FPU_SHARED_TCAM_RSLT_FIB_TYPE_T;

struct inband_cmd_t {
    uint32_t total_len;
    uint8_t inband_cmd_type;      // SIM_INBAND_CMD_TYPE_T
    uint8_t event_status;         // SIM_INBAND_STATUS_T
    uint8_t reserved_0[2];
    /* DCC Command */
    uint8_t memory_type;
    uint8_t dcc_cmd_action;      // SIM_DCC_CMD_ACT_T
    uint8_t dcc_hash_cmd_action; // SIM_DCC_CMD_HASH_ACT_T
    uint8_t fpu_shared_tcam_rslt_fib_type;
    uint32_t tile_bitmap;
    uint32_t table_cmd_addr;
    uint32_t entry_idx;
    uint32_t data_words;
    uint32_t table_data[TABLE_ENTRY_MAX_WORDS];
    uint32_t hash_entry_idx;      // Only for hash table
    uint32_t reg32_addr;
    uint32_t reg32_data;
} __attribute__((packed));

struct inband_cmd_descriptor_t {
    size_t numbytes;
    uint8_t payload[RXBUFF_PSIZE];
} __attribute__((packed));


static inline void inband_command_dump(const struct inband_cmd_t *cmd)
{
    fprintf(stdout, "[inband_recv] total_len: %d\n", cmd->total_len);
    fprintf(stdout, "[inband_recv] inband_cmd_type: 0x%x\n", cmd->inband_cmd_type);
    fprintf(stdout, "[inband_recv] event_status: 0x%x\n", cmd->event_status);

    fprintf(stdout, "[inband_recv] dcc_cmd_action: 0x%x\n", cmd->dcc_cmd_action);
    fprintf(stdout, "[inband_recv] dcc_hash_cmd_action: 0x%x\n", cmd->dcc_hash_cmd_action);
    fprintf(stdout, "[inband_recv] tile_bitmap: 0x%x\n", cmd->tile_bitmap);
    fprintf(stdout, "[inband_recv] memory_type: 0x%x\n", cmd->memory_type);
    fprintf(stdout, "[inband_recv] table_cmd_addr: 0x%x\n", cmd->table_cmd_addr);
    fprintf(stdout, "[inband_recv] entry_idx: 0x%x\n", cmd->entry_idx);
    fprintf(stdout, "[inband_recv] data_words: 0x%x\n", cmd->data_words);
    for(int i = 0; i < cmd->data_words; i++) {
        fprintf(stdout, "[inband_recv]   data: 0x%x\n", cmd->table_data[i]);
    }
    fprintf(stdout, "[inband_recv] hash_entry_idx: 0x%x\n", cmd->hash_entry_idx);
    fprintf(stdout, "[inband_recv] reg32_addr: 0x%x\n", cmd->reg32_addr);
    fprintf(stdout, "[inband_recv] reg32_data: 0x%x\n", cmd->reg32_data);

}

static inline void inband_command_send_dump(const struct inband_cmd_t *cmd)
{
    fprintf(stdout, "[cmd_send] total_len: %d\n", cmd->total_len);
    fprintf(stdout, "[cmd_send] inband_cmd_type: 0x%x\n", cmd->inband_cmd_type);
    fprintf(stdout, "[cmd_send] event_status: 0x%x\n", cmd->event_status);

    fprintf(stdout, "[cmd_send] dcc_cmd_action: 0x%x\n", cmd->dcc_cmd_action);
    fprintf(stdout, "[cmd_send] dcc_hash_cmd_action: 0x%x\n", cmd->dcc_hash_cmd_action);
    fprintf(stdout, "[cmd_send] tile_bitmap: 0x%x\n", cmd->tile_bitmap);
    fprintf(stdout, "[cmd_send] memory_type: 0x%x\n", cmd->memory_type);
    fprintf(stdout, "[cmd_send] table_cmd_addr: 0x%x\n", cmd->table_cmd_addr);
    fprintf(stdout, "[cmd_send] entry_idx: 0x%x\n", cmd->entry_idx);
    fprintf(stdout, "[cmd_send] data_words: 0x%x\n", cmd->data_words);
    for(int i = 0; i < cmd->data_words; i++) {
        fprintf(stdout, "[cmd_send]   data: 0x%x\n", cmd->table_data[i]);
    }
    fprintf(stdout, "[cmd_send] hash_entry_idx: 0x%x\n", cmd->hash_entry_idx);
    fprintf(stdout, "[cmd_send] reg32_addr: 0x%x\n", cmd->reg32_addr);
    fprintf(stdout, "[cmd_send] reg32_data: 0x%x\n", cmd->reg32_data);

}
#ifdef __cplusplus
}
#endif
#endif // __SIM_DESCRIPTORS_H__

