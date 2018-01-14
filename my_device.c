// make

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioport.h>

#include <linux/fs.h>
#include <linux/poll.h>
#include <asm/io.h>

#define DEVICE_MAJOR 42    // Parallel port major number
#define BUFFER_LENGTH 100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bruno Produit");
MODULE_DESCRIPTION("myDevice kernel device");
MODULE_VERSION("1.0");

static unsigned char var[BUFFER_LENGTH]; // Variable 100 bytes

static int     dev_open    (struct inode *inode, struct file *file);
static int     dev_release (struct inode *inode, struct file *file);
static ssize_t dev_read    (struct file *file, char *buf, size_t count, loff_t *offset);
static ssize_t dev_write   (struct file *file, const char *buf, size_t count, loff_t *offset);
static loff_t  dev_lseek   (struct file *file, loff_t offset, int whence);

struct file_operations par_fops = {
	read: dev_read,
	write: dev_write,
	open: dev_open,
	release: dev_release,
	llseek: dev_lseek
};

int kernelVal = 0;

static loff_t dev_lseek ( struct file *file, loff_t offset, int whence)
{

    printk (KERN_DEBUG "Device lseek  offset: %lld  whence: %d\n",offset, whence );
    
    // Test si le déplacement est par rapport à la position courante
    if (whence == SEEK_CUR){
        if (file->f_pos + offset < 0){                                  // Déplacement < 0
            file->f_pos = 0;
            printk (KERN_DEBUG "Device lseek  real effset: %d  ",  0);
            return 0;
        }    
        else if((file->f_pos + offset) >= (BUFFER_LENGTH - 1)) {        // Déplacement >= BUFFER_LENGTH-1
            file -> f_pos = BUFFER_LENGTH - 1;
            printk (KERN_DEBUG "Device lseek  real effset: %d  ",  BUFFER_LENGTH-1);
            return BUFFER_LENGTH - 1;           
        }
        else{                                                           // Déplacement standard
            file -> f_pos = file -> f_pos + offset;
            printk (KERN_DEBUG "Device lseek  real effset: %lld  ",  file->f_pos + offset);
            return file -> f_pos;
        }
    }
    
    // Test si le déplacement est par rapport au départ
    else if (whence == SEEK_SET){
         if (0 + offset < 0){                                           // Déplacement < 0
            file->f_pos = 0;
            printk (KERN_DEBUG "Device lseek  real effset: %d  ",  0);
            return 0;
        }    
        else if((0 + offset) >= (BUFFER_LENGTH - 1)) {                  // Déplacement >= BUFFER_LENGTH-1
            file -> f_pos = BUFFER_LENGTH - 1;
            printk (KERN_DEBUG "Device lseek  real effset: %d  ",  BUFFER_LENGTH-1);
            return BUFFER_LENGTH - 1;           
        }
        else{                                                           // Déplacement standard
            file -> f_pos = 0 + offset;
            printk (KERN_DEBUG "Device lseek  real effset: %lld  ",  0 + offset);
            return file -> f_pos;
        }
    }  
    
    // Test si le déplacement est par rapport à la fin
    else{
         if (BUFFER_LENGTH - 1 + offset < 0){                           // Déplacement < 0
            file->f_pos = 0;
            printk (KERN_DEBUG "Device lseek  real effset: %d  ",  0);
            return 0;
        }    
        else if((BUFFER_LENGTH - 1 + offset) >= (BUFFER_LENGTH - 1)) {  // Déplacement >= BUFFER_LENGTH-1
            file -> f_pos = BUFFER_LENGTH - 1;
            printk (KERN_DEBUG "Device lseek  real effset: %d  ",  BUFFER_LENGTH-1);
            return BUFFER_LENGTH - 1;           
        }
        else{                                                           // Déplacement standard
            file -> f_pos = BUFFER_LENGTH + offset;
            printk (KERN_DEBUG "Device lseek  real effset: %lld  ",  BUFFER_LENGTH-1 + offset);
            return file -> f_pos;
        }   
    }    
         
    return -1;

}

static int dev_open (struct inode *inode, struct file *file)
{
    printk (KERN_DEBUG "Device open, major: %d minor: %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));
    return (0);

}


static int dev_release (struct inode *inode, struct file *file)
{
    printk (KERN_DEBUG "Device released\n");
    return 0;
}

static ssize_t dev_read (struct file *file, char *buf, size_t count, loff_t *offset)
{
        
    if((file->f_pos + count) > (BUFFER_LENGTH - 1)) {                   // Lecture > BUFFER_LENGTH
        count = BUFFER_LENGTH - file->f_pos;
        if (copy_to_user (buf, &var[file->f_pos], count))
            return -EFAULT;       
    }
    else{                                                               // Lecture Standard
        if (copy_to_user (buf, &var[file->f_pos], count))
           return -EFAULT;
    }

    *offset += count;
    printk (KERN_DEBUG "dev_read: count, val, minor: %d, %d\n", (int)count, MINOR(file->f_inode->i_rdev));
    return count;

}


static ssize_t dev_write (struct file *file, const char *buf, size_t count, loff_t *offset)
{   
      
    if((file->f_pos + count) > (BUFFER_LENGTH - 1)) {                   // Ecriture > BUFFER_LENGTH
        count = BUFFER_LENGTH - file->f_pos;
        if (copy_from_user (&var[file->f_pos], buf, count))
            return -EFAULT;       
    }
    else{                                                               // Ecriture Standard
        if (copy_from_user (&var[file->f_pos], buf, count))
           return -EFAULT;
    }
    
    *offset += count;
    printk (KERN_DEBUG "dev_write: count, kernelVal: %d\n", (int)count);

    return count;
        printk (KERN_DEBUG "dev_write: count: %d, buf[0]:%d\n", (int)count,buf[0] );
}


int init_26_module(void)
{
    int res;
    int i; 
    
    for(i=0;i<BUFFER_LENGTH;i++){
        var[i]=(unsigned char)i;
    }
    
    printk (KERN_DEBUG "device start\n");
    res = register_chrdev(DEVICE_MAJOR, "my_device", &par_fops);

    return 0;
}


void cleanup_26_module (void)
{
    unregister_chrdev(DEVICE_MAJOR, "my_device");
    printk (KERN_DEBUG "device end\n");
}

module_init (init_26_module);
module_exit (cleanup_26_module);

