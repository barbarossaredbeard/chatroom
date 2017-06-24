
#include "./server.h"

int server_deal(void *sfd)
{   
    int sockfd = *((int*)sfd);//每个客户端对应的sockfd
    int who_login;

    struct s_send send;
    struct s_recv recv;

    struct online who_online;
    
    while(1)
    {   
      //  read(sockfd,&recv,sizeof(struct s_recv));   //读来自客户断的消息
		 if(read(sockfd,&recv,sizeof(struct s_recv)) == 0)   //读来自客户断的消息
		{
			printf("%s异常离线\n",recv.name);
			strcpy(who_online.name,recv.name);
            who_online.sockfd = sockfd;
            who_online.account = recv.account;//       离线成功的把登录信息从链表删除
            server_quit(&who_online);
            pthread_exit((void*)2);
		}


        if(strcmp(recv.cmd,"quit_first") == 0)   //第一次退出处理，并退出服务器线程
        {
            pthread_exit((void*)2);
        }
        
        if(strcmp(recv.cmd,"quit_log") == 0)//第二次退出处理,并退出服务器线程
        {
            printf("%s 离线成功!\n",recv.name);//打印离线帐号

            strcpy(who_online.name,recv.name);
            who_online.sockfd = sockfd;
            who_online.account = recv.account;//离线成功的把登录信息从链表删除
            server_quit(&who_online);
            pthread_exit((void*)2);

        }
        /*管理员*/
        if(strcmp(recv.cmd,"quit_admin") == 0)//管理员离线,并退出所在线程
        {
            printf("管理员 %s 离线成功!\n",recv.name);//打印离线帐号

            strcpy(who_online.name,recv.name);
            who_online.sockfd = sockfd;
            who_online.account = recv.account;//离线成功的把登录信息从链表删除
            
            server_quit(&who_online);
            sleep(2);
            pthread_exit((void*)2);

        }

        if(strcmp(recv.cmd,"game over") == 0)//重点，被踢后必须结束对应线程
        {
            printf("%s 被踢下线\n",recv.name);
            pthread_exit((void*)2);
        }

        if(strcmp(recv.cmd,"reg") == 0)//服务器注册处理
        {          
            if(server_reg(&recv))
            {
                printf("%s 注册成功!\n",recv.name);//注册成功
                send.account = recv.account; //把生成的帐号发送给客户端
                strcpy(send.cmd,"Register success!");
                write(sockfd,&send,sizeof(struct s_send));
            }
            else
            {   
                printf("%s 注册失败!\n",recv.name);//注册失败
                strcpy(send.cmd,"Register failure!");
                write(sockfd,&send,sizeof(struct s_send));
            }
            continue;
        }

        if(strcmp(recv.cmd,"login") == 0)   //登录处理
        {    
            who_login =  server_login(&recv);

            if(who_login == REPEAT_LOG)
            {
                printf("重复登录!\n");
                strcpy(send.cmd,"repeat_log");
                write(sockfd,&send,sizeof(struct s_send));
                continue;
            }
            
            else if(who_login == LOG_USER)//普通用户登录成功
            {   
                strcpy(who_online.name,recv.name);
                who_online.sockfd = sockfd;
                who_online.account = recv.account;//登录成功的把登录信息记录到链表
                who_online.speak = 1;//默认不被禁言
                inserthead(clientlink,&who_online);//登录成功插入链表
                printf("%s 登录成功!\n",recv.name);
                strcpy(send.name,recv.name);//把取出的用户名传给用户
                strcpy(send.cmd,"Login success!");
                write(sockfd,&send,sizeof(struct s_send));
                continue;
            }
            else if(who_login == LOG_ADMIN)//管理员登录成功
            {   
                strcpy(who_online.name,recv.name);
               // printf("who_online.name %s\n",who_online.name);
                who_online.sockfd = sockfd;
                who_online.account = recv.account;//登录成功的把登录信息记录到链表          
                who_online.speak = 1;
                inserthead(clientlink,&who_online);
               // displaylist(clientlink);
                printf("管理员 %s 登录成功!\n",recv.name);
                strcpy(send.name,recv.name);//把管理员名字取出,并发给管理员客户端          

                strcpy(send.cmd,"Admin,login success!");
                write(sockfd,&send,sizeof(struct s_send));
                continue;
            }
            else//登录失败
            {
                strcpy(send.cmd,"Login failure!");//登录失败返回登录失败信息
                printf("%d 登录失败!\n",recv.account);
                write(sockfd,&send,sizeof(struct s_send));
                continue;
            }
        }
        
        if(strcmp(recv.cmd,"forget pwd") == 0)    //忘记密码
        {
            printf("%d 请求找回密码\n",recv.account);
            forget_pwd(&recv,&send);     //调用函数查找
            write(sockfd,&send,sizeof(struct s_send));
            continue;
        }


        if(strcmp(recv.cmd,"display_list") == 0)//服务器处理在线人员查看
        {   
            printf("%s 请求查看在线用户\n",recv.name);
            server_view(&recv);//在线人员处理函数
            strcpy(send.cmd,"view success!");//处理完通知客户端
            strcpy(send.msg,recv.msg);
            write(sockfd,&send,sizeof(struct s_send)); 
            continue;
        }

        if(strcmp(recv.cmd,"private_chat") == 0) //私聊
        {   
            printf("%s 请求向%s 聊天\n",recv.name,recv.say_to);
            recv.sockfd = sockfd;
            server_private_chat(&recv,&send);//私聊函数
            continue;
        }

        if(strcmp(recv.cmd,"all_chat") == 0) //群聊
        {
            printf("%s 发起了群聊\n",recv.name);
            recv.sockfd = sockfd;
            server_all_chat(&recv,&send);
            continue;
        }

        if(strcmp(recv.cmd,"get_out") == 0) //踢人
        {
            printf("超级用户踢人\n");
            recv.sockfd = sockfd;
            get_out(&send,&recv,&who_online);//从链表中删除
            continue;
        }

        if(strcmp(recv.cmd,"no_say") == 0)  //禁言
        {
            printf("超级用户禁言\n");
            recv.sockfd = sockfd;
            forbid_name(&send,&recv); //查找此人
            continue;
        }

        if(strcmp(recv.cmd,"close_forbid") == 0) //解禁
        {
            printf("超级用户解除禁言!\n");
            recv.sockfd = sockfd;
            close_forbid(&send,&recv);    
            continue;
        }

        if(strcmp(recv.cmd,"will_send") == 0) //文件发送到服务器保存
        {
            printf("%s 请求传送文件给%s !\n",recv.name,recv.say_to);
            file_from = sockfd;//保存发送者套接字
            strcpy(msgbuf,recv.msg);//保存文件内容
            strcpy(file_to,recv.say_to);
            send_file(&send,&recv);
            continue;
        }

        if(strcmp(recv.cmd,"recv_file") == 0) //对方接受文件
        {
            recv.sockfd = sockfd;
            accept_file(&send,&recv);
            continue;
        }

        if(strcmp(recv.cmd,"disagree_file") == 0) //不接受
        {
            strcpy(send.cmd,"no_send");//发送命令
            write(file_from,&send,sizeof(struct s_send));
            continue;
        }
    }
}
