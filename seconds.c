#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

long unsigned int timeInInit = 0;
long unsigned int timeInExit = 0;
/* This function is called when the module is loaded. */
int seconds_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

void seconds_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "Hello from /proc/seconds\n");
    timeInInit = jiffies;
    printk(KERN_INFO "Number of elapsed seconds since the kernel module was created = %lu\n", jiffies / HZ);
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}
module_init(seconds_init);
module_exit(seconds_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("seconds Module");
MODULE_AUTHOR("SGG");
