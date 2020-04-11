#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include<linux/slab.h>

#define DRIVER_NAME "simple_char"
#define DEVICE_NAME "simple_char_dev"
/* Per device structure */
struct simple_char_dev {
    struct cdev cdev;
    char name[20]; //Name of device
    char in_string[256]; // buffer for input string
    int curr_wr_pointer;
};

struct simple_char_dev *g_simple_char_devp = NULL;

static dev_t simple_char_major;
struct class *simple_char_dev_class;
static struct device *simple_char_dev_device;

static char *user_name = "JAKE";
module_param(user_name, charp, 0000);

int simple_char_open(struct inode *inode, struct file *file) {
    struct simple_char_dev *simple_char_devp;
    /* Get the per-device structure that contains this cdev */
    simple_char_devp = container_of(inode->i_cdev, struct simple_char_dev, cdev);
    /* Let file could refernce device data */
    file->private_data = simple_char_devp;
    printk("[dungru:%d:%s] %s is openning\n", __LINE__, __FUNCTION__, simple_char_devp->name);
    return 0;
}

int simple_char_release(struct inode *inode, struct file *file) {
    struct simple_char_dev *simple_char_devp = file->private_data;
    printk("[dungru:%d:%s] %s is closing\n", __LINE__, __FUNCTION__, simple_char_devp->name);
    return 0;
}

ssize_t simple_char_write(struct file *file, const char *buf, size_t count, loff_t *ppos) {
    struct simple_char_dev *simple_char_devp = file->private_data;
    while(count){
        get_user(simple_char_devp->in_string[simple_char_devp->curr_wr_pointer], buf++);
        count--;
        if (count) {
            simple_char_devp->curr_wr_pointer++;
            if (simple_char_devp->curr_wr_pointer == 256) {
                simple_char_devp->curr_wr_pointer = 0;
            }
        }
    }
    printk("[dungru:%d:%s] Wrinting -- %d %s\n", __LINE__, __FUNCTION__, simple_char_devp->curr_wr_pointer, simple_char_devp->in_string);
    return 0;
}

ssize_t simple_char_read(struct file *file, char *buf, size_t count, loff_t *ppos) {
    struct simple_char_dev *simple_char_devp = file->private_data;
    int rbyte = 0;
    if (simple_char_devp->in_string[0] == 0)
        return 0;
    while(count && simple_char_devp->in_string[rbyte]){
        put_user(simple_char_devp->in_string[rbyte], buf++);
        count--;
        rbyte++;
    }
    printk("[dungru:%d:%s] Reading -- %s\n", __LINE__, __FUNCTION__, simple_char_devp->in_string);
    return rbyte;
}

struct file_operations simaple_char_ops = {
    .owner = THIS_MODULE,
    .open = simple_char_open,
    .release = simple_char_release,
    .write = simple_char_write,
    .read = simple_char_read,
};

static void cleanup_dev(int device_created) {
    if (device_created) {
        device_destroy(simple_char_dev_class, simple_char_major);
        cdev_del(&g_simple_char_devp->cdev);
    }
    if (simple_char_dev_class)
        class_destroy(simple_char_dev_class);
    if (simple_char_major != -1)
        unregister_chrdev_region(simple_char_major, 1);
}

static int __init simple_char_init(void) {
    int ret;
    int time_since_boot;
    int device_created = 0;
    /* cat /proc/devices */
    if (alloc_chrdev_region(&simple_char_major, 0, 1, DEVICE_NAME) < 0) {
        printk("[dungru:%d:%s] Can't Register the Device\n", __LINE__, __FUNCTION__);
        return -1;
    }
    /* Populate sysfs entries ls /sys/class */
    simple_char_dev_class = class_create(THIS_MODULE, DEVICE_NAME);
    /* Send uevents to udev, so it will create /dev nodes */
    /* ls /dev/ */
    simple_char_dev_device = device_create(simple_char_dev_class, NULL, MKDEV(MAJOR(simple_char_major), 0), NULL, DEVICE_NAME);
    device_created = 1;
    /* Allocate memory for per-device structure*/
    g_simple_char_devp = kmalloc(sizeof(struct simple_char_dev), GFP_KERNEL);
    if (!g_simple_char_devp) {
        printk("[dungru:%d:%s] Bad kmalloc\n", __LINE__, __FUNCTION__);
    }
    /* Request IO Region */
    sprintf(g_simple_char_devp->name, DEVICE_NAME);
    /* Connect the file operation with the cdev */
    cdev_init(&g_simple_char_devp->cdev, &simaple_char_ops);
    g_simple_char_devp->cdev.owner = THIS_MODULE;
    /* Connect the Major/minor number to the cdev */
    ret = cdev_add(&g_simple_char_devp->cdev, simple_char_major, 1);
    if (ret) {
        printk("[dungru:%d:%s] Bad cdev\n", __LINE__, __FUNCTION__);
        return ret;
    }
    /* Init the pravite data */
    memset(g_simple_char_devp->in_string, 0, 256);
    time_since_boot = (jiffies - INITIAL_JIFFIES)/HZ;
    sprintf(g_simple_char_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);

    g_simple_char_devp->curr_wr_pointer = 0;

    printk("[dungru:%d:%s] simple_char driver initialized.\n", __LINE__, __FUNCTION__);
    return 0;
error:
    cleanup_dev(device_created);
    return -1;
}

static void __exit simple_char_exit(void) {
    cleanup_dev(1);
    printk("[dungru:%d:%s] \n", __LINE__, __FUNCTION__);
}

module_init(simple_char_init);
module_exit(simple_char_exit);
MODULE_AUTHOR("Dung-Ru Tsai");
MODULE_DESCRIPTION("Hello Character Driver!");
MODULE_LICENSE("Dual BSD/GPL");