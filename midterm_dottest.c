#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define PROGRAM_USAGE_STRING "usage: %s [0-9]\n"

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


void print_name();
void print_id();

int id[]={2,0,1,7,4,1,0,7};

int main(int argc, char **argv)
{
    int fd, num, i;
    int button_fd;
    unsigned char bytedata[9]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned char ret;
    unsigned char buf[9];


    button_fd = open("/dev/pbutton", O_RDWR);
	if(button_fd<0){
		printf("Device open error : /dev/pbutton\n");
		return -1;
	}

    fd = open("/dev/midterm_dotmatrix",O_WRONLY);
    printf("input value : %d\n",num);


    if(fd!=-1){

        while(1){
            ret = read(button_fd,&bytedata,9);
            if(ret<0){
                printf("Read Error!\n");
                return -1;
            }
            printf("Current PBUTTON Value: ");
            for(i=0; i<9;i++)
            {
                printf("0x%x ",bytedata[i]);
            }
            printf("\n");

            ioctl(fd, DOTM_SET_CLEAR,NULL);
            if(bytedata[0] == 1 )   ioctl(fd,DOTM_SET_ONE,NULL);
            else if(bytedata[1] == 1 )   ioctl(fd,DOTM_SET_TWO,NULL);
            else if(bytedata[2] == 1 )   ioctl(fd,DOTM_SET_THREE,NULL);
            else if(bytedata[3] == 1 )   print_name(&fd);
            else if(bytedata[4] == 1 )   print_id(&fd);
            else if(bytedata[5] == 1 )   ioctl(fd,DOTM_SET_SIX,NULL);
            else if(bytedata[6] == 1 )   ioctl(fd,DOTM_SET_SEVEN,NULL);
            else if(bytedata[7] == 1 )   ioctl(fd,DOTM_SET_EIGHT,NULL);
            else if(bytedata[8] == 1 )   ioctl(fd,DOTM_SET_NINE,NULL);
            ioctl(fd, DOTM_SET_CLEAR,NULL);
            

        }
        
        
        close(button_fd);
        close(fd);
    } else {
            fprintf(stderr, "error opening device\n");
            return -1;
    }

    return 0;
}

void print_name(int *fd)
{
    ioctl(*fd,DOTM_SET_ONE,NULL);
    usleep(500000);
    ioctl(*fd,DOTM_SET_TWO,NULL);
    usleep(500000);
    ioctl(*fd,DOTM_SET_THREE,NULL);
    usleep(500000);
}

void print_id(int *fd)
{
    int i;

    for(i=0; i<8; i++){
        write(*fd, &id[i], sizeof(id[i]));
        usleep(500000);
    }
}