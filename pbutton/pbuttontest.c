#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd;
	unsigned char bytedata[9]={-1,-1,-1,-1,-1,-1,-1,-1,-1};
	unsigned char ret;

	fd = open("/dev/pbutton", O_RDWR);
	if(fd<0){
		printf("Device open error : /dev/pbutton\n");
		return -1;
	}


	unsigned char bak = 0;
	
	while(1){

		ret = read(fd,&bytedata,9);
		if(ret<0){
			printf("Read Error!\n");
			return -1;
		}
		
		int i;
		printf("Current PBUTTON Value: ");
		for(i=0; i<9;i++)
		{
			printf("0x%x ",bytedata[i]);
		}
		printf("\n");
		
		bak = bytedata;
	
	}
	
	close(fd);

	return 0;
}