/*
 * sploitable.c: A simple module whose only purpose is to present
 * a buffer-overflow vulnerability to a normal user by way of a 
 * character device.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>     /* for put_user */

/*
 * Prototypes - this woud normally go in a .h file 
 */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

/* dacashman added */
static int exploit_loc();

#define SUCCESS 0
#define DEVICE_NAME "sploitable"  /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80             /* Max length of the message from the device */

/*
 * Global variables are declared as static, so are global within the file
 */

static int Major;    /* Major number assigned to our device driver */
static int Device_Open = 0;  /* is device open? */
                             /* used to prevent multiple access to device */
static char msg[BUF_LEN];    /* The msg the device will give when asked */
static char *msg_Ptr;

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

/*
 * This function is called when the module is loaded.  
 */
int init_module(void){
  Major = register_chrdev(0, DEVICE_NAME, &fops);

  if(Major < 0){
    printk(KERN_INFO "Registering char device failed with %d\n", Major);
    return Major;
  }

  printk(KERN_INFO "I was assigned major number %d.  To talk to \n", Major);
  printk(KERN_INFO "the driver, create a dev file with\n");
  printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
  printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
  printk(KERN_INFO "the device file.\n");
  printk(KERN_INFO "Remove the device file and module when done.\n");

  /* read location of exploit_loc() function */
  void *expl_func = (void *) exploit_loc;
  printk(KERN_INFO "CANARY: BOOSH exploit_loc() address: %x\n", expl_func);

  return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void){
  /*
   * Unregister the device
   */
  unregister_chrdev(Major, DEVICE_NAME);
  
}

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */

static int device_open(struct inode *inode, struct file *filp){
  static int counter = 0;

  if(Device_Open)
    return -EBUSY; //what is this?

  Device_Open++;
  if(counter == 0){
    sprintf(msg, "I already told you %d times: Hello World!\n", counter++);
  }
  msg_Ptr = msg;
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

/*
 * Called when a process closes the device file
 */
static int device_release(struct inode *inode, struct file *filp){
  Device_Open--;     /* We're now ready for our next caller */
  
  /*
   * Decrement the usage count, or else once you opened the file, you'll 
   * never get rid of the module.
   */
  module_put(THIS_MODULE);

  return 0;
}

/*
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filep,   /* include/linux/fs.h */
			   char *buffer,         /* buffer to fill w/data*/
			   size_t length,        /* length of buffer */
			   loff_t * offset){

  
  /*
   * Number of bytes actually written to the buffer
   */
  int bytes_read = 0;
  
  /*
   * If we're at the end of the message,
   * return 0 signifying end of file
   */
  if(*msg_Ptr == 0)
    return 0;

  /*
   * Actually put the data into the buffer
   */
  while(length && *msg_Ptr){
    /*
     * The buffer is in the user data segment, not the kernel
     * segment so "*" assignment won't work.  We have to use
     * put_user which copies data from the kernel data segment to
     * the user data segment.
     */
    put_user(*(msg_Ptr++), buffer++);

    length--;
    bytes_read++;
  }

  /*
   * Most read functions return the number of bytes put into the buffer
   */
  return bytes_read;
}

static int
exploit_loc(){
  printk(KERN_EMERG "CANARY: exploit_loc() CALLED!\n");
  return 0;
}

/*
 * foo - implements function to try to find return address.
 *   also: foo has many args to make sure some are saved on stack.  They
 *   should help with canary location 
 */
static ssize_t
/* foo has many args to make sure some are saved on stack.  They
 *  should help with canary location 
 */
foo(char *buff, int buff_len, int a, int b, int c, int d, int e, int f, int g, int h, int j, int aa, int bb, int cc, int dd, int ee, int ff, int gg, int hh, int jj, int z){
  printk(KERN_DEBUG "CANARY: foo called\n");
  /* printing out all values to make sure compiler doesn't ignore them */
  printk(KERN_DEBUG "CANARY: foo args -> %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x\n",
	 a, b, c, d, e, f, g, h, j, aa, bb, cc, dd, ee, ff, gg, hh, jj, z );
  int i;
  int marker = 0xdeadbeef; //another marker on stack
  int marker_array[2] = {0xdeadbeef, 0xdeadbeef};
  char *vuln_marker = marker_array;

  printk(KERN_DEBUG "CANARY: marker -> %x, marker + a -> %x, marker_array\n", 
	 marker, marker+a, *vuln_marker);
  char vuln_buff[16] = "aaaabbbbccccddd";
  char *vuln_ptr = vuln_buff;

  /* attempting buffer overflow here */
  for(i = 0; i < buff_len/* && i < 20*/; i++)
    get_user(vuln_buff[i], buff + i);

  /*print out stack */
  vuln_ptr = vuln_ptr + 95;
  printk(KERN_INFO "CANARY print values from vuln_buff and above:\n");
  for(i = 0; i < 24; i++){
    printk(KERN_INFO "CANARY word %d: \%02hhx\%02hhx\%02hhx\%02hhx\n",
	   i, *vuln_ptr, *(vuln_ptr-1), *(vuln_ptr-2), *(vuln_ptr-3));
    vuln_ptr-=4;
  }

  struct task_struct *dan_task;
  dan_task = current;
  char name[256] = "not_implemented";
  //get_task_comm(name, dan_task);
  printk(KERN_DEBUG "CANARY current process id: %d, current process name: %s, "
	 "current process canary: %lx, parent process id: %d\n", 
	 dan_task->pid, name, dan_task->stack_canary, (dan_task->real_parent)->pid);

  printk(KERN_INFO "CANARY dump stack to get addresses to compare to ret-val\n");
  dump_stack();

  return 0;

}


/*
 * Called when a process writes to a dev file: echo "hi" > /dev/hello
 */

static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off){


  foo(buff, len, 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
      0xaaaabbbb, 0xaaaacccc, 0xaaaadddd, 0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
      0xaaaabbbb, 0xaaaacccc, 0xaaaadddd, 0xbbbbbbbb);
  /* commented out and replaced by foo 
   */
  int i;
  /*int marker = 15;
  char vuln_buff[16] = "aaaabbbbccccddd";
  char *vuln_ptr = vuln_buff;
  vuln_ptr = vuln_ptr + 95;
  printk(KERN_INFO "CANARY print values from vuln_buff and above:\n");
  for(i = 0; i < 24; i++){
    printk(KERN_INFO "CANARY word %d: \%02hhx\%02hhx\%02hhx\%02hhx\n",
	   i, *vuln_ptr, *(vuln_ptr-1), *(vuln_ptr-2), *(vuln_ptr-3));
    vuln_ptr-=4;
  }

  printk(KERN_INFO "CANARY dump stack to get addresses to compare to ret-val\n");
  dump_stack();
  */
  printk(KERN_INFO "device_write(%p, %s, %d)", filp, buff, len);


  for(i = 0; i < len && i < BUF_LEN; i++)
    get_user(msg[i], buff + i);

  msg_Ptr = msg;
  
  for(i = 0; i < BUF_LEN; i++)
    printk(KERN_INFO "Byte %d of msg buffer in kernel-space: %c\n",
	   i, msg[i]);

  /* 
   * Again, return the number of input characters used 
   */
  return i;


}
