CC      := gcc
LIBS    := -pthread -lrt
CFLAGS  := -W

PROGS = client2 server2

all:	${PROGS}


client2:	client2.o
	${CC} ${CFLAGS} -o $@ client2.c ${LIBS}

server2:	server2.o
	${CC} ${CFLAGS} -o $@ server2.c ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFIFES}
