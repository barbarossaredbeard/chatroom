
#include "./client_handle.h"

int read_msg(void *sockfd)
{   
    int sfd = *((int *)sockfd);
    struct c_recv recv;
    struct c_send send;


    char chat_record[50];
    char str[1000];
    time_t timep;
    int chat_record_fd;//聊天记录用到的5个变量
    int size;

    while(1)
    {   

        if(read(sfd,&recv,sizeof(struct c_recv)) == 0)
		{
			printf("服务器异常退出！\n");
			exit(0);
		}

        if(strcmp(recv.cmd,"Register success!") == 0)     
        {
            printf("成功注册,你的帐号为:%d 请记牢你的帐号!\n",recv.account);//注册成功
            sleep(3);                 //等待，让读线程全部执行完操作
            read_finish = 1;
            is_reg_and_login = 1;
            continue;
        }

        if(strcmp(recv.cmd,"Register failure!") == 0)  //注册失败
        {   
            sleep(3);
            read_finish = 1;
            is_reg_and_login = 1;
            continue;
        }                                                       
       /* 登录 */   
        if(strcmp(recv.cmd,"repeat_log") == 0)    //重新登录
        {
            printf("\n重复登录!\n");
            sleep(3);
            read_finish = 1;
            is_reg_and_login = 1;
            continue;
        }

        if(strcmp(recv.cmd,"Login success!") == 0)// 普通用户登录成功
        {   

            strcpy(localname,recv.name);
            sleep(3);
            read_finish = 1;                    //通知客户端写线程可以继续写命令了
            is_chat = 1;                      //通知客户端写线程可以输出聊天界面了
            continue;
        }
        
        if(strcmp(recv.cmd,"Admin,login success!") == 0)//管理员登录成功
        {
            strcpy(localname,recv.name);//将管理员名字存下
            sleep(3);
            read_finish = 1;
            is_admin = 1;
            continue;
        }

        if(strcmp(recv.cmd,"Login failure!") == 0)//登录失败
        {   
            printf("\n");
            printf("服务器发来 : %s\n",recv.cmd);
            sleep(3);
            read_finish = 1;
            is_reg_and_login = 1;//由于登录失败,通知客户端再次进入登录界面
            continue;
        }


        if(strcmp(recv.cmd,"view success!") == 0)   //显示在线
        {   
            printf("\n%s\n",recv.msg);               //打印在线信息
            sleep(3);
            read_finish = 1;
            is_chat= 1;
            continue;
        }
        
        if(strcmp(recv.cmd,"can't_chat") == 0)      //私聊   通知其不能发送
        {
            printf("\n你已被禁言，无法发送！\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }
 
        if(strcmp(recv.cmd,"can't_all_chat") == 0)  //群聊　　通知其不能发送 
        {
            printf("\n你已被禁言，无法发送！\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"online_no") == 0)       
        {
            printf("\n此人不在线\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }
        
        if(strcmp(recv.cmd,"online_ok") == 0)         // 私聊对象
        {
            printf("\n%s 对你说了悄悄话: %s \n",recv.name,recv.msg);
            sprintf(chat_record,"./chat_record/%s.text",localname);
            chat_record_fd = open(chat_record,O_RDWR|O_CREAT|O_APPEND); //打开并创建chat_recod目录下的文件
            if(chat_record_fd < 0)
            {
                printf("open chat_record is error!\n");
                exit(0);
            }
            time(&timep);
            bzero(str,1000);
            sprintf(str,"\n**时间: %s \n**消息记录: %s 对你说: %s\n===================================",ctime(&timep),recv.name,recv.msg);
            write(chat_record_fd,str,strlen(str));
            close(chat_record_fd);
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"all_success") == 0)        //群聊成功
        {
            printf("\n%s 对大家说: %s \n",recv.name,recv.msg);
            sprintf(chat_record,"./chat_record/%s.text",localname);
            chat_record_fd = open(chat_record,O_RDWR|O_CREAT|O_APPEND);
            if(chat_record_fd < 0)
            {
                printf("open chat_record is error!\n");
                exit(0);
            }
            time(&timep);
            bzero(str,1000);
            sprintf(str,"\n**时间: %s \n**消息记录: %s 对大家说: %s\n===================================",ctime(&timep),recv.name,recv.msg);
            write(chat_record_fd,str,strlen(str));
            close(chat_record_fd);
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }
        
        if(strcmp(recv.cmd,"yourself") == 0)   //群聊时发送着的界面与接受者相同
        {
            printf("\n你对大家说: %s \n",recv.msg);
            sprintf(chat_record,"./chat_record/%s.text",localname);
            chat_record_fd = open(chat_record,O_RDWR|O_CREAT|O_APPEND);
            if(chat_record_fd < 0)
            {
                printf("open chat_record is error!\n");
                exit(0);
            }
            time(&timep);
            bzero(str,1000);
            sprintf(str,"\n**时间: %s \n**消息记录: %s 你对大家说: %s\n===================================",ctime(&timep),recv.name,recv.msg);
            write(chat_record_fd,str,strlen(str));
            close(chat_record_fd);
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"private_success") == 0)   //私聊成功　告知自己
        {
            printf("\n消息发送成功!\n");
            sprintf(chat_record,"./chat_record/%stext",localname);
            chat_record_fd = open(chat_record,O_RDWR|O_CREAT|O_APPEND);
            if(chat_record_fd < 0)
            {
                printf("open chat_record is error!\n");
                exit(0);
            }
            time(&timep);
            bzero(str,1000);
            sprintf(str,"\n**时间: %s \n**消息记录: 你对%s 说 :%s\n=================================== ",ctime(&timep),recv.name,recv.msg);
            write(chat_record_fd,str,strlen(str));
            close(chat_record_fd);
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"is_get_out") == 0)   //告知目标
        {   
            printf("\n你即将被管理员踢下线!\n");
            strcpy(send.cmd,"game over");
            strcpy(send.name,localname);
            write(sfd,&send,sizeof(struct c_send));
            sleep(3);
            exit(0);
        }

        if(strcmp(recv.cmd,"get_out_success") == 0) //告知管理员
        {
            printf("\n成功将%s 踢下线！\n",recv.name);
            sleep(3);
            read_finish = 1;
            is_admin =1;
            continue;
        }

        if(strcmp(recv.cmd,"be forbided") == 0)
        {
            printf("\n你已被禁言!\n");
            sleep(3);
            read_finish = 1;
            is_chat =1;
            continue;
        }

        if(strcmp(recv.cmd,"forbid success") == 0)
        {
            printf("\n成功将%s 禁言!\n",recv.name);
            sleep(3);
            read_finish = 1;
            is_admin =1;
            continue;
        }

        if(strcmp(recv.cmd,"no_such_p") == 0) //告知管理员
        {
            printf("\n没有找到此人！\n");
            sleep(3);
            read_finish = 1;
            is_admin =1;
            continue;
        }

        if(strcmp(recv.cmd,"close be forbided") == 0)
        {
            printf("\n你已被解禁\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"close forbid success") == 0)
        {
            printf("\n成功将%s 解禁!\n",recv.name);
            sleep(3);
            read_finish = 1;
            is_admin =1;
            continue;
        }

        if(strcmp(recv.cmd,"you can't recv") == 0)  //群聊，被禁言的无法收到消息
        {
            printf("\n%s发来一条消息，您已被禁言，无法查看!\n",recv.name);
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"no_find") == 0)         //找回密码
        {
            printf("\n没有查找到此用户或密保手机错误！！！\n");
            sleep(3);
            read_finish = 1;
            is_reg_and_login = 1;
            continue;
        }

        if(strcmp(recv.cmd,"find") == 0)   
        {
            printf("\n请记住此密码!!\n");
            printf("%s\n",recv.msg);
            sleep(3);
            read_finish = 1;
            is_reg_and_login = 1;
            continue;
        }

        if(strcmp(recv.cmd,"will_file") == 0)   //告知即将有文件传送
        {
            printf("\n%s 向你发送了一个文件!\n",recv.name);
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"not_exist") == 0)  //文件接受者不在线或不存在
        {
            printf("\n此人不在线或不存在!\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"accept_success") == 0)  //接受者成功接受
        {
            file_accept(&recv); 
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"send_success") == 0)  //发送着成功发送
        {
            printf("\n对方成功接收文件!\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }

        if(strcmp(recv.cmd,"no_send") == 0)   //告知发送者
        {
            printf("\n对方拒绝接受你的文件！\n");
            sleep(3);
            read_finish = 1;
            is_chat = 1;
            continue;
        }
    }
}
