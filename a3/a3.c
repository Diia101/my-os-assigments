#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define RESP_PIPE_NAME "RESP_PIPE_61586"
#define REQ_PIPE_NAME "REQ_PIPE_61586"

int main() {
    // Pasul 1: Creează pipe-ul de răspuns
    if (mkfifo(RESP_PIPE_NAME, 0666) == -1) {
        perror("ERROR: cannot create the response pipe");
        exit(1);
    }

    // Pasul 2: Deschide pipe-ul de cerere în modul citire
    int reqPipe = open(REQ_PIPE_NAME, O_RDONLY);
    if (reqPipe == -1) {
        perror("ERROR: cannot open the request pipe");
        exit(1);
    }

    // Pasul 3: Deschide pipe-ul de răspuns în modul scriere
    int respPipe = open(RESP_PIPE_NAME, O_WRONLY);
    if (respPipe == -1) {
        perror("ERROR: cannot open the response pipe");
        exit(1);
    }

    // Pasul 4: Scrie mesajul de cerere în pipe-ul de răspuns
    const char *requestMessage = "HELLO!";
    if (write(respPipe, requestMessage, strlen(requestMessage) + 1) == -1) {
        perror("ERROR: cannot write to the response pipe");
        exit(1);
    }
printf("SUCCES\n");

    // Citeste raspunsul din pipe-ul de cerere
    char responseMessage[256];
    ssize_t bytesRead = read(reqPipe, responseMessage, sizeof(responseMessage));
    if (bytesRead == -1) {
        perror("ERROR: cannot read from the request pipe");
        exit(1);
    }

    // Verifică dacă s-a primit răspunsul complet
    if (bytesRead > 0) {
        responseMessage[bytesRead] = '\0';  // Asigură terminarea șirului de caractere

        // Afiseaza raspunsul
        printf("%s\n", responseMessage);
    }


    // Închide pipe-urile
    close(reqPipe);
    close(respPipe);

    return 0;
}
