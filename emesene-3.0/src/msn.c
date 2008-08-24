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
    
    msn_socket = dx_socket_new("messenger.hotmail.com", 1863);
    
    if (!presentation(msn, &msn_socket)) return false;
    return true;
}

int presentation(MsnSession *msn, MsnSocket *msn_socket) {
    char cmd[CMDBUF];
    char tmp[CMDBUF];

    
    socket_send_command(msn_socket, "VER", "MSNP15 CVR0");

    socket_receive_command(msn_socket, cmd);
    if (!dx_match_command(cmd, 3, "VER", NULL, "MSNP15")) return false;
    
    snprintf(tmp, CMDBUF, "0x0c0a winnt 5.1 i386 MSNMSGR 8.1.0178 msmsgs %s",
        msn->username);
    socket_send_command(msn_socket, "CVR", tmp);
    
    socket_receive_command(msn_socket, cmd);
    if (!dx_match_command(cmd, 1, "CVR")) return false;
    
    snprintf(tmp, 256, "SSO I %s", msn->username);
    socket_send_command(msn_socket, "USR", tmp);

    socket_receive_command(msn_socket, cmd);

    if (dx_match_command(cmd, 5, "XFR", NULL, "NS", NULL, "U")) {
        /* redirect */

        int i, port;
        char *address = dx_get_param(cmd, 3);

        for (i=0; address[i] != ':'; i++); /*nothing here :D*/
        address[i] = '\0'; /*terminate the hostname*/
        port = atoi(address + i+1); /*parse port number*/
        
        socket_close(msn_socket);
        *msn_socket = dx_socket_new(address, port);

        return presentation(msn, msn_socket);

    } else if (dx_match_command(cmd, 1, "USR", NULL, "SSO", "S")) {
        if (!sso_auth(cmd)) return false;
    }
        
    return true;
}


int sso_auth(char *cmd) {
    return true;
}

