
#include "server.h"

int server_login(struct s_recv *recv )
{  
    int who_login;

    who_login = login_check(recv);//服务器检查谁登录

    if(who_login == LOG_ADMIN)//检查到管理员登录
    {
        return LOG_ADMIN;
    }
    else
    {
        if(who_login == LOG_USER)//检查到普通用户登录
        {
            return LOG_USER;       
        }
        else
        {
            if(who_login == LOG_FAULT)
            {
                return LOG_FAULT;    //登录失败
            }
            else
                return REPEAT_LOG;
        }
    }
}

int server_quit(Linklist client)//服务器下线处理,从链表删除节点
{
    Linklist my_head;
    my_head = clientlink;
    Linklist p1 = NULL;
    Linklist p2 = NULL;

    p1  = my_head -> next;
    p2  = my_head;

    while((p1 != NULL)&&(strcmp(p1->name,client->name) != 0)) 
    {
        p2 = p1;
        p1 = p1->next;
    }
    p2->next = p1->next;


    //free(tmp1);
    return 0;
}
