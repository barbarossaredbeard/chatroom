
#include "./server.h"

int server_view(struct s_recv *recv)
{   
    char str[MAXLEN];
    char buf[MAXLEN];
    Linklist L;

    L = clientlink;
    L = L -> next;
    strcpy(buf,"Online:");
    while(L != NULL)
    {
        sprintf(str,"%s---在线  ",L -> name);
        strcat(buf,str);  //不覆盖拷贝
        L = L -> next;
    }
    strcpy((*recv).msg,buf);
    return 0;
}
