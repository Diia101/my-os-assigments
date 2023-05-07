#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;


void *thread_function(void* arg)
{
    int thread_no = *(int*)arg;
    info(BEGIN, 9, thread_no);
    // do something in the thread
    info(END, 9, thread_no);
    return NULL;
}



void *thread_function_49(void* arg)
{
    int thread_no = *(int*)arg;
     sem_wait(&sem);
    info(BEGIN, 5, thread_no);
    // do something in the thread
    info(END, 5, thread_no);
    sem_post(&sem);
    return NULL;
}


void *thread_function_6(void* arg)
{
    int thread_no = *(int*)arg;
    info(BEGIN, 4, thread_no);
    // do something in the thread
    info(END, 4, thread_no);
    return NULL;
}

int main(){
    init();

    info(BEGIN, 1, 0);
  sem_init(&sem,0,6);
    // creez procesul 2
    if (fork() == 0) {
        //  start
        info(BEGIN, 2, 0);
        // end
        info(END, 2, 0);
        exit(0);
    }
    //asteptam sa se termine copii lui P2 
      wait(NULL);

    // creez procesul P3
    if (fork() == 0) {
        //  start
        info(BEGIN, 3, 0);

        // creez procesul P6
        if (fork() == 0) {
            //  start
            info(BEGIN, 6, 0);

            // creez procesul P9
            if (fork() == 0) {
                //  start
                info(BEGIN, 9, 0);
                //creez threadurile
                 pthread_t threads[5];
                int thread_args[5];
                for (int i = 0; i < 5; i++) {
                    thread_args[i] = i+1;
                    pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
                }
                // asteptam sa se termine threadurile
                for (int i = 0; i < 5; i++) {
                    pthread_join(threads[i], NULL);
                }
            //end
                info(END, 9, 0);
                exit(0);
            }
            // asteptam sa se termine copii lui P9
            wait(NULL);

            // end
            info(END, 6, 0);
            exit(0);
        }
        //asteptam sa se termine copii lui P6
        wait(NULL);

  // end
        info(END, 3, 0);
        exit(0);
    }
        wait(NULL);
    
        // creez procesul P4
        if (fork() == 0) {
            // start
            info(BEGIN, 4, 0);
            // end

            //creez threadurile
             pthread_t threads[5];
                int thread_args[6];
                for (int i = 0; i < 6; i++) {
                    thread_args[i] = i+1;
                    pthread_create(&threads[i], NULL, thread_function_6, &thread_args[i]);
                }
                // asteptam sa se termine threadurile
                for (int i = 0; i < 6; i++) {
                    pthread_join(threads[i], NULL);
                }
            info(END, 4, 0);
            exit(0);
        }
        //asteptam sa se termine copii lui P4
         wait(NULL);


        // creez procesul P5
        if (fork() == 0) {
            // start
            info(BEGIN, 5, 0);
            // end

            //creez threadurile
             pthread_t threads[49];
                int thread_args[49];
                for (int i = 0; i < 49; i++) {
                    thread_args[i] = i+1;
                    pthread_create(&threads[i], NULL, thread_function_49, &thread_args[i]);
                }
                // astpetam sa se termine threadurile
                for (int i = 0; i < 49; i++) {
                    pthread_join(threads[i], NULL);
                }
                sem_destroy(&sem);
            info(END, 5, 0);
            exit(0);
        }

        //asteptam sa se termine copii lui P9
        wait(NULL);

        // creez procesul P7
        if (fork() == 0) {
            // start
            info(BEGIN, 7, 0);

            // creez procesul P8
            if (fork() == 0) {
                // start
                info(BEGIN, 8, 0);
                // end
                info(END, 8, 0);
                exit(0);
            }

            // asteptam sa se termine copii lui P8
            wait(NULL);

            // end
            info(END, 7, 0);
            exit(0);
        }

        //asteptam sa se termine copii lui P7
        wait(NULL);

    // final end
    info(END, 1, 0);
    return 0;
}