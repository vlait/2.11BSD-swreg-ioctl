# 2.11BSD-swreg-ioctl
display/swreg access methods testing for pidp11  
This repo resulted from my need to extend esppdp   
( https://github.com/Spritetm/esppdp ) to allow it to interact with the physical world easier.  
CSW just happened to be the simplest "accessory" available that has no built-in access on 2.11BSD

Initial idea was to keep all kernel-related additions as light as possible,  
remains to be seen what the final result would look like.


*Massive thanks to Johnny Billquist for advice and general guidance on kernel internals*


Apologies for the mindflow comments, wanted to get something out before dec rush.. 
do not trust anything you read or anything you see on the disk image.

None of these are meant to be suggestions for 2.11BSD additions but 
PiDP11 users might find them ... useful .. or something :)

The tar files contain the patches and examples if you want to test just one approach.

disk image suitable for pidp11 2.11BSD pl482 based on Chase Covello's prepatched 481 
( https://github.com/chasecovello/211bsd-pidp11 )
please make sure you back up your own image before copying this in.

included are 
1. switch/dr access via syscalls - see /home/syscall
   - syscall code was posted by DigbyT on the pidp11 mailing list some years ago, this
   just adds them into libc for easier access. (see examples, definitions not added to include files .. yet-- maybe)
   patch (re)introduces 2 new syscalls 'getsw'/'setdr' for for CSW access

   to read CSW :
   ```
   extern unsigned int getsw();  
   ...  
   int a=getsw()  
   ```
   to set DR :  
   ```
   extern unsigned int setdr(unsigned int );  
   ...  
   unsigned int meh = 1212; setdr(meh)  
   ```
3. switch/dr access via sysctl - see /home/sysctl
   new variable hw.csw  
   This is RW for uid0 - RO for others - would be trivial to allow regular user write access.
   to read CSW :
   ```
   sysctl hw.csw 
   ```
   and
   to set DR value :  
   ```
   sysctl -w hw.csw=1212
   ```
     
   sysctl is (or at least i thought it was) meant to allow easier userland access to kernel tunables/data/memory.
   
   
5. new device /dev/panel (c 1 4) that is really only one word address/register , access either via regular fd or ioctl.  
   This is only an extension for the "mem" driver so does not require much more than adding  
   an ioctl call definition in /usr/src/sys/pdp/conf.c for the mem driver + some additional code for the minor 4 device access and the ioctl.
   
   see iotcl-mem for examples



Simh as in the PiDPD11 apparently does not handle the CSW register byte-wide access correctly, which caused some headache as i did not
want to introduce more assembly subroutines in the kernel that are only used once.
   
simh-classic-current already does have a fix(several) submitted by JohnnyB to fix that
, adding a copy here if needed
   
A disk image is compiled with #define SIMH_BORK YES (in /usr/src/sys/pdp/mem.c) until (pidp11)simh has the csw fix included.  
For now writes/reads are not safe if the CSW is not present and will likely cause a kernel panic.

... will be fixed if someone gets around to groom pidp11 simh source from teens to adulthood :)

   
   
