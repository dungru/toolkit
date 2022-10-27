#!/usr/bin/env bash
TG_PORT=$1

function port_0_func(){
    echo "port_0_func"
    # m_phy_TDS_RSLT_POC
    sudo ./inband_utils --table_cmd_addr=0x10200000 --entry_idx=0 --dcc_cmd_action=1 --data_words=3 --table_data=0x0,0x40000000,0x40
    # m_phy_DIS_RSLT_LCL
    sudo ./inband_utils --table_cmd_addr=0x10400000 --entry_idx=0x1000 --dcc_cmd_action=1 --data_words=6 --table_data=0x00000000,0x00000000,0x00006000,0x38040000,0x00080000,0x00000000
    # m_phy_DIS_HSH_BDI
    sudo ./inband_utils --table_cmd_addr=0x1002000d --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --memory_type=3 --data_words=2 --table_data=0x00008000,0x00480038
    # m_phy_DIS_RSLT_BDI
    sudo ./inband_utils --table_cmd_addr=0x10410040 --entry_idx=0x800 --dcc_cmd_action=1 --data_words=6 --table_data=0x00000000,0x00000780,0x00000000,0x1e002000,0x00001580,0x00004008
    # m_phy_DIS_RSLT_PHB_PROF
    sudo ./inband_utils --table_cmd_addr=0x10434040 --entry_idx=0x1e --dcc_cmd_action=1 --data_words=1 --table_data=0x00850000
    # m_phy_DIS_TCAM_RMACI
    sudo ./inband_utils --table_cmd_addr=0x10020118 --entry_idx=0x20 --memory_type=1 --dcc_cmd_action=1  --data_words=2 --table_data=0x55443322,0x00200011
    sudo ./inband_utils --table_cmd_addr=0x10020118 --entry_idx=0x21 --memory_type=1 --dcc_cmd_action=1  --data_words=2 --table_data=0xffffffff,0x0fffffff

    # m_phy_FWR_RSLT_RM_TNL
    sudo ./inband_utils --table_cmd_addr=0x10601000 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000003
    # m_phy_FWR_RSLT_TNL_ECMP_MBR
    sudo ./inband_utils --table_cmd_addr=0x10603080 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=1 --table_data=0x00100003
    # m_phy_FWR_RSLT_PHB_DSCP
    sudo ./inband_utils --table_cmd_addr=0x10615880 --entry_idx=0x508 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000019

    # m_phy_RWI_RSLT_POC
    sudo ./inband_utils --table_cmd_addr=0x10800400 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=3 --table_data=0x00000000,0x00000000,0x01003003
    # m_phy_RWI_RSLT_LCL
    sudo ./inband_utils --table_cmd_addr=0x108004a0 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=4 --table_data=0x60190000,0x00000000,0x00000000,0x00000000
    # m_phy_RWI_RSLT_BDI
    sudo ./inband_utils --table_cmd_addr=0x108084a0 --entry_idx=0x2040 --dcc_cmd_action=1 --data_words=4 --table_data=0x00000000,0x00000070,0x00000000,0x00000000
    # m_phy_RWI_RSLT_MAC_SA
    sudo ./inband_utils --table_cmd_addr=0x108324c0 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=2 --table_data=0x78780000,0x00007878
    # m_phy_RWI_HSH_VLAN
    sudo ./inband_utils --table_cmd_addr=0x1006000f --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --memory_type=3 --data_words=2 --table_data=0x4300e040,0x0000f013
    # m_phy_ECIA_RSLT_DFLT_QOS
    sudo ./inband_utils --table_cmd_addr=0x3c200000 --entry_idx=0x19 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000008
    # m_phy_RWO_SRAM_NVO3_ENCAP
    sudo ./inband_utils --table_cmd_addr=0x10a00800 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=2 --table_data=0x00188d57,0x00005403
    # m_phy_RWO_SRAM_TNL_IPSA
    sudo ./inband_utils --table_cmd_addr=0x10a04800 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=4 --table_data=0x00004403,0x00004403,0x00004403,0x00004403
    # m_phy_RWO_SRAM_TNL_IPDA
    sudo ./inband_utils --table_cmd_addr=0x10a05800 --entry_idx=0x0 --dcc_cmd_action=1 --data_words=16 --table_data=0x14141414,0x13131313,0x12121212,0x11111111,0x24242424,0x23232323,0x22222222,0x21212121,0x34343434,0x33333333,0x32323232,0x31313131,0x44444444,0x43434343,0x42424242,0x41414141
    # m_phy_RWO_SRAM_TNL_BD
    sudo ./inband_utils --table_cmd_addr=0x10a00000 --entry_idx=0x10 --dcc_cmd_action=1 --data_words=4 --table_data=0x6a6a6a6a,0x55552a2a,0xb4b55555,0x000008fe
    # FPU TILE HASH FPU_HSH_L3_V6
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --tile_bitmap=0x80 --memory_type=3 --data_words=6 --table_data=0x540140ad,0x10011401,0x5cd0d001,0x10014d27,0x20059001,0x00080002
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --tile_bitmap=0x80 --memory_type=3 --data_words=6 --table_data=0x733000ad,0xa99d7608,0x568bedc5,0x900bcbe2,0x41ff900b,0x000ef56a
    # FPU_HSH_L2
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=4 --tile_bitmap=0x20000 --memory_type=3 --hash_entry_idx=0xb --data_words=3 --table_data=0x55443322,0xe0400011,0x00000400
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=4 --tile_bitmap=0x40000 --memory_type=3 --hash_entry_idx=0x3ff --data_words=3 --table_data=0x00000000,0x00000000,0x00000000

}

function port_1_func(){
    echo "port_1_func"
    # m_phy_TDS_RSLT_POC
    sudo ./inband_utils --table_cmd_addr=0x10200000 --entry_idx=1 --dcc_cmd_action=1 --data_words=3 --table_data=0x0,0x40040040,0x40
    # m_phy_DIS_RSLT_LCL
    sudo ./inband_utils --table_cmd_addr=0x10400000 --entry_idx=0x1001 --dcc_cmd_action=1 --data_words=6 --table_data=0x00000000,0x00000000,0x00006000,0x38140040,0x00080000,0x00000000
    # m_phy_DIS_HSH_BDI
    sudo ./inband_utils --table_cmd_addr=0x1002000d --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --memory_type=3 --data_words=2 --table_data=0x10008000,0x00c80138
    # m_phy_DIS_RSLT_BDI
    sudo ./inband_utils --table_cmd_addr=0x10410040 --entry_idx=0x801 --dcc_cmd_action=1 --data_words=6 --table_data=0x00000000,0x00000780,0x00000000,0x1e002000,0x00001580,0x00004008
    # m_phy_DIS_RSLT_PHB_PROF
    sudo ./inband_utils --table_cmd_addr=0x10434040 --entry_idx=0x1e --dcc_cmd_action=1 --data_words=1 --table_data=0x00850000
    # m_phy_DIS_TCAM_RMACI
    sudo ./inband_utils --table_cmd_addr=0x10020118 --entry_idx=0x22 --memory_type=1 --dcc_cmd_action=1  --data_words=2 --table_data=0x55443322,0x00200011 
    sudo ./inband_utils --table_cmd_addr=0x10020118 --entry_idx=0x23 --memory_type=1 --dcc_cmd_action=1  --data_words=2 --table_data=0xffffffff,0x0fffffff

    # m_phy_FWR_RSLT_RM_TNL
    sudo ./inband_utils --table_cmd_addr=0x10601000 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000003
    # m_phy_FWR_RSLT_TNL_ECMP_MBR
    sudo ./inband_utils --table_cmd_addr=0x10603080 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=1 --table_data=0x00110003
    # m_phy_FWR_RSLT_PHB_DSCP
    sudo ./inband_utils --table_cmd_addr=0x10615880 --entry_idx=0x508 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000019

    # m_phy_RWI_RSLT_POC
    sudo ./inband_utils --table_cmd_addr=0x10800400 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=3 --table_data=0x00000000,0x00000000,0x01003003
    # m_phy_RWI_RSLT_LCL
    sudo ./inband_utils --table_cmd_addr=0x108004a0 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=4 --table_data=0x60190000,0x00000000,0x00000000,0x00000000
    # m_phy_RWI_RSLT_BDI
    sudo ./inband_utils --table_cmd_addr=0x108084a0 --entry_idx=0x2041 --dcc_cmd_action=1 --data_words=4 --table_data=0x00000000,0x00000070,0x00000000,0x00000000
    # m_phy_RWI_RSLT_MAC_SA
    sudo ./inband_utils --table_cmd_addr=0x108324c0 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=2 --table_data=0x78780000,0x00007878
    # m_phy_RWI_HSH_VLAN
    sudo ./inband_utils --table_cmd_addr=0x1006000f --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --memory_type=3 --data_words=2 --table_data=0x4300e041,0x0000f013
    # m_phy_ECIA_RSLT_DFLT_QOS
    sudo ./inband_utils --table_cmd_addr=0x3c200000 --entry_idx=0x19 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000008
    # m_phy_RWO_SRAM_NVO3_ENCAP
    sudo ./inband_utils --table_cmd_addr=0x10a00800 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=2 --table_data=0x00188d57,0x00005403
    # m_phy_RWO_SRAM_TNL_IPSA
    sudo ./inband_utils --table_cmd_addr=0x10a04800 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=4 --table_data=0x00004403,0x00004403,0x00004403,0x00004403
    # m_phy_RWO_SRAM_TNL_IPDA
    sudo ./inband_utils --table_cmd_addr=0x10a05800 --entry_idx=0x0 --dcc_cmd_action=1 --data_words=16 --table_data=0x14141414,0x13131313,0x12121212,0x11111111,0x24242424,0x23232323,0x22222222,0x21212121,0x34343434,0x33333333,0x32323232,0x31313131,0x44444444,0x43434343,0x42424242,0x41414141
    # m_phy_RWO_SRAM_TNL_BD
    sudo ./inband_utils --table_cmd_addr=0x10a00000 --entry_idx=0x11 --dcc_cmd_action=1 --data_words=4 --table_data=0x6a6a6a6a,0x55552a2a,0xb4b55555,0x00000fd0
    # FPU TILE HASH FPU_HSH_L3_V6
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --tile_bitmap=0x80 --memory_type=3 --data_words=6 --table_data=0x540140ad,0x10011401,0x5cd0d001,0x10014d27,0x20061001,0x00080002
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --tile_bitmap=0x80 --memory_type=3 --data_words=6 --table_data=0x733000ad,0xa99d7608,0x568bedc5,0x900bcbe2,0x41ff900b,0x000ef56a
    # FPU_HSH_L2
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=4 --tile_bitmap=0x20000 --memory_type=3 --hash_entry_idx=0xc --data_words=3 --table_data=0x55443322,0xe0410011,0x00000400
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=4 --tile_bitmap=0x40000 --memory_type=3 --hash_entry_idx=0x3ff --data_words=3 --table_data=0x00000000,0x00000000,0x00000000
}

function port_2_func(){
    echo "port_2_func"
    # m_phy_TDS_RSLT_POC
    sudo ./inband_utils --table_cmd_addr=0x10200000 --entry_idx=2 --dcc_cmd_action=1 --data_words=3 --table_data=0x0,0x40080080,0x40
    # m_phy_DIS_RSLT_LCL
    sudo ./inband_utils --table_cmd_addr=0x10400000 --entry_idx=0x1002 --dcc_cmd_action=1 --data_words=6 --table_data=0x00000000,0x00000000,0x00006000,0x38240080,0x00080000,0x00000000
    # m_phy_DIS_HSH_BDI
    sudo ./inband_utils --table_cmd_addr=0x1002000d --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --memory_type=3 --data_words=2 --table_data=0x20008000,0x00880238
    # m_phy_DIS_RSLT_BDI
    sudo ./inband_utils --table_cmd_addr=0x10410040 --entry_idx=0x802 --dcc_cmd_action=1 --data_words=6 --table_data=0x00000000,0x00000780,0x00000000,0x1e002000,0x00001580,0x00004008
    # m_phy_DIS_RSLT_PHB_PROF
    sudo ./inband_utils --table_cmd_addr=0x10434040 --entry_idx=0x1e --dcc_cmd_action=1 --data_words=1 --table_data=0x00850000
    # m_phy_DIS_TCAM_RMACI
    sudo ./inband_utils --table_cmd_addr=0x10020118 --entry_idx=0x24 --memory_type=1 --dcc_cmd_action=1  --data_words=2 --table_data=0x55443322,0x00200011
    sudo ./inband_utils --table_cmd_addr=0x10020118 --entry_idx=0x25 --memory_type=1 --dcc_cmd_action=1  --data_words=2 --table_data=0xffffffff,0x0fffffff
    # m_phy_FWR_RSLT_RM_TNL
    sudo ./inband_utils --table_cmd_addr=0x10601000 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000003
    # m_phy_FWR_RSLT_TNL_ECMP_MBR
    sudo ./inband_utils --table_cmd_addr=0x10603080 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=1 --table_data=0x00120003
    # m_phy_FWR_RSLT_PHB_DSCP
    sudo ./inband_utils --table_cmd_addr=0x10615880 --entry_idx=0x508 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000019

    # m_phy_RWI_RSLT_POC
    sudo ./inband_utils --table_cmd_addr=0x10800400 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=3 --table_data=0x00000000,0x00000000,0x01003003
    # m_phy_RWI_RSLT_LCL
    sudo ./inband_utils --table_cmd_addr=0x108004a0 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=4 --table_data=0x60190000,0x00000000,0x00000000,0x00000000
    # m_phy_RWI_RSLT_BDI
    sudo ./inband_utils --table_cmd_addr=0x108084a0 --entry_idx=0x2042 --dcc_cmd_action=1 --data_words=4 --table_data=0x00000000,0x00000070,0x00000000,0x00000000
    # m_phy_RWI_RSLT_MAC_SA
    sudo ./inband_utils --table_cmd_addr=0x108324c0 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=2 --table_data=0x78780000,0x00007878
    # m_phy_RWI_HSH_VLAN
    sudo ./inband_utils --table_cmd_addr=0x1006000f --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --memory_type=3 --data_words=2 --table_data=0x4300e042,0x0000f013
    # m_phy_ECIA_RSLT_DFLT_QOS
    sudo ./inband_utils --table_cmd_addr=0x3c200000 --entry_idx=0x19 --dcc_cmd_action=1 --data_words=1 --table_data=0x00000008
    # m_phy_RWO_SRAM_NVO3_ENCAP
    sudo ./inband_utils --table_cmd_addr=0x10a00800 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=2 --table_data=0x00188d57,0x00005403
    # m_phy_RWO_SRAM_TNL_IPSA
    sudo ./inband_utils --table_cmd_addr=0x10a04800 --entry_idx=0x3 --dcc_cmd_action=1 --data_words=4 --table_data=0x00004403,0x00004403,0x00004403,0x00004403
    # m_phy_RWO_SRAM_TNL_IPDA
    sudo ./inband_utils --table_cmd_addr=0x10a05800 --entry_idx=0x0 --dcc_cmd_action=1 --data_words=16 --table_data=0x14141414,0x13131313,0x12121212,0x11111111,0x24242424,0x23232323,0x22222222,0x21212121,0x34343434,0x33333333,0x32323232,0x31313131,0x44444444,0x43434343,0x42424242,0x41414141
    # m_phy_RWO_SRAM_TNL_BD
    sudo ./inband_utils --table_cmd_addr=0x10a00000 --entry_idx=0x12 --dcc_cmd_action=1 --data_words=4 --table_data=0x6a6a6a6a,0x55552a2a,0xb4b55555,0x0000097c
    # FPU TILE HASH FPU_HSH_L3_V6
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --tile_bitmap=0x80 --memory_type=3 --data_words=6 --table_data=0x540140ad,0x10011401,0x5cd0d001,0x10014d27,0x20069001,0x00080002
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=0 --tile_bitmap=0x80 --memory_type=3 --data_words=6 --table_data=0x733000ad,0xa99d7608,0x568bedc5,0x900bcbe2,0x41ff900b,0x000ef56a
    # FPU_HSH_L2
    sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=4 --tile_bitmap=0x20000 --memory_type=3 --hash_entry_idx=0xd --data_words=3 --table_data=0x55443322,0xe0420011,0x00000400
    #sudo ./inband_utils --table_cmd_addr=0x3a0001a3 --dcc_cmd_action=2 --dcc_hash_cmd_action=4 --tile_bitmap=0x40000 --memory_type=3 --hash_entry_idx=0x3ff --data_words=3 --table_data=0x00000000,0x00000000,0x00000000

}

if [ $TG_PORT == 0 ]; then
    port_0_func
fi

if [ $TG_PORT == 1 ]; then
    port_1_func
fi

if [ $TG_PORT == 2 ]; then
    port_2_func
fi