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



int main(int argc, char **argv)
{
    int fd, num, i;

    if(argc <=1){
        fprintf(stderr, PROGRAM_USAGE_STRING, argv[0]);
        return -1;
    }

    num = (int) strtol(argv[1], NULL, 10);
    if(num<0||num>9){
        fprintf(stderr, PROGRAM_USAGE_STRING,argv[0]);
        return -1;
    }

    fd = open("/dev/midterm_dotmatrix",O_WRONLY);
    printf("input value : %d\n",num);
    if(fd!=-1){

        ioctl(fd,DOTM_SET_ALL,NULL);
        usleep(500000);

        switch (num){
        case 0:
            ioctl(fd,DOTM_SET_ONE,NULL);
            break;
        case 1:
            ioctl(fd,DOTM_SET_TWO,NULL);
            break;
        default:
            break;
        }

        ioctl(fd, DOTM_SET_CLEAR,NULL);
        usleep(500000);
        
        close(fd);
    } else {
            fprintf(stderr, "error opening device\n");
            return -1;
    }

    return 0;
}