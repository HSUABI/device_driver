#include <linux/module.h>	// needed all modules
#include <linux/kernel.h>	// needed kernel function
#include <linux/init.h>		// needed module macros
#include <linux/uaccess.h>	// needed user space copy functions
#include <linux/fs.h>		// needed file operations
#include <linux/miscdevice.h>	// needed by misc device driver functions


#define DRIVER_AUTHOR	"CAUSW song"
#define DRIVER_DESC	"driver for midterm_dotmatrix"

#define DOTM_NAME 		"midterm_dotmatrix"
#define DOTM_MODULE_VERSION	"midterm_dotmatrix V1.0"
#define DOTM_ADDR 		0x210

#define DOTM_MAGIC      0xBC
#define DOTM_SET_ALL        _IOW(DOTM_MAGIC, 0, int)
#define DOTM_SET_CLEAR      _IOW(DOTM_MAGIC, 1, int)
#define DOTM_SET_ONE        _IOW(DOTM_MAGIC, 2, int)
#define DOTM_SET_TWO        _IOW(DOTM_MAGIC, 3, int)
#define DOTM_SET_THREE      _IOW(DOTM_MAGIC, 4, int)
#define DOTM_SET_FOUR       _IOW(DOTM_MAGIC, 5, int)
#define DOTM_SET_FIVE       _IOW(DOTM_MAGIC, 6, int)
#define DOTM_SET_SIX        _IOW(DOTM_MAGIC, 7, int)
#define DOTM_SET_SEVEN      _IOW(DOTM_MAGIC, 8, int)
#define DOTM_SET_EIGHT      _IOW(DOTM_MAGIC, 9, int)
#define DOTM_SET_NINE       _IOW(DOTM_MAGIC, 10, int)


// gpio fpga interface provided
extern ssize_t iom_fpga_itf_read(unsigned int addr);
extern ssize_t iom_fpga_itf_write(unsigned int addr, unsigned short int value);

// global
static int dotm_in_use = 0;



unsigned char dotm_fontmap_name[10][10] = {
    {0x08,0x14,0x22,0x08,0x7F,0x00,0x08,0x14,0x14,0x08}, // 송
    {0x00,0x22,0x72,0x56,0x52,0x02,0x08,0x14,0x14,0x08}, // 성
    {0x08,0x14,0x14,0x08,0x00,0x3E,0x08,0x1C,0x04,0x04}, // 욱
};
unsigned char dotm_fontmap_decimal[10][10] = {
    {0x3e,0x7f ,0x63,0x73,0x73, 0x6f ,0x67,0x63,0x7f,0x3e}, // 0
    {0x0c ,0x1c,0x1c,0x0c ,0x0c,0xc ,0x0c,0x0c,0x0c,0x1e}, // 1
    {0x7e,0x7f ,0x03 ,0x03,0x3f ,0x7e,0x60,0x60,0x7f ,0x7f}, // 2
    {0xfe,0x7f ,0x03 ,0x03,0x7f ,0x7f ,0x03,0x03,0x7f ,0x7e}, // 3
    {0x66 , 0x66, 0x66 ,0x66,0x66,0x66,0x7f,0x7f ,0x06,0x06}, // 4
    {0x7f ,0x7f ,0x60 ,0x60,0x7e, 0x7f ,0x03,0x03,0x7f ,0x7e}, // 5
    {0x60 , 0x60, 0x60 ,0x60,0x7e, 0x7f ,0x63,0x63,0x7f,0x3e}, // 6
    {0x7f ,0x7f ,0x63 ,0x63 ,0x03,0x03 ,0x03,0x03,0x03,0x03}, // 7
    {0x3e,0x7f ,0x63 ,0x63,0x7f ,0x7f ,0x63,0x63,0x7f,0x3e}, // 8
    {0x3e,0x7f ,0x63 ,0x63,0x7f ,0x3f ,0x03,0x03,0x03,0x03} // 9
};

unsigned char dotm_fontmap_full[10] = {
    0x7f ,0x7f ,0x7f ,0x7f ,0x7f ,0x7f ,0x7f ,0x7f ,0x7f ,0x7f
};

unsigned char dotm_fontmap_empty[10] = {
    0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
};



int dotm_open(struct inode *pinode, struct file *pfile)
{
	if (dotm_in_use != 0) {
		return -EBUSY;
	}

	dotm_in_use = 1;

	return 0;
}

int dotm_release(struct inode *pinode, struct file *pfile)
{
	dotm_in_use = 0;

	return 0;
}

ssize_t dotm_write(struct file *pinode, const char *gdata, size_t len, loff_t *off_what)
{
    int ret, i;
    unsigned char num;
    unsigned short wordvalue;
    const char *tmp = NULL;

    tmp = gdata;

    if(len > 1){
        printk(KERN_WARNING "only 1 byte data will be processed\n");
        len = 1;
    }

    ret = copy_from_user(&num, tmp, 1);
    if(ret <0){
        return -EFAULT;
    }

    for(i=0; i<10; i++){
        wordvalue = dotm_fontmap_decimal[num][i] & 0x7F;
        iom_fpga_itf_write((unsigned int) DOTM_ADDR+(i*2), wordvalue);
    }

    return len;
}

static long dotm_ioctl(struct file *pinode, unsigned int cmd, unsigned long data)
{
    int i;
    int j;
    unsigned short wordvalue;

    switch(cmd){
    case DOTM_SET_ALL:
        for(i=0;i<10;i++){
            wordvalue = dotm_fontmap_full[i] & 0x7F;
            iom_fpga_itf_write((unsigned int) DOTM_ADDR+(i*2), wordvalue);
        }
        break;
    case DOTM_SET_CLEAR:
        for(i=0;i<10;i++){
            wordvalue = dotm_fontmap_empty[i] & 0x7F;
            iom_fpga_itf_write((unsigned int) DOTM_ADDR+(i*2), wordvalue);
        }
        break;
    case DOTM_SET_ONE:
        for(j=0;j<7;j++){ // j for loop is just for repeat-printing
            for(i=0;i<10;i++){
                wordvalue = dotm_fontmap_name[0][i] & 0x7F;
                iom_fpga_itf_write((unsigned int) DOTM_ADDR+(i*2), wordvalue);
            }
        }
        break;
    case DOTM_SET_TWO:
        for(j=0;j<7;j++){ // j for loop is just for repeat-printing
            for(i=0;i<10;i++){
                wordvalue = dotm_fontmap_name[1][i] & 0x7F;
                iom_fpga_itf_write((unsigned int) DOTM_ADDR+(i*2), wordvalue);
            }
        }
        break;
    }

    return 0;
}

static struct file_operations dotm_fops = {
	.owner	= THIS_MODULE,
	.open	= dotm_open,
	.write	= dotm_write,
    .unlocked_ioctl = dotm_ioctl,
	.release= dotm_release,
};

static struct miscdevice dotm_driver = {
	.fops	= &dotm_fops,
	.name	= DOTM_NAME,
	.minor 	= MISC_DYNAMIC_MINOR,
};


int dotm_init(void)
{
	misc_register(&dotm_driver);
	printk(KERN_INFO "driver: %s driver init\n", DOTM_NAME);

	return 0;
}

void dotm_exit(void)
{
	misc_deregister(&dotm_driver);
	printk(KERN_INFO "driver: %s driver exit\n", DOTM_NAME);
}

module_init(dotm_init);
module_exit(dotm_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");