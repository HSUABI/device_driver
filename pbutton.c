#include <linux/module.h>	// needed all modules
#include <linux/kernel.h>	// needed kernel function
#include <linux/init.h>		// needed module macros
#include <linux/uaccess.h>	// needed user space copy functions
#include <linux/fs.h>		// needed file operations
#include <linux/miscdevice.h>	// needed by misc device driver functions


#define DRIVER_AUTHOR	"CAUSW song"
#define DRIVER_DESC	"driver for 8-PBUTTONs FPGA"

#define PBUTTON_NAME 		"pbutton"
#define PBUTTON_MODULE_VERSION	"PBUTTON V1.0"
#define PBUTTON_ADDR 		0x050

// gpio fpga interface provided
extern ssize_t iom_fpga_itf_read(unsigned int addr);
extern ssize_t iom_fpga_itf_write(unsigned int addr, unsigned short int value);

// global
static int pbutton_in_use = 0;

int pbutton_open(struct inode *pinode, struct file *pfile)
{
	if (pbutton_in_use != 0) {
		return -EBUSY;
	}

	pbutton_in_use = 1;

	return 0;
}

int pbutton_release(struct inode *pinode, struct file *pfile)
{
	pbutton_in_use = 0;

	return 0;
}

ssize_t pbutton_read(struct file *pinode, char *gdata, size_t len, loff_t *off_what)
{
	unsigned char bytevalues[9];
	unsigned short wordvalue;
	char *tmp = NULL;
	int i;

	tmp = gdata;

	printk(KERN_INFO "workvalue = ");
	for(i=0; i<9;i++)
	{
		wordvalue = iom_fpga_itf_read((unsigned int) PBUTTON_ADDR + (i*2));
    	bytevalues[i] = (unsigned char) wordvalue & 0x00FF;
		printk(KERN_INFO "%x ", wordvalue);
	}
	printk(KERN_INFO "\n");

	if (copy_to_user(tmp, &bytevalues, 9)){

		return -EFAULT;

	}

	return len;

	
}


static struct file_operations pbutton_fops = {
	.owner	= THIS_MODULE,
	.open	= pbutton_open,
	.read	= pbutton_read,
	.release= pbutton_release,
};

static struct miscdevice pbutton_driver = {
	.fops	= &pbutton_fops,
	.name	= PBUTTON_NAME,
	.minor 	= MISC_DYNAMIC_MINOR,
};


int pbutton_init(void)
{
	misc_register(&pbutton_driver);
	printk(KERN_INFO "driver: %s driver init\n", PBUTTON_NAME);

	return 0;
}

void pbutton_exit(void)
{
	misc_deregister(&pbutton_driver);
	printk(KERN_INFO "driver: %s driver exit\n", PBUTTON_NAME);
}

module_init(pbutton_init);
module_exit(pbutton_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");