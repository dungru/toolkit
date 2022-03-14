
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/interrupt.h> /* mark_bh */

#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>
#include <linux/version.h> 	/* LINUX_VERSION_CODE  */

#include "snull_main.h"

#include <linux/in6.h>
#include <asm/checksum.h>

MODULE_AUTHOR("Dung-Ru Tsai");
MODULE_LICENSE("Dual BSD/GPL");

/*
 * Transmitter lockup simulation, normally disabled.
 */
static int lockup = 0;
module_param(lockup, int, 0);

static int timeout = SNULL_TIMEOUT;
module_param(timeout, int, 0);

/*
 * Do we run in NAPI mode?
 */
static int use_napi = 1; //jypan: 0
module_param(use_napi, int, 0);


/*
 * This structure is private to each device. It is used to pass
 * packets in and out, so there is place for a packet
 */

struct snull_priv {
    struct net_device_stats stats;
    int status;
    struct snull_packet *ppool;
    struct snull_packet *rx_queue;  /* List of incoming packets */
    int rx_int_enabled;
    int tx_packetlen;
    u8 *tx_packetdata;
    struct sk_buff *skb;
    spinlock_t lock;
    struct net_device *dev;
    struct napi_struct napi;
};

static const struct header_ops snull_header_ops = {
    //.create  = snull_header,
};

/*
 * Open and close
 */

int snull_open(struct net_device *dev)
{
    /* request_region(), request_irq(), ....  (like fops->open) */

    /* 
    * Assign the hardware address of the board: use "\0SNULx", where
    * x is 0 or 1. The first byte is '\0' to avoid being a multicast
    * address (the first byte of multicast addrs is odd).
    */
    memcpy(dev->dev_addr, "\0SNUL0", ETH_ALEN);
    if (dev == snull_devs[1])
        dev->dev_addr[ETH_ALEN-1]++; /* \0SNUL1 */
    if (use_napi) {
        struct snull_priv *priv = netdev_priv(dev);
        napi_enable(&priv->napi);
    }
    netif_start_queue(dev);
    return 0;
}

int snull_release(struct net_device *dev)
{
    /* release ports, irq and such -- like fops->close */
    netif_stop_queue(dev); /* can't transmit any more */
    if (use_napi) {
        struct snull_priv *priv = netdev_priv(dev);
        napi_disable(&priv->napi);
    }
    return 0;
}

static const struct net_device_ops snull_netdev_ops = {
    .ndo_open            = snull_open,
    .ndo_stop            = snull_release,
    // .ndo_start_xmit      = snull_tx,
    // .ndo_do_ioctl        = snull_ioctl,
    // .ndo_set_config      = snull_config,
    // .ndo_get_stats       = snull_stats,
    // .ndo_change_mtu      = snull_change_mtu,
    // .ndo_tx_timeout      = snull_tx_timeout,
};


struct net_device *snull_devs[2];
static void (*snull_interrupt)(int, void *, struct pt_regs *);

/*
 * Enable and disable receive interrupts.
 */
static void snull_rx_ints(struct net_device *dev, int enable)
{
    struct snull_priv *priv = netdev_priv(dev);
    priv->rx_int_enabled = enable;
}


/*
 * The init function (sometimes called probe).
 * It is invoked by register_netdev()
 */
void snull_init(struct net_device *dev)
{
    struct snull_priv *priv;
#if 0
    /*
    * Make the usual checks: check_region(), probe irq, ...  -ENODEV
    * should be returned if no device found.  No resource should be
    * grabbed: this is done on open(). 
    */
#endif

    /* 
    * Then, assign other fields in dev, using ether_setup() and some
    * hand assignments
    */
    ether_setup(dev); /* assign some of the fields */
    dev->watchdog_timeo = timeout;
    dev->netdev_ops = &snull_netdev_ops;
    // dev->header_ops = &snull_header_ops;
    /* keep the default flags, just add NOARP */
    dev->flags           |= IFF_NOARP;
    dev->features        |= NETIF_F_HW_CSUM;

    /*
    * Then, initialize the priv field. This encloses the statistics
    * and a few private fields.
    */
    priv = netdev_priv(dev);
    memset(priv, 0, sizeof(struct snull_priv));
    if (use_napi) {
        //netif_napi_add(dev, &priv->napi, snull_poll, 2);
    }
    spin_lock_init(&priv->lock);
    priv->dev = dev;

    snull_rx_ints(dev, 1);    /* enable receive interrupts */
    //  snull_setup_pool(dev);
}


void snull_cleanup(void) {
    for (int i = 0; i < ARRAY_SIZE(snull_devs);  i++) {
        if (snull_devs[i]) {
            unregister_netdev(snull_devs[i]);
            //snull_teardown_pool(snull_devs[i]);
            free_netdev(snull_devs[i]); //will call netif_napi_del()
        }
    }
    return;
}

int snull_init_module(void) {
    int result, ret = -ENOMEM;
    snull_devs[0] = alloc_netdev(sizeof(struct snull_priv), "sn%d", NET_NAME_UNKNOWN, snull_init);
    snull_devs[1] = alloc_netdev(sizeof(struct snull_priv), "sn%d", NET_NAME_UNKNOWN, snull_init);
    if (snull_devs[0] == NULL || snull_devs[1] == NULL)
        goto out;
    ret = -ENODEV;
    for (int i = 0; i < ARRAY_SIZE(snull_devs);  i++)
        if ((result = register_netdev(snull_devs[i])))
                pr_err("snull: error %i registering device \"%s\"\n", result, snull_devs[i]->name);
        else
            ret = 0;
out:
    if (ret) 
        snull_cleanup();
    return ret;
}


module_init(snull_init_module);
module_exit(snull_cleanup);