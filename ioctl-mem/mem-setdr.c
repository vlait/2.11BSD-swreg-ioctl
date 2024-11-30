#include <fcntl.h>
#include <sys/file.h>
#include <stdio.h>
#include <errno.h>


#define MEM     "/dev/panel"

unsigned int
main(int argc, char * argv[])
{
        int fd;
        unsigned int lr;
        int i;

	if ( argc != 2 ) {
		printf("usage: %s <unsgigned int value>\n",argv[0]);
		exit(1);
	}
	lr = atoi(argv[1]);
        fd = open( MEM, O_RDWR, 0 );
        if ( fd < 0 ) {
            printf("Open failed. %d\n", errno );
            return 1;
        }

	i = write( fd, &lr, 2);

        return i;
}

