#include "pipe_networking.h"


/*=========================
  server_handshake
    args: int * to_client

    Performs the client side pipe 3 way handshake.
    Sets *to_client to the file descriptor to the downstream pipe.

    returns the file descriptor for the upstream pipe.
=========================*/
int server_handshake(int *to_client) {
    mkfifo("WKP", 0644);
    printf("[server] wkp\n");
    int wkp = open("WKP", O_RDONLY);
    if (wkp == -1) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    printf("[server] opened wkp\n");
    char pipe_name[64];
    read(wkp, &pipe_name, sizeof(pipe_name));
    printf("[server] pipe_name: %s\n", pipe_name);
    remove("WKP");
    printf("[server] WKP removed\n");

    *to_client = open(pipe_name, O_WRONLY, 0644);
    int msg = 0;
    printf("[server] writing %d\n", msg);
    write(*to_client, &msg, sizeof(int));

    int res = -1;
    read(wkp, &res, sizeof(int));
    if (res == -1) {
        printf("Error: client message not received\n");
        return -1;
    }
    printf("[server] read %d\n", res);
    return wkp;
}


/*=========================
  client_handshake
    args: int * to_server

    Performs the client side pipe 3 way handshake.
    Sets *to_server to the file descriptor for the upstream pipe.

    returns the file descriptor for the downstream pipe.
=========================*/
int client_handshake(int *to_server) {
    int pid = getpid();
    char pipe_name[64];
    sprintf(pipe_name, "pipe_%d", pid);
    mkfifo(pipe_name, 0644);
    printf("[client] made pipe %s\n", pipe_name);

    *to_server = open("WKP", O_WRONLY, 0644);
    if (*to_server == -1) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    printf("[client] opened wkp\n");
    printf("[client] writing %s\n", pipe_name);
    write(*to_server, &pipe_name, sizeof(pipe_name));

    int priv = open(pipe_name, O_RDONLY, 0644);
    if (priv == -1) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    printf("[client] opened pipe %s\n", pipe_name);

    int res = -1;
    read(priv, &res, sizeof(int));
    if (res == -1) {
        printf("Error: server message not received\n");
        return -1;
    }
    printf("[client] read %d\n", res);
    remove(pipe_name);
    printf("[client] removed %s\n", pipe_name);

    int msg = 0;
    printf("[client] writing %d\n", msg);
    write(*to_server, &msg, sizeof(int));
    return priv;
}
