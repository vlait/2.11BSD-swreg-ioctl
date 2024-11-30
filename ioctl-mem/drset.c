#include<stdio.h>
#include<sys/ioctl.h>
#include<sys/file.h>
#include<fcntl.h>

main(int argc, char *argv[]) {

	unsigned int data;
	int fd;
	int error;

	if ( argc != 2 ) {
                printf("usage: %s <unsigned int value>\n",argv[0]);
                exit(1);
        }
	data = atoi(argv[1]);

	fd = open("/dev/panel",O_RDWR,0);
	if(fd < 0) 
		printf("error opening fd\n");
	
	error = ioctl(fd,MIOCSCSW,&data);
	if(error) {
		printf("iotcl error %d\n",error);
		exit(1);
	}
	close(fd);
}
