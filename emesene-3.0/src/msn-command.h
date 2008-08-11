#ifndef _MSN_COMMAND_H
#define _MSN_COMMAND_H

#define DX_COMMAND_LENGTH 4
#define DX_PARAM_LENGTH 500
#define DX_CMDBUFFER 512

typedef struct {
    char command[DX_COMMAND_LENGTH];
    char params[DX_PARAM_LENGTH];
} MsnCommand;

MsnCommand socket_receive_command(MsnSocket *);
int socket_send_command(MsnSocket *, char[], char[]);

#endif
