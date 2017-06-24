
#include "./server.h"

int get_out(struct s_send * send,struct s_recv * recv)  //踢人
{
    Linklist my_head;
    my_head = clientlink;
    my_head = my_head->next;
    struct online who_online;

    strcpy(who_online.name,recv->say_to);
    strcpy(send->name,recv->say_to);

    while(my_head != NULL)
    {
        if(strcmp(who_online.name,my_head->name) == 0) //找到此人套接字
        {
            server_quit(&who_online);
            strcpy(send->cmd,"is_get_out");
            write(my_head->sockfd,send,sizeof(struct s_send));//通知其被踢

            bzero(send->cmd,sizeof(send->cmd));
            strcpy(send->cmd,"get_out_success");
            write(recv->sockfd,send,sizeof(struct s_send));//通知超级用户成功踢人
            break;
        }
        my_head = my_head->next;
    }

    if(my_head == NULL) //没有找到此人
    {
        strcpy(send->cmd,"no_such_p");
        write(recv->sockfd,send,sizeof(struct s_send)); //通知超级用户此人不在线或不存在
    }
    return 0;
}

int forbid_name(struct s_send *send,struct s_recv*recv)//禁言
{
    Linklist my_head;
    my_head = clientlink;
    my_head = my_head->next;

    strcpy(send->name,recv->say_to);

    while(my_head != NULL)
    {
        if(strcmp(my_head->name,recv->say_to) == 0)
        {
            strcpy(send->cmd,"forbid success"); 
            my_head->speak = 0;
            write(recv->sockfd,send,sizeof(struct s_send));//通知超级用户禁言成功
            bzero(send->cmd,sizeof(send->cmd));
            strcpy(send->cmd,"be forbided");
            write(my_head->sockfd,send,sizeof(struct s_send));//通知目标被禁言
            break;
        } 
        my_head = my_head->next;
    }
    if(my_head == NULL)
    {
        strcpy(send->cmd,"no_such_p");
        write(recv->sockfd,send,sizeof(struct s_send));
    }
    return 0;
}

int close_forbid(struct s_send *send,struct s_recv*recv)//解除禁言
{
    Linklist my_head;
    my_head = clientlink;
    my_head = my_head->next;

    strcpy(send->name,recv->say_to);

    while(my_head != NULL)
    {
        if(strcmp(my_head->name,recv->say_to) == 0)
        {
            strcpy(send->cmd,"close forbid success");
            my_head->speak = 1;//禁言标志更改
            write(recv->sockfd,send,sizeof(struct s_send));
            bzero(send->cmd,sizeof(send->cmd));
            strcpy(send->cmd,"close be forbided");
            write(my_head->sockfd,send,sizeof(struct s_send));
            break;
        } 
        my_head = my_head->next;
    }
    if(my_head == NULL)
    {
        strcpy(send->cmd,"no_such_p");
        write(recv->sockfd,send,sizeof(struct s_send));
    }
    return 0;
}

