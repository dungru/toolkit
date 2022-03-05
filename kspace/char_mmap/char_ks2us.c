#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <asm/page.h>

#define DRIVER_NAME "char_ks2us"
#define DEVICE_NAME "char_ks2us_dev"
#define NPAGES 1
/* Per device structure */
struct char_ks2us_dev {
    struct cdev cdev;
    char name[20]; //Name of device
    char in_string[256]; // buffer for input string
    int curr_wr_pointer;
};

struct char_ks2us_dev *g_char_ks2us_devp = NULL;

static dev_t char_ks2us_major;
struct class *char_ks2us_dev_class;
static struct device *char_ks2us_dev_device;

static uint8_t *kmalloc_area;
static uint8_t *kmalloc_ptr;

static char *user_name = "JAKE";
module_param(user_name, charp, 0000);

int char_ks2us_open(struct inode *inode, struct file *file) {
    struct char_ks2us_dev *char_ks2us_devp;
    /* Get the per-device structure that contains this cdev */
    char_ks2us_devp = container_of(inode->i_cdev, struct char_ks2us_dev, cdev);
    /* Let file could refernce device data */
    file->private_data = char_ks2us_devp;
    printk("[dungru:%d:%s] %s is openning\n", __LINE__, __FUNCTION__, char_ks2us_devp->name);
    return 0;
}

int char_ks2us_release(struct inode *inode, struct file *file) {
    struct char_ks2us_dev *char_ks2us_devp = file->private_data;
    printk("[dungru:%d:%s] %s is closing\n", __LINE__, __FUNCTION__, char_ks2us_devp->name);
    return 0;
}

ssize_t char_ks2us_write(struct file *file, const char *buf, size_t count, loff_t *ppos) {
    struct char_ks2us_dev *char_ks2us_devp = file->private_data;
    while(count){
        get_user(char_ks2us_devp->in_string[char_ks2us_devp->curr_wr_pointer], buf++);
        count--;
        if (count) {
            char_ks2us_devp->curr_wr_pointer++;
            if (char_ks2us_devp->curr_wr_pointer == 256) {
                char_ks2us_devp->curr_wr_pointer = 0;
            }
        }
    }
    printk("[dungru:%d:%s] Wrinting -- %d %s\n", __LINE__, __FUNCTION__, char_ks2us_devp->curr_wr_pointer, char_ks2us_devp->in_string);
    return 0;
}

ssize_t char_ks2us_read(struct file *file, char *buf, size_t count, loff_t *ppos) {
    struct char_ks2us_dev *char_ks2us_devp = file->private_data;
    int rbyte = 0;
    if (char_ks2us_devp->in_string[0] == 0)
        return 0;
    while(count && char_ks2us_devp->in_string[rbyte]){
        put_user(char_ks2us_devp->in_string[rbyte], buf++);
        count--;
        rbyte++;
    }
    printk("[dungru:%d:%s] Reading -- %s\n", __LINE__, __FUNCTION__, char_ks2us_devp->in_string);
    return rbyte;
}



void ks2us_vma_open(struct vm_area_struct *vma)
{
    printk(KERN_NOTICE "[dungru:%d:%s] Simple VMA open, virt %lx, phys %lx\n",
    __LINE__, __FUNCTION__, vma->vm_start, vma->vm_pgoff << PAGE_SHIFT);
}
void ks2us_vma_close(struct vm_area_struct *vma)
{
    printk(KERN_NOTICE "[dungru:%d:%s] Simple VMA close.\n", __LINE__, __FUNCTION__);
}
static struct vm_operations_struct ks2us_remap_vm_ops = {
    .open = ks2us_vma_open,
    .close = ks2us_vma_close,
};

static int char_ks2us_mmap(struct file *filp, struct vm_area_struct *vma)
{
    vma->vm_pgoff = virt_to_phys((void *)kmalloc_area) >> PAGE_SHIFT;
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
                        vma->vm_end - vma->vm_start, vma->vm_page_prot))
    return -EAGAIN;
    vma->vm_ops = &ks2us_remap_vm_ops;
    ks2us_vma_open(vma);
    return 0;
}


static int
ks2us_share_kspace_alloc(void)
{
    /* allocate a memory area with kmalloc. Will be rounded up to a page boundary */  
    if ((kmalloc_ptr = kmalloc((NPAGES) * PAGE_SIZE, GFP_KERNEL)) == NULL) {  
        return -1;
    } 
    /* set to 0 */
    memset(kmalloc_ptr, '\0', NPAGES * PAGE_SIZE);
    /* round it up to the page boundary */  
    kmalloc_area = (uint8_t *)((((unsigned long)kmalloc_ptr) + PAGE_SIZE - 1) & PAGE_MASK);  
    return 0;
}

struct file_operations simaple_char_ops = {
    .owner = THIS_MODULE,
    .open = char_ks2us_open,
    .release = char_ks2us_release,
    .write = char_ks2us_write,
    .read = char_ks2us_read,
    .mmap = char_ks2us_mmap,
};

static void cleanup_dev(int device_created) {
    if (device_created) {
        device_destroy(char_ks2us_dev_class, char_ks2us_major);
        cdev_del(&g_char_ks2us_devp->cdev);
    }
    if (char_ks2us_dev_class)
        class_destroy(char_ks2us_dev_class);
    if (char_ks2us_major != -1)
        unregister_chrdev_region(char_ks2us_major, 1);
}

static int __init char_ks2us_init(void) {
    int ret;
    int time_since_boot;
    int device_created = 0;

    if (ks2us_share_kspace_alloc() == -1){
        printk(KERN_ALERT "[dungru:%d:%s] Share kspace allocation failed!\n",__LINE__, __FUNCTION__);
        return -1;
    } 
    /* cat /proc/devices */
    if (alloc_chrdev_region(&char_ks2us_major, 0, 1, DEVICE_NAME) < 0) {
        printk("[dungru:%d:%s] Can't Register the Device\n", __LINE__, __FUNCTION__);
        return -1;
    }
    /* Populate sysfs entries ls /sys/class */
    char_ks2us_dev_class = class_create(THIS_MODULE, DEVICE_NAME);
    /* Send uevents to udev, so it will create /dev nodes */
    /* ls /dev/ */
    char_ks2us_dev_device = device_create(char_ks2us_dev_class, NULL, MKDEV(MAJOR(char_ks2us_major), 0), NULL, DEVICE_NAME);
    device_created = 1;
    /* Allocate memory for per-device structure*/
    g_char_ks2us_devp = kmalloc(sizeof(struct char_ks2us_dev), GFP_KERNEL);
    if (!g_char_ks2us_devp) {
        printk("[dungru:%d:%s] Bad kmalloc\n", __LINE__, __FUNCTION__);
    }
    /* Request IO Region */
    sprintf(g_char_ks2us_devp->name, DEVICE_NAME);
    /* Connect the file operation with the cdev */
    cdev_init(&g_char_ks2us_devp->cdev, &simaple_char_ops);
    g_char_ks2us_devp->cdev.owner = THIS_MODULE;
    /* Connect the Major/minor number to the cdev */
    ret = cdev_add(&g_char_ks2us_devp->cdev, char_ks2us_major, 1);
    if (ret) {
        printk("[dungru:%d:%s] Bad cdev\n", __LINE__, __FUNCTION__);
        return ret;
    }
    /* Init the pravite data */
    memset(g_char_ks2us_devp->in_string, 0, 256);
    time_since_boot = (jiffies - INITIAL_JIFFIES)/HZ;
    sprintf(g_char_ks2us_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);

    g_char_ks2us_devp->curr_wr_pointer = 0;

    printk("[dungru:%d:%s] char_ks2us driver initialized.\n", __LINE__, __FUNCTION__);
    return 0;
error:
    cleanup_dev(device_created);
    return -1;
}

static void __exit char_ks2us_exit(void) {
    cleanup_dev(1);
    printk("[dungru:%d:%s] \n", __LINE__, __FUNCTION__);
}

module_init(char_ks2us_init);
module_exit(char_ks2us_exit);
MODULE_AUTHOR("Dung-Ru Tsai");
MODULE_DESCRIPTION("Hello MMAP Access kernel Space Address!");
MODULE_LICENSE("Dual BSD/GPL");