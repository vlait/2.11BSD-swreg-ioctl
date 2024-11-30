#include<stdio.h>
#include<sys/ioctl.h>
#include<sys/file.h>
#include<fcntl.h>
#define PANEL "/dev/panel"

binout(unsigned int val){
	unsigned int i=( 0x1 << 15 );
	while (i) {
		printf("%d",val&i ? 1:0);
		i=i>>1;
	}
	printf("\n");
}
main(int argc, char *argv[]) {

	unsigned int data;
	int fd;
	int error;

	fd = open(PANEL,O_RDWR,0);
	if(fd < 0) { 
		printf("error opening %s\n",PANEL);
		exit(1);
	}
	error = ioctl(fd,MIOCGCSW,(char *)&data);
	if (error) {
		printf("ioctl error %d\n",error);
		exit(1);
	}	
	if (argc == 2 && argv[1][0]=='b') binout(data);
		else printf("%u\n",data);

	close(fd);
}
