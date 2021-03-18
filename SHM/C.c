/*
* C.c is a process file that runs simultaneously to A.c and C.c
*   the program waits for '1' to be written to integer shared memory (smh_i)
*   then writes "shared" to string shared memory (shm_s) and "2" to shm_i
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ     27

int
main()
{
    int shmid_s, shmid_i;
    key_t key_s, key_i;
    char *shm_s, *s;
    int *shm_i, *i;
    
    // create string shared memory and link it
    key_s = 5678;

    if( (shmid_s = shmget(key_s, SHMSZ, 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }
    if( (shm_s = shmat(shmid_s, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }


    // create integer shared memory and link it
    key_i = 1234;

    if( (shmid_i = shmget(key_i, SHMSZ, 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }

    if( (shm_i = shmat(shmid_i, NULL, 0)) == (int *) -1 )
    {
        perror("shmat");
        exit(1);
    }

    // wait until process B writes "2" into shared memory
    while( *shm_i != 2)
        sleep(1);
    
    // continue. program will stay in loop until 2 is read from shm_i
    
    //write "memory" into string shm
    sprintf(shm_s, "memory");

    //write 3 into int shm
    *shm_i = 3;
    
    return 0;
}