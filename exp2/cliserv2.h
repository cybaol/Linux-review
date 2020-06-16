#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>     /* for nonblocking */
#include <limits.h>    /* PIPE_BUF */
#include <semaphore.h> /* Posix semaphores */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>  /* Posix shared memory */
#include <sys/stat.h>  /* for S_xxx file mode constants */
#include <sys/time.h>  /* timeval{} for select() */
#include <sys/types.h> /* basic system data types */
#include <sys/wait.h>
#include <time.h> /* timespec{} for pselect() */
#include <unistd.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define MESGSIZE 256 /* max #bytes per message, incl. null at end */
#define NMESG 16     /* max #messages */

struct shmstruct { /* struct stored in shared memory */
    sem_t mutex;   /* three Posix memory-based semaphores */
    sem_t nempty;
    sem_t nstored;
    int nput;                       /* index into msgoff[] for next put */
    long noverflow;                 /* #overflows by senders */
    sem_t noverflowmutex;           /* mutex for noverflow counter */
    long msgoff[NMESG];             /* offset in shared memory of each message */
    char msgdata[NMESG * MESGSIZE]; /* the actual messages */
};

/*
	Multiple  Producer      ptr returned by      Consumer
		     |		|   mmap		 |
		     |		V      			 |
		     |		+---------------+	 |
		     |----P()-->|mutex: 1       |	 |         msgdata
		     |----P()-->|nempty: 16     |--V()-->|  	 +--------------------+
		     |----V()-->|nstored: 0     |--P()-->|	 |pid 2312: massage 2 |
		     |		+---------------+    	 |	 |	...    	      |
		     |----P()-->|noverflowmutex |   +--index	 |      ...    	      |
		     |		|noverflow      |   |    |   +-->|--------------------|
		     |		+---------------+   | 	 |   |	 |	       	      |
		     |      +---+nput: 4        |   | 	 |   |	 |             	      |
		     |	    |	+---------------+   | 	 |   |	 |	       	      |
		     |	    | 	|msgoff long 	|   | 	 |   |	 |	       	      |
		     |	    | 	|     ...       |<--+ 	 |   |	 +--------------------+
                     |	    | 	|     ...       |    	 |   |   |<------MSGSIZE----->|
                     |	    |  	|     ...       |        |   |
		     |	    +-->| 4 * MSGSIZE   +---+    |   |
		     |		|     ... 	|   |	 |   |
		     |		|     ...       |   |	 |   |
		     |		|     ...       |   |	 |   |
		     |		|     ...       |   | 	 |   |
		     |		+---------------+   | 	 |   |
		     |				    | 	 |   |
						    | 	     |
  						    +--------+
*/
