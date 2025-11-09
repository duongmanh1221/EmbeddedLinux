#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/kernel.h>

#define NPAGE  100
#define DRIVER_AUTHOR "Duong"
#define DRIVER_DESC "Hello Character device"
#define DRIVER_VERS "1.0"

struct m_chdev {
    int32_t size;               // số byte đã ghi
    char *kmalloc_ptr;          // bộ nhớ kernel
    dev_t dev_num;              // major/minor
    struct class *m_class;      // class
    struct cdev m_cdev;         // character device
} m_dev;

// Prototypes
static int m_open(struct inode *inode, struct file *file);
static int m_release(struct inode *inode, struct file *file);
static ssize_t m_read(struct file *filp, char __user *user_buff, size_t size, loff_t *offset);
static ssize_t m_write(struct file *filp, const char __user *user_buff, size_t size, loff_t *offset);

// File operations
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = m_read,
    .write = m_write,
    .open = m_open,
    .release = m_release,
};

// Open
static int m_open(struct inode *inode, struct file *file){
    pr_info("Device opened\n");
    return 0;
}

// Release
static int m_release(struct inode *inode, struct file *file){
    pr_info("Device closed\n");
    return 0;
}

// Read from kernel buffer to user
static ssize_t m_read(struct file *filp, char __user *user_buff, size_t size, loff_t *offset){
    size_t to_read;

    if (*offset >= m_dev.size)
        return 0; // hết dữ liệu

    to_read = min(size, (size_t)(m_dev.size - *offset));

    if (copy_to_user(user_buff, m_dev.kmalloc_ptr + *offset, to_read))
        return -EFAULT;

    *offset += to_read;
    return to_read;
}

// Write from user to kernel buffer
static ssize_t m_write(struct file *filp, const char __user *user_buff, size_t size, loff_t *offset){
    size_t to_write;
    pr_info("m_write called\n");
    if (*offset >= NPAGE * PAGE_SIZE)
        return 0;

    to_write = min(size, (size_t)(NPAGE * PAGE_SIZE - *offset));

    if (copy_from_user(m_dev.kmalloc_ptr + *offset, user_buff, to_write) != 0)
        return -EFAULT;

    *offset += to_write;
    m_dev.size = max(m_dev.size, (int32_t)(*offset));
    pr_info("m_write called\n");

    pr_info("Data written: %.*s\n", (int)to_write, m_dev.kmalloc_ptr + *offset - to_write);
    return to_write;
}

// Module init
static int __init chdev_init(void){
    // Allocate device number
    if (alloc_chrdev_region(&m_dev.dev_num, 0, 1, "dev_num") < 0){
        pr_err("Fail to alloc chdev_region\n");
        return -1;
    }

    pr_info("Hello kernel module\n");
    pr_info("Major = %d Minor = %d\n", MAJOR(m_dev.dev_num), MINOR(m_dev.dev_num));

    // Create class
    m_dev.m_class = class_create( "m_class");
    if (!m_dev.m_class){
        pr_err("Cannot create class\n");
        goto rm_device_num;
    }

    // Create device file /dev/m_device
    if (!device_create(m_dev.m_class, NULL, m_dev.dev_num, NULL, "m_device")){
        pr_err("Cannot create device\n");
        goto rm_class;
    }

    // Initialize cdev
    cdev_init(&m_dev.m_cdev, &fops);
    if (cdev_add(&m_dev.m_cdev, m_dev.dev_num, 1) < 0){
        pr_err("Cannot add cdev\n");
        goto rm_device;
    }

    // Allocate kernel buffer
    m_dev.kmalloc_ptr = kmalloc(NPAGE * PAGE_SIZE, GFP_KERNEL);
    if (!m_dev.kmalloc_ptr){
        pr_err("Cannot allocate kernel memory\n");
        goto rm_cdev;
    }

    m_dev.size = 0;
    pr_info("Module loaded successfully\n");
    return 0;

// Error cleanup
rm_cdev:
    cdev_del(&m_dev.m_cdev);
rm_device:
    device_destroy(m_dev.m_class, m_dev.dev_num);
rm_class:
    class_destroy(m_dev.m_class);
rm_device_num:
    unregister_chrdev_region(m_dev.dev_num, 1);
    return -1;
}

// Module exit
static void __exit chdev_exit(void){
    kfree(m_dev.kmalloc_ptr);
    cdev_del(&m_dev.m_cdev);
    device_destroy(m_dev.m_class, m_dev.dev_num);
    class_destroy(m_dev.m_class);
    unregister_chrdev_region(m_dev.dev_num, 1);
    pr_info("Goodbye\n");
}

module_init(chdev_init);
module_exit(chdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
