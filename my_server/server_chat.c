
#include "./server.h"

int server_private_chat(struct s_recv*recv,struct s_send*send)
{
    bzero(send,sizeof(struct s_send));
    int result;
    result = check_forbid(recv);//私聊  先判断自己是否被禁言
    strcpy(send->msg,recv->msg);
    strcpy(send->name,recv->name);
    if(result == 1)   //没有被禁言，可以私聊
    {
        Linklist my_head;
        my_head = clientlink;
        my_head = my_head->next;
        while(my_head != NULL)  //找出私聊对象
        {
            if(strcmp(my_head->name,recv->say_to) == 0)
            {
                strcpy(send->cmd,"private_success");
                write(recv->sockfd,send,sizeof(struct s_send)); //发送给对象
                bzero(send->cmd,sizeof(send->cmd));
                strcpy(send->cmd,"online_ok");
                write(my_head->sockfd,send,sizeof(struct s_send));//发送给自己，私聊成功
                return 0;
            }
        
            my_head = my_head->next;
        }
        if(my_head ==  NULL)
        {
            printf("此人不在线......\n");
            strcpy(send->cmd,"online_no");
            write(recv->sockfd,send,sizeof(struct s_send));
            return 0;
        }
    }
    else //不能私聊，自己被禁言
    {
        strcpy(send->cmd,"can't_chat");
        write(recv->sockfd,send,sizeof(struct s_send));
        return 0;
    }
}

int server_all_chat(struct s_recv *recv,struct s_send*send)
{
    int result;
    result = check_forbid(recv);  //群聊   判断自己是否被禁言  
    strcpy(send->msg,recv->msg);
    strcpy(send->name,recv->name);
    if(result == 1)    //没有被禁言
    {
        Linklist my_head;
        my_head = clientlink;
        my_head = my_head->next;
        while(my_head != NULL)
        {
            if(my_head->speak == 1) //找出没有被禁言的人，群聊可以接受消息
            {
                if((recv->sockfd != my_head->sockfd)&&(strcmp(my_head->name,"Admin") != 0)) //自己和管理员不接受消息
                {
                    strcpy(send->cmd,"all_success");
                    write(my_head->sockfd,send,sizeof(struct s_send));
                }
                else 
                {
                    strcpy(send->cmd,"yourself"); //群聊时，自己的界面消息
                    write(recv->sockfd,send,sizeof(struct s_send));
                }
            }
            else  //被禁言的人不能接受群聊消息
            {
                strcpy(send->cmd,"you can't recv");
                write(my_head->sockfd,send,sizeof(struct s_send));
            }
            my_head = my_head->next;
        }
    }
    else   //自己被禁言，不能群聊 
    {
        strcpy(send->cmd,"can't_all_chat");
        write(recv->sockfd,send,sizeof(struct s_send));
    }
    return 0;
}
