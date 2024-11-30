#
# Public Domain. 2024/11/28 vla 
#
#
CFLAGS=	 -O
SEPFLAG= -i
SRCS1=	drset.c 
OBJS1=	drset.o 
SRCS2=  swread.c
OBJS2=  swread.o
SRCS3=  mem-getdr.c
OBJS3=  mem-getdr.o
SRCS4=  mem-setdr.c
OBJS4=  mem-setdr.o

SRCS= ${SRCS1} ${SRCS2} ${SRCS3} ${SRCS4}
OBJS= ${OBJS1} ${OBJS2} ${OBJS3} ${OBJS4}

all: ioctl-setdr ioctl-getsw mem-getdr mem-setdr

ioctl-setdr: ${OBJS1}
	${CC} ${CFLAGS} ${SEPFLAG} -o $@ ${OBJS1}

ioctl-getsw: ${OBJS2}
	${CC} ${CFLAGS} ${SEPFLAG} -o $@ ${OBJS2}

mem-getdr: ${OBJS3}
	${CC} ${CFLAGS} ${SEPFLAG} -o $@ ${OBJS3}

mem-setdr: ${OBJS4}
	${CC} ${CFLAGS} ${SEPFLAG} -o $@ ${OBJS4}
clean:
	rm -f ${OBJS} 

