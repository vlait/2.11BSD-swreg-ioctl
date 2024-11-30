#include <fcntl.h>
#include <sys/file.h>
#include <stdio.h>
#include <errno.h>


#define MEM     "/dev/panel"

unsigned int
main()
{
        int fd;
        unsigned int lr;
        int i;

        fd = open( MEM, O_RDWR, 0 );
        if ( fd < 0 ) {
            printf("Open failed. %d\n", errno );
            return 1;
        }

        i = read( fd, &lr, 2 );
	printf("%u",lr);
        return 0;
}

