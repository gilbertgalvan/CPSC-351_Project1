/**
 * jiffies.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

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
#define PROC_NAME "jiffies"

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

long unsigned int timeInInit = 0;
long unsigned int timeInExit = 0;
/* This function is called when the module is loaded. */
int jiffies_init(void)
{
    //printk(KERN_INFO "Loading Module\n");
    //printk(KERN_INFO "GOLDEN_RATIO_PRIME: %lu\n", GOLDEN_RATIO_PRIME);

    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    //printk(KERN_INFO "HZ = %i\n", HZ);

    return 0;
}

/* This function is called when the module is removed. */
void jiffies_exit(void)
{

    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);

    //long unsigned int answer = 0;
    //answer = gcd(3300, 24);
    //printk(KERN_INFO "gcd(3300,24): %lu\n", answer);
    //timeInExit = jiffies;
    //printk(KERN_INFO "Jiffies in siimple_exit() = %lu\n", jiffies);
    //printk(KERN_INFO "simple_init() jiffies - simple_exit() jiffies = %lu\n", timeInExit - timeInInit);
    printk(KERN_INFO "Removing Module\n");
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

    rv = sprintf(buffer, "Hello from /proc/jiffies\n");
    timeInInit = jiffies;
    printk(KERN_INFO "Current value of jiffies = %lu\n", jiffies);

    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

/* Macros for registering module entry and exit points. */
module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("SGG");
