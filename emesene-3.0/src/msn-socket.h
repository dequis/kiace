
#ifndef _DX_MSN_SOCKET_H
#define _DX_MSN_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct {
    struct sockaddr_in address;
    int socket;
    int trid;
} MsnSocket;

MsnSocket dx_socket_new(char[], int);

#include "msn-command.h"

#endif  /* _DX_MSN_SOCKET_H */

