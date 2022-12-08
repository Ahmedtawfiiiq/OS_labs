/*
 * Shared Memory Program (System V)
 *
 * One parent places characters in SM, and child reads it.
 *
 */

/*
 * Shared memory is when a single piece of memory is in the address 
 * space of two or more processes. 
 * If one process makes a change to this memory then it is changed for both processes. 
 * Some system calls:
 *   - int shmget(key_t key, int size, int shmflg);
 *   - char *shmat(int shmid, char *shmaddr, int shmflg);
 *   - int shmdt(char *shmaddr);
 *
 * shmget(): Creates or locats a piece of shared memory. 
 *           The pieces of shared memory are identified by a key, which is an integer 
 *           agreed upon by all processes that are going to use this piece of memory. 
 *           The call also specifies the size of the memory and the permission 
 *           flags (eg -rw-rw-rw-). 
 * shmat():  Takes the shared memory identifier and performs a mapping of that 
 *           into the address space of the process, returning a pointer to it. 
 *           From then on it can be used just like any pointer to memory. 
 * shmdt():  Detaches it from the address space of the process. 
 *
 * On the command line run ipcs to see SM segments etc.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {
	int shmid;
	char *shmPtr;
	int n;

	if (fork() == 0) {
		sleep(5);		/* UUPS */

		if( (shmid = shmget(2041, 32, 0)) == -1 ) {
  			exit(1);
		}

		shmPtr = shmat(shmid, 0, 0);
		if (shmPtr == (char *) -1) 
			exit(2);

		for (n = 0; n < 26; n++)
			putchar(shmPtr[n]);
		putchar('\n');
	}
	else {
		if( (shmid = shmget(2041, 32, 0666 | IPC_CREAT)) == -1 ) {
  			exit(1);
		}

		shmPtr = shmat(shmid, 0, 0);
		if (shmPtr == (char *) -1) 
			exit(2);

		for (n = 0; n < 26; n++)
			shmPtr[n] = 'a' + n;

		for (n = 0; n < 26; n++)
			putchar(shmPtr[n]);
		putchar('\n');
		wait(NULL);
		shmdt(NULL);

		/*
		 * Remove the shared memory segment identified by shmid from the system
		 * and destroy the shared memory segment.
		 */
		if( shmctl(shmid, IPC_RMID, NULL) == -1 ) {
			perror("shmctl");
			exit(-1);
		}
	}
	exit(0);
}
