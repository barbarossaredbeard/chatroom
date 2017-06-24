/*****************************************************
copyright (C), NanHang Jincheng College
File name：server.h
Author:zhaopeng  Version:0.1    Date: 2017-01-08 21:51
Description：
Funcion List: 
*****************************************************/

#ifndef SERVER_H
#define SERVER_H

#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sqlite3.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define portnumber 6666
#define MAXLEN 1000
#define MAXLINE 80
#define NORMAL 50  
#define OK 1
#define FAULT 0           

#define LOG_USER 1                
#define LOG_ADMIN 2                  
#define LOG_FAULT -1
#define REPEAT_LOG 0   //4个登录返回值整形

struct s_recv
{
    char cmd[NORMAL];
    int account;
    char name[NORMAL];
    char pwd[NORMAL];
    char pwd_lock[NORMAL];
    char msg[MAXLEN];
    char say_to[NORMAL];
    int sockfd;
    char filename[NORMAL];
    int fd;
    int speak;      //禁言标志
};

struct online       //在线人员
{   
    int account;
    char name[NORMAL];
    int sockfd;
    int speak;
    struct online *next;
};

typedef struct online *Linklist;
Linklist clientlink;

struct s_send
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

char msgbuf[MAXLEN];  //传送文件时服务器的文件缓冲区
char file_to[NORMAL]; //文件接受对象缓冲区
int  file_from;//用来存储文件发送者的套接字
char localname[NORMAL]; 

extern int server_deal(void *sfd);
extern Linklist CreateLinkList(void);
extern void inserthead(Linklist head,Linklist new_client);
extern int server_login(struct s_recv *recv);
extern int server_quit(Linklist client);
extern int create_table(sqlite3*db);
extern int server_view(struct s_recv*recv);
extern int reg_check(struct s_recv*recv);
extern int login_check(struct s_recv *recv);
extern int accept_file(struct s_send *send,struct s_recv*recv);
extern int send_file(struct s_send*send,struct s_recv*recv);
extern int server_reg(struct s_recv*a);
extern int server_all_chat(struct s_recv*recv,struct s_send*send);
extern int server_private_chat(struct s_recv*recv,struct s_send*send);
extern int check_forbid(struct s_recv*recv);
extern int forget_pwd(struct s_recv*recv,struct s_send*send);
extern int login_check(struct s_recv*recv);
extern void CreatLinkList();
extern void inserthead(Linklist head, Linklist new_client);
//extern void displaylist(Linklist L);

#endif
