
#include "./client_handle.h"

int write_msg(void *sockfd)
{
    int sfd = *((int*)sockfd);
    int do_reg_log;
    int do_chat;
    int do_admin;

    struct c_send send;

    char chat_record[50];
    int  chat_record_fd;   //聊天记录文件描述符
    char str[1000];        //存聊天记录

    int size;

    
    read_finish = 1;
    is_reg_and_login = 1;
    while(1)
    {
        while(read_finish)      
        {
            if(is_reg_and_login)
            {
                do_reg_log = reg_log_face();

                switch(do_reg_log)
                {   
                    case 1://登录
                        {
                            read_finish = 0;                  
                            is_reg_and_login = 0;     
                            log_user(&send,sfd);
                            break;
                        }

                    case 2://注册
                        {
                            read_finish = 0;
                            is_reg_and_login = 0; //此步将客户端写线程停下来,让客户端读线程处理事情，防止输出混乱
                            reg_user(&send,sfd);
                            break;
                        }

                    case 3://找回密码
                        {   
                            read_finish = 0;
                            is_reg_and_login = 0;
                            
                            forget_pwd(&send,sfd);
                            break;
                        }

                    case 0://退出
                        {
                            printf("离开聊天室...\n");
                            strcpy(send.cmd,"quit_first");   //退出必须发送命令，让服务器结束其线程，否则混乱
                            write(sfd,&send,sizeof(struct c_send));
                            exit(0);
                        }

                    default:
                        {
                            break;
                        }
                }
            } //第一个if
            if(is_admin)
            {
                do_admin = admin_face();
                switch(do_admin)
                {
                    case 1:{
                               read_finish = 0;
                               is_admin = 0;
                               printf("请输入想踢出的用户名:\n");
                               scanf("%s",send.say_to);
                               strcpy(send.cmd,"get_out");         //命令
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 2:{        //禁言
                               read_finish = 0;
                               is_admin = 0;
                               printf("请输入想禁言的用户名:\n");
                               scanf("%s",send.say_to);
                               strcpy(send.cmd,"no_say");
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 3:{
                               read_finish = 0;
                               is_admin = 0;
                               printf("请输入想解禁的用户名:\n");
                               scanf("%s",send.say_to);
                               strcpy(send.cmd,"close_forbid");
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 0:{
                               printf("正在离开聊天室>>>>>>\n");
                               sleep(1);
                               strcpy(send.cmd,"quit_admin");
                               strcpy(send.name,localname);
                               write(sfd,&send,sizeof(struct c_send));
                               exit(0);
                           }

                    default:break;
                }
            }

            if(is_chat)                   //登录后
            {
                do_chat = chat_face();    //登陆后界面
                switch(do_chat)
                {
                    case 1:{ //群聊
                               read_finish = 0;            
                               is_chat = 0;                               //停下写进程 等读线程信号再运作
                               strcpy(send.cmd,"all_chat");
                               strcpy(send.name,localname);
                               printf("请输入想要说的话：\n");
                               scanf("%s",send.msg);
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 2:{   //私聊
                               read_finish = 0;
                               is_chat = 0;
                               strcpy(send.cmd,"private_chat");
                               strcpy(send.name,localname);
                               printf("请输入私聊对象：\n");
                               scanf("%s",send.say_to);
                               printf("请输入想要说的话：\n");
                               scanf("%s",send.msg);
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 3:{  //在线查找
                               read_finish = 0;
                               is_chat = 0;
                               strcpy(send.cmd,"display_list");
                               strcpy(send.name,localname);
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 4:{   //发送文件
                               read_finish = 0;
                               is_chat = 0;

                               if(send_file(&send,sfd) == 0)
                               {
                                   read_finish = 1;
                                   is_chat =1;
                               }
                               break;
                           }
                           
                    case 5:{  //接受文件　
                               read_finish = 0;
                               is_chat = 0;
                               strcpy(send.name,localname);
                               strcpy(send.cmd,"recv_file");
                               write(sfd,&send,sizeof(struct c_send));
                               break;
                           }

                    case 6:{   //拒绝接受文件
                               read_finish = 0;
                               is_chat = 0;
                               strcpy(send.name,localname);
                               strcpy(send.cmd,"disagree_file");
                               write(sfd,&send,sizeof(struct c_send));
                               sleep(3);
                               read_finish = 1;
                               is_chat = 1;
                               break;
                           }

                    case 7:{   //聊天记录查找
                               read_finish = 0;
                               is_chat = 0;
                               sprintf(chat_record,"./chat_record/%s.text",localname);
                               chat_record_fd = open(chat_record,O_RDONLY);
                               if(chat_record_fd < 0)
                               {
                                   printf("聊天记录打开失败!\n");
                                   exit(0);
                               }
                               while(size = read(chat_record_fd,str,sizeof(str)))
                               {
                                   if(size == -1)break;
                               }

                               close(chat_record_fd);
                               printf("%s\n",str);
                               sleep(2);
                               read_finish = 1;
                               is_chat = 1;
                               break;
                           }
                           
                    case 0:{
                               printf("正在离开聊天室>>>>>>>>\n");
                               sleep(2);
                               strcpy(send.name,localname);
                               strcpy(send.cmd,"quit_log");
                               write(sfd,&send,sizeof(struct c_send));
                               exit(0);
                           }

                    default:break;
                }
            }
        }
    }
}
