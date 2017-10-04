#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function
#define  DEVICE_NAME "progtest"    
#define  CLASS_NAME  "pro"        

static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

void main()
{
	char message[256]; 
	char tipo;
	scanf("%s", message);
	
	int ret, fd;    
	    
	printf("Starting device test code example...\n");    
	fd = open("/dev/ebbchar", O_RDWR);             // Open the device with read/write access    
	if (fd < 0){       
	perror("Failed to open the device...");     
	return errno;    }
	
	
	ret=write(fd, message, strlen(message)); // Send the string to the LKM
	if(ret<0)
	{
		perror("Fail do write.");
	}
	
	return 0;
	
}




