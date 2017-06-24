
#include "./server.h"

int send_file(struct s_send *send,struct s_recv*recv)
{
    Linklist my_head;
    my_head = clientlink;
    my_head = my_head -> next;
    strcpy(send->name,recv->name);
    while(my_head != NULL)              //找出文件发送目标的套接字
    {   
        if(strcmp(my_head->name,file_to) == 0)
        {
            strcpy(send->cmd,"will_file");
            write(my_head->sockfd,send,sizeof(struct s_send)); //通知其有人发送文件
            break;
        }
        my_head = my_head -> next;
    }
    if(my_head == NULL)
    {
        strcpy(send->cmd,"not_exist");
        write(file_from,send,sizeof(struct s_send));
    }
    return 0;
}


int accept_file(struct s_send*send,struct s_recv *recv)
{
    strcpy(send->msg,msgbuf);
    strcpy(send->cmd,"accept_success");  
    write(recv->sockfd,send,sizeof(struct s_send)); //通知目标，接受成功

    bzero(send->cmd,sizeof(send->cmd));
    strcpy(send->cmd,"send_success");
    write(file_from,send,sizeof(struct s_send));//通知发送者，发送成功，已接收
    return 0;
}
