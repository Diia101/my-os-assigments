#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
sem_t sem;
sem_t *s1,*s2;
int k=0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

//pthread_cond_t cond2= PTHREAD_COND_INITIALIZER;


void *thread_function(void* arg)
{
    int thread_no = *(int*)arg;
     pthread_mutex_lock(&lock);

      if(thread_no==2)
    {
        if(k==1)
            info(BEGIN,9,thread_no);
        else
        {
            pthread_cond_wait(&cond,&lock);
            info(BEGIN, 9, thread_no);
        }
    }
    // else if(thread_no==3)
    // {
    //         sem_wait(s1);
    //         info(BEGIN, 9, thread_no);
    // }
    else if(thread_no==5)
    {
            info(BEGIN, 9, thread_no);
            pthread_cond_signal(&cond);
            k=1;
    }else 
         info(BEGIN, 9, thread_no);
    
 if(thread_no==5)
    {
        pthread_cond_wait(&cond1,&lock);
        info(END, 9, thread_no);
    }
    else if(thread_no==3)
    {
          info(END, 9, thread_no);
          sem_post(s2);
         
    }
    else if(thread_no==2)
    { 
        info(END, 9, thread_no);
        pthread_cond_signal(&cond1);
    }
    else
       info(END, 9, thread_no);
        
        
    pthread_mutex_unlock(&lock);

    return NULL;
}

//• threadul T9.5 trebuie sa inceapa inainte de T9.2 si se termina dupa ce termina el

void *thread_function_49(void* arg)
{
    int thread_no = *(int*)arg;
     sem_wait(&sem);

    info(BEGIN, 5, thread_no);
    info(END, 5, thread_no);
    sem_post(&sem);
    return NULL;
}

//fac threadurile lui 4

void *thread_function_6(void* arg)
{
    int thread_no = *(int*)arg;
    if(thread_no==2)
    {  
        sem_wait(s2);
        info(BEGIN, 4, thread_no);
    }
    else
        info(BEGIN, 4, thread_no);

    // if (thread_no== 3) {
      
    //     info(END, 4, thread_no);
    //       sem_post(s1); 
    //  }
    //  else 
     info(END, 4, thread_no);
    return NULL;
}

int main(){
    init();

    info(BEGIN, 1, 0);
      s1 = sem_open("/s1", O_CREAT, 0644, 0);
    s2 = sem_open("/s2", O_CREAT, 0644, 0);
    pthread_mutex_init(&lock, NULL);
  
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond1,NULL);
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
            sem_close(s1);
    sem_unlink("/s1");
    sem_close(s2);
    sem_unlink("/s2");
    
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