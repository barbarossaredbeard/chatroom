
#include "./server.h"

void CreatLinkList()//创建在线人员列表
{   
    clientlink = (Linklist)malloc(sizeof(struct online));
    clientlink -> next = NULL;
}

void inserthead(Linklist head,Linklist new_client)//登录成功,插入在线链表
{
    new_client -> next = head -> next;
    head -> next = new_client;
}

/*void displaylist(Linklist L) 
{
    Linklist temp;
    temp = L -> next;
    while(temp != NULL)
    {   
        printf("帐号 : %d\n",temp -> account);
        printf("用户名 : %s\n",temp -> name);
        printf("Socket : %d\n",temp -> sockfd);
        temp = temp -> next;
    }
}*/

