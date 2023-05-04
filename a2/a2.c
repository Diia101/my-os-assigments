#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>


int main(){
    init();

    info(BEGIN, 1, 0);

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
                // end
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