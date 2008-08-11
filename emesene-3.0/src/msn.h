
#ifndef _DX_MSN_H
#define _DX_MSN_H

#include "msn-socket.h"

#define DX_MAIL_LENGTH 100
#define DX_PASS_LENGTH 24

typedef struct {
    char username[DX_MAIL_LENGTH];
    char password[DX_PASS_LENGTH];
} MsnSession;

int msn_init(MsnSession *, int, char **);
int msn_login(MsnSession *);
int msn_login_sso(char *);
int presentation(MsnSession *, MsnSocket *);

#endif  /* _DX_MSN_H */
