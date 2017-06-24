
#include "./client_handle.h"

int send_file(struct c_send* send,int sockfd)
{
    int read_num;
    printf("请输入想要发送的文件路径:\n");//如./hello   ../../
    scanf("%s",send -> filename);          
    send->fd = open(send->filename,O_RDWR);
    if(send->fd < 0)
    {
        sleep(2);
        printf("没有找到此文件!\n");
        return 0;
    }
    else
    {
        strcpy(send->name,localname);
        printf("请输入发送对象：\n");
        scanf("%s",send->say_to);
        while(read_num != 0)  //将文件内容读到结构体成员中
        {
           read_num = read(send->fd,send->msg,1000);
        }
        strcpy(send->cmd,"will_send");
        write(sockfd,send,sizeof(struct c_send)); //发送给接受者
        printf("文件正在发送>>>>>>>\n");
        close(send->fd);
        sleep(2);
        read_finish = 1;
        is_chat = 1;
        return 1;
    }
}


int file_accept(struct c_recv *recv)
{
    char savefilename[NORMAL];
    int  save_fd;
    int  write_num;

    printf("请输入保存的路径：\n");
    scanf("%s",savefilename);
    save_fd = open(savefilename,O_RDWR|O_CREAT|O_APPEND);//打开一个不存在就创建的文件
    write_num = write(save_fd,recv->msg,1000); //将内容写入文件中
    if(write_num < 0)
    {
        printf("文件保存失败！\n");
    }
    else
    {   
       printf("文件保存成功!\n");
    }
    return 0;
}
