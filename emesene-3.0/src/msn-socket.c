
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    printf("Connected to %s\n", host->h_name);

    sock.trid = 0;

    return sock;
}

int socket_send_command(MsnSocket *sock,
                        char command[],
                        char params[]) {
    int len;
    int trid;
    char output[DX_CMDBUFFER];

    sock->trid += 1;
    trid = sock->trid;

    snprintf(output, DX_CMDBUFFER, "%s %d %s\r\n", command, trid, params);
    printf(">>> %s", output);
    
    len = strlen(output);
    if (send(sock->socket, output, len, 0) != len) {
        perror("send");
        return -1;
    }

    return 0;
}

MsnCommand socket_receive_command(MsnSocket *sock) {
    char input[DX_CMDBUFFER];
    char buffer;
    int bytes = 0;
    MsnCommand cmd;

    /*fill input with the incoming data until a \r is found*/
    while (recv(sock->socket, &buffer, 1, 0) != -1 &&
           buffer != '\r' &&
           bytes < DX_CMDBUFFER) {
        input[bytes] = buffer;
        bytes++;
    }

    recv(sock->socket, &buffer, 1, 0); /*discard \n*/

    /*extract command*/
    strncpy(cmd.command, input, 3);
    cmd.command[3] = '\0';
    
    strncat(cmd.params, input + 4, DX_PARAM_LENGTH);

    printf("<<< %s %s", cmd.command, cmd.params);
    
    return cmd;
}


