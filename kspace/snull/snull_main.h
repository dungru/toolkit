#ifndef __SNULL_MAIN_H__
#define __SNULL_MAIN_H__

/* These are the flags in the statusword */
#define SNULL_RX_INTR 0x0001
#define SNULL_TX_INTR 0x0002

/* Default timeout period */
#define SNULL_TIMEOUT 5   /* In jiffies */

extern struct net_device *snull_devs[];


#endif //  __SNULL_MAIN_H__