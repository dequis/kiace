
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "msn.h"
#include "msn-socket.h"


MsnSocket dx_socket_new(char hostname[], int port) {
    MsnSocket sock;
    struct hostent *host;

    printf("dx_socket_new: %s:%d...\n", hostname, port);

    if ((sock.socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("dx_socket_new");
        abort();
    }

    if ((host = gethostbyname(hostname)) == NULL) {
        perror("dx_socket_new");
        abort();
    }

    
    memset(&sock.address, 0, sizeof(sock.address));
    sock.address.sin_family = AF_INET;
    sock.address.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
    sock.address.sin_port = htons(port);

    if (connect(sock.socket,
                (struct sockaddr *) &sock.address, sizeof(sock.address)) < 0) {
        perror("dx_socket_new");
        abort();
    }

    sock.trid = 0;

    return sock;
}

void socket_close(MsnSocket *sock) {
    close(sock->socket);
    sock->socket = 0;
}

int socket_send_command(MsnSocket *sock, char *command, char *params) {
    int len;
    int trid;
    char output[CMDBUF];

    sock->trid += 1;
    trid = sock->trid;

    snprintf(output, CMDBUF, "%s %d %s\r\n", command, trid, params);
    printf(">>> %s", output);
    
    len = strlen(output);
    if (send(sock->socket, output, len, 0) != len) {
        perror("send");
        return false;
    }

    return true;
}

int socket_receive_command(MsnSocket *sock, char *input) {
    char buffer;   /*single char*/
    int bytes = 0;

    /*fill input with the incoming data until a \r is found*/
    while (1) {
        if (recv(sock->socket, &buffer, 1, 0) == -1) {
            perror("recv");
            return false;
        }
        if (buffer == '\r' || bytes >= CMDBUF) {
            break;
        } 
        input[bytes++] = buffer;
    }

    recv(sock->socket, &buffer, 1, 0); /*discard \n*/
    printf("<<< %s\n", input);
    return true;
}


