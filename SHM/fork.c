#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *glob_var;
static char c;
static int shmid_s, shmid_i;
static key_t key_s, key_i;
static char *shm_s, *s;
static int *shm_i, *i;

#define SHMSZ     27

int main(void)
{
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

    /*
    * create child B
    */
    if (fork() == 0) {
        //write "shared" into string shared memory
        sprintf(shm_s, "shared");

        //write 2 into int shm
        *shm_i = 2;

        printf("string shared memory: ");
            for( s = shm_s; *s != (char) NULL; s++ )
                putchar(*s);
            putchar('\n');
            
        /*
        * create child C
        */
         if (fork() == 0) {
            //write "memory" into string shared memory
            sprintf(shm_s, "memory");

            //write 3 into int shm
            *shm_i = 3;

            printf("string shared memory: ");
            for( s = shm_s; *s != (char) NULL; s++ )
                putchar(*s);
            putchar('\n');

            //exit child C
            exit(EXIT_SUCCESS);
        } 
        else {
            wait(NULL);
        }
        //exit child B
        exit(EXIT_SUCCESS);
    } 
    else {
        wait(NULL);
    }

    // exit process A
    return 0;
}