
#include "./server.h"

int create_table(sqlite3 *db)
{
    char *errmsg = NULL;
    char *sql = NULL;

    sql = "create table if not exists reg_table(account integer primary key,user_name text,password text,pwd_lock text,reg_time timestamp);";

    if(SQLITE_OK != sqlite3_exec(db,sql,0,0,&errmsg))
    {
        printf("create register table error :%s\n",errmsg);
        exit(1);
    }

    return 0;
}

int reg_check(struct s_recv *recv)
{
    int ret;
    sqlite3 *db;
    char sql[1024];
    char *errmsg = NULL;

    ret = sqlite3_open("./register.db",&db);
    if(SQLITE_OK != ret)
    {
        printf("cant't open register.db:%s\n",sqlite3_errmsg(db));
        exit(-1);
    }

    create_table(db);
    
    srand(time(NULL));
    recv -> account = rand()%10000;//生成帐号
    //recv -> account = 00001;//生成管理员时用
    sprintf(sql,"insert into reg_table(account,user_name,password,pwd_lock,reg_time) values(%d,'%s','%s','%s',datetime('now','localtime'));",recv -> account,recv -> name,recv -> pwd,recv -> pwd_lock); 

    if(SQLITE_OK != sqlite3_exec(db,sql,0,0,&errmsg))
    {
        printf("insert error !\n");
        return FAULT;
    }

    sqlite3_close(db);
    return OK;
}

int login_check(struct s_recv *recv)
{
    int ret;
    char sql[1024];
    char *errmsg = NULL;
    char **result;
    int rows,cols;
    sqlite3 *db;
    Linklist my_head;
    my_head = clientlink;
    my_head = my_head->next;

    while(my_head != NULL)
    {
        if(my_head->account == recv->account)
        {
            return REPEAT_LOG;
        }
        my_head = my_head->next;
    }

    ret = sqlite3_open("./register.db",&db);
    if(SQLITE_OK != ret)
    {
        printf("cant't open register.db:%s\n",sqlite3_errmsg(db));
        exit(-1);
    }

    sprintf(sql,"select * from reg_table where account=%d and password='%s';",recv -> account,recv -> pwd);

    sqlite3_get_table(db,sql,&result,&rows,&cols,&errmsg);

    if((rows == 1) && (recv -> account == 1)) //管理员登录
    {   
        strcpy(recv -> name ,result[6]);//根据用户输入的帐号和密码找到匹配的用户名(管理员)
       // printf("recv -> name %s\n",result[6]);
        sqlite3_close(db);
        return LOG_ADMIN;
    }
    if(rows ==  1) //普通用户登录
    {  
        strcpy(recv -> name,result[6]);//根据用户输入的帐号和密码找到匹配的用户名
        sqlite3_close(db);
        return LOG_USER;
    }
    else        //登录失败
    {
        sqlite3_close(db);
        return LOG_FAULT;
    }
}

int check_forbid(struct s_recv *recv)  //私聊群聊时，检查自己是否被禁言
{
    Linklist my_head; 
    my_head = clientlink;
    my_head = my_head->next;

    while(strcmp(recv->name,my_head->name) != 0)
    {
        my_head = my_head->next;
    }

    if((my_head->speak) == 1)
        return 1;     //没有被禁言
    else
        return 0;
    
}

int forget_pwd(struct s_recv *recv,struct s_send *send)
{
    sqlite3 *db = NULL;
    int ret;
    int cols;
    int rows;
    char **result;
    char *errmsg;
    char sql[1024];
    ret = sqlite3_open("./register.db",&db);
    if(SQLITE_OK != ret)
    {
        printf("cant't open register.db:%s\n",sqlite3_errmsg(db));
        exit(-1);
    }

    sprintf(sql,"select * from reg_table where account=%d and pwd_lock='%s';",recv -> account,recv -> pwd_lock);

    sqlite3_get_table(db,sql,&result,&rows,&cols,&errmsg);//找出对应的消息记录

    if(rows == 1)  //找到此人
    { 
        strcpy(send -> cmd,"find");
        strcpy(send -> msg,result[7]);//密码拷贝
        sqlite3_close(db);
    }
    else   //没有找到此人
    {
        strcpy(send -> cmd,"no_find");
        sqlite3_close(db);
    }
    return 0;
}
