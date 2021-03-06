/*
 * buff_ovrflw.c Vulnerable buffer in kernel
 */
#include <linux/module.h>
#include <linux/moduleparam.h>  /* new! used to grab params */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>         /* new!  not sure what  ... */

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Daniel A Cashman");


/*
 * module_param(foo, int, 0000)
 * The first param is the parameter's name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) 
 * at a later stage.
 */


//module_param(mystring, charp, 0000);
//MODULE_PARM_DESC(mystring, "A character string");

/*
 * module_param_array(name, type, num, perm)
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number 
 * of elemetns of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
 */



static int __init buff_ovrflw_init(void){
  char c_array[16];
  module_param_array(c_array, char, &arr_argc, 0000);
  MODULE_PARM_DESC(myintArray, "An array of chars");
  for(int i=0; i < 17; i++){
    printk(KERN_INFO "char %d: %c", i, c_array[i]);
  }
  
  return 0;
}

static void __exit buff_ovrflw_exit(void){
  printk(KERN_INFO "Goodbye, world BUFF OVERFLOW \n");
}
module_init(buff_ovrflw_init);
module_exit(buff_ovrflw_exit);
