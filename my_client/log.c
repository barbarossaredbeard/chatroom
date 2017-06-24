
#include "./client_handle.h"

int log_user(struct c_send *send,int sockfd)
{   
    do
    {
        printf("请输入帐号:");
        setbuf(stdin,NULL);
        scanf("%d",&(*send).account);
        printf("请输入密码(20字以内):");
        scanf("%s",(*send).pwd);
    }while(strlen((*send).pwd) > 20);
    
    strcpy((*send).cmd,"login");
    printf("正在登录，请稍等...\n");
    sleep(1);
    write(sockfd,send,sizeof(struct c_send));
}


int forget_pwd(struct c_send *send,int sockfd)
{
    printf("请输入帐号:");
    scanf("%d",&(*send).account);
    printf("请输入该帐号的密保手机:");
    scanf("%s",(*send).pwd_lock);
    
    strcpy((*send).cmd,"forget pwd");
    printf("服务器正在处理,请稍等...\n ");
    sleep(1);
    write(sockfd,send,sizeof(struct c_send));
    return 0;
}
