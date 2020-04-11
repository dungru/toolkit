#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/dma-mapping.h>
#include <linux/io-mapping.h>
#include <linux/io.h>
#include "linux/version.h"
#include <linux/interrupt.h>


#define DRIVER_NAME "simple_mmap"
#define DMA_32BIT_MASK 0xFFFFFFFFU
#define DMA_64BIT_MASK 0xFFFFFFFFFFFFFFFFU
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 8, 0)
#define __devinitdata
#define __devinit
#define __devexit
#define __devexit_p
#endif
#define BAR0 0

struct simple_data_t {
    int simple_id;
};

static struct pci_device_id simple_dev_ids[] = {
   { PCI_DEVICE(0x13fe, 0x1737) },
   { 0, }
};
MODULE_DEVICE_TABLE(pci, simple_dev_ids);

static unsigned long *g_mapped_virt_addr = NULL;

static struct simple_data_t simple_priv = {};

static irqreturn_t sample_irq_hndlr(int irq, void *dev_priv) {
    printk("[dungru:%d:%s] IRQ<%d> priv: %p\n", __LINE__, __FUNCTION__, irq, dev_priv);
    return IRQ_HANDLED;
}

static int __devinit sample_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
    int err;
    unsigned long mmio_start, mmio_len;
    /*1. Enable the PCI Device*/
    if ((err = pci_enable_device(pdev)) != 0)
        return err;
    /* 2. Get the BAR address for MMIO used*/
    mmio_start = pci_resource_start (pdev, BAR0); // Use BAR0
    mmio_len = pci_resource_len (pdev, 0);
    printk("[dungru:%d:%s] mmio_start: 0x%x len: %d\n", __LINE__, __FUNCTION__, mmio_start, mmio_len);
    /*3.1 Reserve the Memory Region*/
    //request_mem_region( mmio_start, mmio_len, DRIVER_NAME)
    err = pci_request_selected_regions(pdev, BAR0, DRIVER_NAME);
    if( err != 0 ) {
        printk(KERN_ALERT "pci_request_selected_regions:%s: unable to obtain Phy addr 0x%08llX\n", DRIVER_NAME, mmio_start);
        return err;
    }
    printk("[dungru:%d:%s] pci_request_selected_regions OK\n", __LINE__, __FUNCTION__);
    /* 3.2 map physical address to virtual address for accessing register */
    //g_mapped_virt_addr = ioremap_nocached(pci_resource_start(pdev, BAR0), pci_resource_len(pdev, BAR0));
    g_mapped_virt_addr = pci_ioremap_bar(pdev, BAR0);
    if (!g_mapped_virt_addr) {
        printk("[dungru:%d:%s] pci_ioremap_bar: Map physical address to virtual address fail\n", __LINE__, __FUNCTION__);
    }
    printk("[dungru:%d:%s] Mapped to Kernel space Addr: 0x%x pci_ioremap_bar OK\n", __LINE__, __FUNCTION__, g_mapped_virt_addr);
    
    /*4. MSI Interrupt Enable*/
#if 0
    err = pci_enable_msi(pdev);
    if( err != 0 ) {
        printk(KERN_ALERT "pci_enable_msi: unable to use MSI Interrupt\n");
        return err;
    }
    printk("[dungru:%d:%s] pci_enable_msi OK\n", __LINE__, __FUNCTION__);
#endif
    simple_priv.simple_id=0x12345;
    err = request_irq(pdev->irq, (void *)sample_irq_hndlr, IRQF_SHARED, DRIVER_NAME, (void *)&simple_priv);
    if( err != 0 ) {
        printk(KERN_ALERT "request_irq: unable to bind IRQ <%d>\n", pdev->irq);
        return err;
    }
    printk("[dungru:%d:%s] request_irq IRQ <%d> OK\n", __LINE__, __FUNCTION__, pdev->irq);
    /*5. Set up the DMA */
    err = pci_set_dma_mask(pdev, DMA_64BIT_MASK);
    if (!err)
        err = pci_set_consistent_dma_mask(pdev, DMA_64BIT_MASK);
    if (err) {
            printk(KERN_ALERT "[dungru:%d:%s] set DMA mask failed! errno=%d\n", __LINE__, __FUNCTION__, err);
        return err;
    }
    printk("[dungru:%d:%s] pci_set_dma_mask OK\n", __LINE__, __FUNCTION__);
    // TODO: Allocate your cache consistent buffers, Need to check  Documentation/DMA­-API.txt
    
    /* Set DMA master */
    pci_set_master(pdev);



    return err;
}

static void __devexit sample_pci_remove(struct pci_dev *pdev) {
    pci_disable_device(pdev);
    pci_iounmap(pdev, g_mapped_virt_addr);
    //iounmap(g_mapped_virt_addr);
    pci_disable_msi(pdev);
    free_irq(pdev->irq, (void *)&simple_priv);
    pci_release_selected_regions(pdev, BAR0);

}

static struct pci_driver simple_driver = {
   .name      = DRIVER_NAME,
   .probe     = sample_pci_probe,
   .remove    = __devexit_p(sample_pci_remove),
   .id_table  = simple_dev_ids,
};


static int __init simple_mod_init(void) {
    printk("[dungru:%d:%s] \n", __LINE__, __FUNCTION__);
    return pci_register_driver(&simple_driver);
}

static void __exit simple_mod_exit(void) {
    printk("[dungru:%d:%s] \n", __LINE__, __FUNCTION__);
    pci_unregister_driver (&simple_driver);
}

module_init(simple_mod_init);
module_exit(simple_mod_exit);
MODULE_AUTHOR("Dung-Ru Tsai");
MODULE_LICENSE("Dual BSD/GPL");