
#include "./server.h"

int main()
{   

    pthread_t tid;

    int sfd;
    int lfd;

    struct sockaddr_in sin;
    struct sockaddr_in cin;
    socklen_t addr_len;

    int opt;
    int len;
    char addr_p[20];
    char buf[MAXLEN];
    time_t ticks;


    int err;
    int err_quit;

    CreatLinkList();  //创建在线人员列表

    bzero(&sin,sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);//接受这个系统任意客户端
    sin.sin_port = htons(portnumber);

    if((lfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("socket error!\n");
        exit(1);
    }
    
    opt = 1;
    if(setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
    {
        perror("setsockfd failure!\n");
        exit(1);
    }

    if(bind(lfd,(struct sockaddr *)(&sin),sizeof(struct sockaddr)) == -1)  //用来关联地址和套件字
    {
        printf("bind error!\n");
        exit(1);
    }

    if(listen(lfd,20) == -1)        //宣告服务器愿意接受连接请求
    {
        printf("listen error!\n");
        exit(1);
    }

    system("clear");
    printf("             聊天室服务器\n");
    ticks = time(NULL);
    sprintf(buf,"Current time : %s",ctime(&ticks));
    printf("%s\n",buf); //显示当前时间
    printf("connenct...\n");

    while(1)
    {   
        addr_len = sizeof(sin);
        if((sfd = accept(lfd,(struct sockaddr *)(&cin),&addr_len)) == -1)
        {
            printf("accept error!\n");
            exit(-1);
        }

        err = pthread_create(&tid,NULL,(void *)server_deal,(void*)&sfd);   //服务器处理线程（可读可写）
        if(err != 0)
        {
            printf("can't create server thread!\n");
            exit(-1);
        }
    }

    close(lfd);
    return 0;
}



