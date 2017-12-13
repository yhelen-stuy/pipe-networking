#include "pipe_networking.h"


int main() {

    int to_client;
    int from_client;
    char buffer[256];
    char *modified;

    while (1) {
        from_client = server_handshake( &to_client );
        if (from_client == -1) {
            printf("Handshake failed\n");
            return 1;
        }
        while (1) {
            read(from_client, buffer, sizeof(buffer));
            if (strcmp(buffer, "\n") == 0) {
                close(to_client);
                close(from_client);
                break;
            }
            printf("%s\n", buffer);
            upper(buffer);
            write(to_client, buffer, sizeof(buffer));
        }
    }
    return 0;
}

unsigned char * upper(char *str) {
    unsigned char *my_str = (unsigned char *) str;
    int i = 0;
    for (; my_str[i]; i++) {
        my_str[i] = toupper(my_str[i]);
    }
    return my_str;
}
