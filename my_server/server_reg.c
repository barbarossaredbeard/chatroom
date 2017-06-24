
#include "./server.h"

int server_reg(struct s_recv *a)
{
    int ok;
    char buf[MAXLEN];
    ok = reg_check(a);

    if(ok == 1)
    {
        return OK;
    }
    else
    {
        return FAULT;
    }
}


