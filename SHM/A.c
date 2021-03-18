#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> /* for exit */


/* 
 * A - Server file of first process using shared memory
 */


// shared memory size
#define SHMSZ     27


int 
main()
{
    char c;
    int shmid_s, shmid_i;
    key_t key_s, key_i;
    char *shm_s, *s;
    int *shm_i, *i;

    /*
     * We'll name our string shared memory segment
     * "5678".
     */
    key_s = 5678;

    /*
     * Create the segment.
     */
    if( (shmid_s = shmget(key_s, SHMSZ, IPC_CREAT | 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if( (shm_s = shmat(shmid_s, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }



    // Now repeat process with integer shared memory
    key_i = 1234;

    /*
     * Create the segment.
     */
    if( (shmid_i = shmget(key_i, SHMSZ, IPC_CREAT | 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if( (shm_i = shmat(shmid_i, NULL, 0)) == (int *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    // Process A writes "1" to the integer shared memory
    i = shm_i;
    *i = 1;

   
    while( *shm_i != 2 )
        sleep(1);
    
    printf("string shared memory: ");
        for( s = shm_s; *s != (char) NULL; s++ )
            putchar(*s);
        putchar('\n');

    while( *shm_i != 3 )
        sleep(1);

    printf("string shared memory: ");
        for( s = shm_s; *s != (char) NULL; s++ )
            putchar(*s);
        putchar('\n');

    printf("Goodbye");

    return 0;
}
