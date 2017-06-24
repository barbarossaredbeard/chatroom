/*****************************************************
copyright (C), NanHang Jincheng College wubeiwen
File name：client_handle.h
Author:WUBEIWEN  Version:0.1    Date: 2017-01-08 21:45
Description：
Funcion List: 
*****************************************************/

#ifndef CLIENT_HANDLE_H
#define CLIENT_HANDLE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 6666
#define MAXLEN 1000
#define NORMAL 50
#define OK 1
#define FAULT 0

struct c_send
{
    char cmd[NORMAL];            
    int account;                 
    char name[NORMAL];       
    char pwd[NORMAL];
    char  pwd_lock[NORMAL];   //密保手机
    char msg[MAXLEN];
    char say_to[NORMAL];      //私聊，禁言等对象
    int sockfd;               
    char filename[NORMAL];    //发送的文件名
    int fd;
    int speak;                //禁言标志位
};

struct c_recv
{
    char cmd[NORMAL];
    int account;
    char succ_or_fail[NORMAL];
    char say_to[NORMAL];
    char from_to[NORMAL];
    char msg[MAXLEN];
    int sockfd;
    char name[NORMAL];
    char forbid_name[NORMAL];
};


int read_finish;          //读线程结束标志
int is_reg_and_login;     //注册登录第一阶段标志
int is_chat;              //登录后的标志
int is_admin;             //管理员标志
int is_get_out;         
int quit;
char localname[NORMAL];  //本地帐号
 
//声明所有函数
extern int read_msg(void *sockfd);
extern int write_msg(void *sockfd);
extern int reg_user(struct c_send*send,int sockfd);
extern int reg_log_face();
extern int chat_face();
extern int admin_face();
extern int send_file(struct c_send*send,int sockfd);
extern int file_accept(struct c_recv*recv);
extern int forget_pwd(struct c_send*send,int sockfd);
extern int log_user(struct c_send*send,int sockfd);
#endif
