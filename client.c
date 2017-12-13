#include "pipe_networking.h"


int main() {

    int to_server;
    int from_server;
    char buffer[256];

    from_server = client_handshake( &to_server );

    if (from_server == -1) {
        printf("Handshake failed\n");
        return 1;
    }

    while (1) {
        printf("If you would like to quit, press Enter\n");
        printf("Enter text: ");
        fgets(buffer, sizeof(buffer), stdin);
        write(to_server, buffer, sizeof(buffer));
        if (strcmp(buffer, "\n") == 0) {
            break;
        }
        read(from_server, buffer, sizeof(buffer));
        printf("Modified text: %s\n", buffer);
        buffer[0] = 0;
    }
    close(to_server);
    close(from_server);
    return 0;
}
