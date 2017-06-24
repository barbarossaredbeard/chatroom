
#include "./client_handle.h"

int reg_user(struct c_send *send,int sockfd)
{
    char passwd_t[20];
    char passwd_lock_t[20];

    do
    {
        printf("请输入用户名(10字以内):\n");
        memset((*send).name,0,strlen((*send).name));//输入用户名
        setbuf(stdin,NULL);
        scanf("%s",(*send).name);
    }while(strlen((*send).name) > 10);

    while(1)
    {
        printf("请输入密码(20字以内):\n");
        memset((*send).pwd,0,strlen((*send).pwd));
        setbuf(stdin,NULL);
        scanf("%s",(*send).pwd);
        printf("请再次输入密码(20字以内):\n");
        memset(passwd_t,0,strlen(passwd_t));
        setbuf(stdin,NULL);
        scanf("%s",passwd_t);
        if(strcmp((*send).pwd,passwd_t) != 0 || strlen((*send).pwd) > 20 || strlen(passwd_t) > 20) //检查输入密码的一致性
        {
            printf("密码错误,请重输...\n");
        }
        else
        {
            break;
        }
    }

    printf("请输入密保手机:\n");
    memset((*send).pwd_lock,0,strlen((*send).pwd_lock));
    setbuf(stdin,NULL);
    scanf("%s",(*send).pwd_lock);

    strcpy((*send).cmd,"reg");
    write(sockfd,send,sizeof(struct c_send));//发送注册信息给服务器
    printf("正在注册,请稍等...\n");
    
    sleep(2);
}
