#include <linux/module.h>
#include <linux/kernel.h>

#define DRIVER_AUTHOR "Duong"
#define DRIVER_DESC "Hello"
#define DRIVER_VERS "1.0"

static int __init hello_world(void){
    printk(KERN_INFO "Hello kernel modules\n");
    return 0;
}

static void __exit hello_exit(void){
    printk(KERN_INFO "Goodbye\n");
}

module_init(hello_world);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);