/*
 * THe following license idents are currently accepted as indicating free
 * software modules
 *
 *    "GPL"                        [GNU Public license v2 or later]
 *    "GPL v2"                     [GNU Public License v2]
 *    "GPL and additional rights"  [GNU Public license v2 rights and more]
 *    "Dual BSD/GPL"               [GNU Public LIcense v2 or BSD license choice]
 *    "Dual MIT/GPL"               [GNU Public Licence v2 or MIT license choice]
 *    "Dual MPL/GPL"               [GNU Public LIcense v2 or Mozilla license choice]
 *    
 *
 * The following other idetns are available
 *
 *    "Proprietary"                [non free products]
 *
 * There are dual licensed components, but when runnign with Linux it is the
 * GPL that is relevant so this is a non issue.  Similarly, LGPL linked with GPL
 * is a GPL combined work.
 *
 * This exists for several reasons
 * 1.   So modinfo can show license info for users wanting to vet their setup
 *      is free
 * 2.   So the community can ignore bug reports including proprietary modules
 * 3. So vendors can do likewise based on their own policies
 */

/*
 * hello-4.c - Demonstrates module documentation.
 */
#include <linux/module.h>  /* needed by all modules */
#include <linux/kernel.h>  /* needed for printk macros */
#include <linux/init.h>    /* needed for init macros */

#define DRIVER_AUTHOR "Daniel A Cashman <dcashman@cs.ucsd.edu>"
#define DRIVER_DESC   "An example driver from the internetz"

static int __init init_hello_4(void){
  printk(KERN_INFO "Hello, world 4\n");
  return 0;
}

static void __exit cleanup_hello_4(void){
  printk(KERN_INFO "Goodbye, world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

/* get rid of taint message by declaring code as GPL */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR); /* who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC);   /* What does this module do? */

/*
 * This module uses /dev/testdevice. The MODULE_SUPPORTED_DEVICE macro
 * might be used in the future to help automatic configuration of modules, but
 * is currently unused other than for documentation purposes.
 */
MODULE_SUPPORTED_DEVICE("testdevice");
