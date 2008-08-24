
#ifndef _DX_MSN_SOCKET_H
#define _DX_MSN_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#define CMDBUF 512

typedef struct {
    struct sockaddr_in address;
    int socket;
    int trid;
} MsnSocket;

MsnSocket dx_socket_new(char *, int);
int socket_receive_command(MsnSocket *, char *);
int socket_send_command(MsnSocket *, char *, char *);
void socket_close(MsnSocket *);

#endif  /* _DX_MSN_SOCKET_H */

