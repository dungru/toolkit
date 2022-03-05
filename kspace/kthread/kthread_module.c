#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/slab.h>
// copy_to_user
#include <linux/uaccess.h>
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Dung-Ru Tsai, Taiwan");
MODULE_DESCRIPTION("Kthread Simple Execrise");
MODULE_VERSION("0.1");


#define MODULE_NAME "kth_counter"
#define PERMS 0644
#define PARENT NULL

static struct task_struct *count_task;
static int counter;

static char *message_kspace;
static int read_p;

int counter_proc_open(struct inode *inode, struct file *file) {

    printk(KERN_ALERT "[%s:%d] \n", __FUNCTION__, __LINE__);
    read_p = 1;
    message_kspace = kmalloc(sizeof(char) * 20, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
    if (message_kspace == NULL) {
        printk(KERN_ERR "ERROR, counter_proc_open");
        return -ENOMEM;
    }
    sprintf(message_kspace, "The counter is now at: %d\n", counter);
    return 0;
}

ssize_t counter_proc_read(struct file *file, char *buf, size_t count, loff_t *ppos) {
    int len = strlen(message_kspace);
    int ret = 0;
    read_p = !read_p;
    if (read_p) {
        return 0;
    }
    printk(KERN_ALERT "[%s:%d] \n", __FUNCTION__, __LINE__);
    // Returns number of bytes that could not be copied. On success, this will be zero.
    ret = copy_to_user(buf, message_kspace, len);
    if (ret)
        len = 0;
    return len; //  Must return the read size
}

int counter_proc_release(struct inode *inode, struct file *file) {
    printk(KERN_ALERT "[%s:%d] \n", __FUNCTION__, __LINE__);
    kfree(message_kspace);
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = counter_proc_open,
    .read = counter_proc_read,
    .release = counter_proc_release,
};

int counter_run(void *data) {
    while (!kthread_should_stop()) {
        ssleep(1);
        counter += 1;
    }
    printk(KERN_ALERT "The counter thread has terminated\n");
    return counter;
}

static int kthread_counter_init_module(void) {
    int error = 0;
    printk(KERN_ALERT "/proc/%s create\n", MODULE_NAME);

    if (!proc_create(MODULE_NAME, PERMS, NULL, &fops)) {
        printk(KERN_ERR "ERROR! proc_create\n");
        error = -ENOMEM;
        goto error_proc;
    }
        
    count_task = kthread_run(counter_run, NULL, MODULE_NAME);
    if (IS_ERR(count_task)) {
        error = PTR_ERR(count_task);
        goto error_kthread;
    }
    goto success;
error_kthread:
    printk(KERN_ERR "ERROR! kthread_run\n");
error_proc:
    remove_proc_entry(MODULE_NAME, NULL);
success:
    return error;
}

static void kthread_counter_exit_module(void) {
    int ret = kthread_stop(count_task);
    if (ret != -EINTR)
        printk(KERN_ALERT "Counter thread has stopped\n");
    remove_proc_entry(MODULE_NAME, NULL);
    printk(KERN_ALERT "Removing /proc/%s.\n", MODULE_NAME);
}

module_init(kthread_counter_init_module);
module_exit(kthread_counter_exit_module);