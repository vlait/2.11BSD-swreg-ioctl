The mem/ioctl is an extension to the "mem" device driver 

device major 1 minor 4 exposes the memory at the CSW address only (at least hopefully) to the user and is really simple

```
/* minor device 3 is ZERO (/dev/zero) */
                case 3:
                        if      (uio->uio_rw == UIO_WRITE)
                                return(EIO);
                        c = MIN(iov->iov_len, sizeof (zero));
                        error = uiomove(zero, c, uio);
                        break;
/* minor device 4 is the DISPLAY/SWITCH register */
                case 4:
                /*pidp11 simh will not allow byte sized access to CSW
                 *this only to hopefully safely limit the access to reg addr
                 *a patch submitted by grandmaster JohnnyB is available in
                 *simh-classic current to fix this.. for now however- this
                */
                        error = uiomove(CSW,iov->iov_len>=2?2:iov->iov_len,uio);
                        break;
                default:
                        return(EINVAL);
```
uiomove is the general kernel helper used to copy user data in/out of the kernel space , it also has fault trapping so that trying to access 
the CSW on a system where it does not exist will not cause a kernel panic.

/dev/panel is as much as /dev/mem a "real" file, it is best to be dealt with open/read/write/close rather than the higher 
level file counterparts.

for example to set a value to the dr
```
main(void)
{
        int fd;
        unsigned int lr=1212;
        int i;

        fd = open( MEM, O_RDWR, 0 );
        if ( fd < 0 ) {
            printf("Open failed. %d\n", errno );
            return 1;
        }

        i = write( fd, &lr, 2);

        return i;
}
```

This patch also adds an ioctl interface to same major 1 minor 4 device allowing access to the address/register via ioctl which is possibly a little safer.

```
/* mem device ioctl */
mmioctl(dev, com, data, flag)
        dev_t dev;
        u_int com;
        caddr_t data;
        int flag;
{
        int error;

        if(minor(dev) != 4)
                return(ENODEV);


        switch (com) {
                case MIOCGCSW:
        /* data buffer passed by the ioctl syscall already in seg6,
         * don't remap anything... fmove used to avoid traps if CSW
         * does not exist on the host.
        */
#ifdef SIMH_BORK
                        error = 0;
                        *data = *CSW;
#else
                        error = fmove(*KDSA6,*KDSD6,CSW,data,2);
#endif
                        return(error);
                case MIOCSCSW:
#ifdef SIMH_BORK
                        error = 0;
                        *CSW = *data;
#else
                        error = fmove(*KDSA6,*KDSD6,data,CSW,2);
#endif
                        return(error);

                default:
                        return (ENXIO);
                }
        return (0);
```

Simh currently used by PiDP11 will not allow byte-wide access to the CSW which the kernel helper 'fmove' wants to use
for short copies, fmove however would have the benefit of trapping faults and returning an error instead of a kernel 
panic when the CSW is not present. 

Johnny Billquist submitted a patch for simh that is already committed to the original simh - simh-classic - current sources,
it might take some time to get the patch to appear on v4/opensimh.


ioctl access is really not much different from file access in this case.  
for example to read the sw register:  

```
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
        /*read value from CSW*/
        error = ioctl(fd,MIOCGCSW,(char *)&data);
        if (error) {
                printf("ioctl error %d\n",error);
                exit(1);
        }
        if (argc == 2 && argv[1][0]=='b') binout(data);
                else printf("%u\n",data);

        close(fd);
}
```








