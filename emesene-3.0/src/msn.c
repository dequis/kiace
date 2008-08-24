#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "msn.h"
#include "msn-socket.h"
#include "param-parser.h"

int presentation(MsnSession *, MsnSocket *);
int sso_auth(char *);

int msn_init(MsnSession *msn, int argc, char *argv[]) {
    memset(msn, 0, sizeof(MsnSession));
    /*here i should parse the args*/
    return true;
}

int msn_login(MsnSession *msn) {
    MsnSocket msn_socket;
    
    if (!presentation(msn, &msn_socket)) return false;
    return true;
}

int presentation(MsnSession *msn, MsnSocket *msn_socket) {
    MsnCommand cmd;
    char tmp[256];

    *msn_socket = dx_socket_new("messenger.hotmail.com", 1863);
    
    socket_send_command(msn_socket, "VER", "MSNP15 CVR0");
    
    cmd = socket_receive_command(msn_socket);
    
    if (strncmp("VER", cmd.command, 3) ||
        strncmp("MSNP13", cmd.params, 6)) return false;
    
    snprintf(tmp, 256, "0x0c0a winnt 5.1 i386 MSNMSGR 8.1.0178 msmsgs %s",
        msn->username);
    socket_send_command(msn_socket, "CVR", tmp);
    
    cmd = socket_receive_command(msn_socket);
    if (strncmp("CVR", cmd.command, 3)) return false;
    
    snprintf(tmp, 256, "SSO I %s", msn->username);
    socket_send_command(msn_socket, "USR", tmp);

    cmd = socket_receive_command(msn_socket);
    if (strncmp("USR", cmd.command, 3) ||
        strncmp("SSO S", cmd.params, 5)) return false;

    if (!sso_auth(cmd.params)) return false;

    return true;
}


int sso_auth(char *hash) {
    return true;
}

