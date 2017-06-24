
#include "./client_handle.h"

int reg_log_face()
{
    int do_number;

    do
    {
        system("clear");
        printf("\n\n\n\n");
        printf("                    欢迎来到凹凸曼聊天室      \n");
        printf("               =============================\n");
        printf("                    1. *****用户登录*****      \n");
        printf("                    2. *****用户注册*****     \n");
        printf("                    3. *****忘记密码*****     \n");
        printf("                    0. *****  退出  *****   \n");
        printf("               =============================\n");
        printf("请选择(0-4):");
        setbuf(stdin,NULL);
        scanf("%d",&do_number);
        setbuf(stdin,NULL);
    }while((do_number != 0) && (do_number != 1) && (do_number != 2) && (do_number != 3));
    return do_number;
}

int chat_face()
{   
    int do_number;

    do
    {   
        system("clear");
        printf("\n\n\n\n");
        printf("                       尊敬的 %s 用户,你好:\n",localname);
        printf("              ===========================================\n");
        printf("                           1.**** 群聊\n");
        printf("                           2.**** 私聊\n");
        printf("                           3.**** 查看在线用户\n");
        printf("                           4.**** 传输文件\n");
        printf("                           5.**** 同意接受文件\n");
        printf("                           6.**** 拒绝接受文件\n");
        printf("                           7.**** 查看聊天记录\n");
        printf("                           0.**** 退出\n"); 
        printf("              ===========================================\n");
        printf("请选择(0-7):");
        setbuf(stdin,NULL);
        scanf("%d",&do_number);
        setbuf(stdin,NULL);
    }while((do_number != 1) && (do_number != 2) && (do_number != 3) && (do_number != 0) && (do_number != 4) && (do_number != 5) && (do_number != 6) && (do_number != 7));

    return do_number;    
}

int admin_face()
{
    int do_number;

    do
    {   
        system("clear");
        printf("\n\n\n\n");
        printf("                       尊敬的管理员,你好:\n");
        printf("                 ===============================\n");
        printf("                          1.**** 踢人\n");
        printf("                          2.**** 禁言\n");
        printf("                          3.**** 解禁\n");
        printf("                          0.**** 退出\n"); 
        printf("                 ===============================\n");
        printf("请选择(0-3):");
        setbuf(stdin,NULL);
        scanf("%d",&do_number);
        setbuf(stdin,NULL);
    }while((do_number != 1) && (do_number != 2) &&(do_number != 3)&&(do_number != 0));

    return do_number;    

}
