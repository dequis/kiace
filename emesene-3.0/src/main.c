
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "msn.h"

int main(int argc, char **argv) {
    MsnSession msn;
    /*GtkWindow window;*/
    
    if (!msn_init(&msn, argc, argv)) abort();

    strncat(msn.username, "pr0n@dxzone.com.ar", DX_MAIL_LENGTH);
    strncat(msn.password, "WINDOWS_L33T", DX_PASS_LENGTH);

    if (!msn_login(&msn)) abort();

    /*window = dx_build_main(msn);*/

    return 0;
}

