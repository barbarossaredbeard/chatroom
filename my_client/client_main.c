
#include "./client_handle.h"

int main(int argc)
{
    struct sockaddr_in my_addr;
    struct hostent *host;
    int sockfd;

    pthread_t tid_write;     
    pthread_t tid_read;
    
    int err_write;
    int err_read;


  /*  if(argc != 2)
    {
        fprintf(stderr,"Usage : %s hostname\n",argv[0]);
        exit(1);
    }

    if((host = gethostbyname(argv[1])) == NULL)
    {
        fprintf(stderr,"Gethostname error\n ");
        exit(1);
    }*/

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {   
        fprintf(stderr,"Socket Error : %s \a\n",strerror(errno));
        exit(-1);
    }

    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    my_addr.sin_port = htons(PORT);

    if((connect(sockfd,(struct sockaddr *)(&my_addr),sizeof(my_addr))) == -1)
    {
        printf("正在连接,请稍等...\n");
        exit(1);
    }
   

    err_write = pthread_create(&tid_write,NULL,(void*)write_msg,(void*)&sockfd);//创建客户端写线程
    if(err_write != 0)
    {
        printf("can't create client writing thread!");
        exit(1);
    }


    err_read = pthread_create(&tid_read,NULL,(void*)read_msg,(void*)&sockfd); //创建客户端读线程
    if(err_read != 0)
    {
        printf("can't create client reading thread!");
        exit(1);
    }


    pthread_join(tid_write,NULL);   //等待写线程的结束
    pthread_join(tid_read,NULL);    //等待读线程的结束
    
    close(sockfd);
    exit(0);
}


