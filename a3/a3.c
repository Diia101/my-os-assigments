#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define REQ_PIPE_NAME "REQ PIPE 61586"
#define RESP_PIPE_NAME "RESP PIPE 61586"

#define CONNECTION_REQUEST_MSG "HELLO"
#define CONNECTION_SUCCESS_MSG "SUCCESS"
#define CONNECTION_FAILURE_MSG "ERROR\ncannot create the response pipe | cannot open the request pipe"

int main() {
    // Creează pipe-ul de răspuns
    if (mkfifo(RESP_PIPE_NAME, 0666) == -1) {
        perror("Cannot create the response pipe");
        exit(1);
    }

    // Deschide pipe-ul de cereri în mod citire
    int reqPipe = open(REQ_PIPE_NAME, O_RDONLY);
    if (reqPipe == -1) {
        perror("Cannot open the request pipe");
        exit(1);
    }

    // Deschide pipe-ul de răspuns în mod scriere
    int respPipe = open(RESP_PIPE_NAME, O_WRONLY);
    if (respPipe == -1) {
        perror("Cannot open the response pipe");
        exit(1);
    }

    // Trimite mesajul de cerere de conexiune
    if (write(respPipe, CONNECTION_REQUEST_MSG, strlen(CONNECTION_REQUEST_MSG)) == -1) {
        perror("Failed to write connection request");
        exit(1);
    }

    // Primește mesajul de răspuns
    char response[1024];
    if (read(reqPipe, response, sizeof(response)) == -1) {
        perror("Failed to read connection response");
        exit(1);
    }

    // Verifică mesajul de răspuns și afișează rezultatul corespunzător
    if (strcmp(response, CONNECTION_SUCCESS_MSG) == 0) {
        printf("Connection Successful Message\n");
        printf("%s\n", response);
    } else {
        printf("Connection Failure Message\n");
        printf("%s\n", CONNECTION_FAILURE_MSG);
    }

    // Buclă principală
    while (1) {
        // Citește cererea din pipe-ul de cereri
        char request[1024];
        if (read(reqPipe, request, sizeof(request)) == -1) {
            perror("Failed to read request");
            exit(1);
        }

        // Tratează cererea și obține rezultatul
        // (aici ar trebui adăugat codul pentru tratarea diferitelor tipuri de cereri)

        // Scrie rezultatul în pipe-ul de răspuns
        if (write(respPipe, response, strlen(response)) == -1) {
            perror("Failed to write response");
            exit(1);
        }
    }

    // Închide pipe-urile și eliberează resursele
    close(reqPipe);
    close(respPipe);
    unlink(RESP_PIPE_NAME);

    return 0;
}
