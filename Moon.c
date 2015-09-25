/*************************************************************************
> File Name: Moon.c
> Author: AnSwEr
> Mail: 1045837697@qq.com
> Created Time: 2015年09月17日 星期四 19时11分05秒
************************************************************************/

/*
* moon服务器主程序
*/

#include <stdio.h>
#include "configure.h"
#include "function.h"
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

/*
* function: Debug
*/
#define Debug_Flag TRUE

#if defined(Debug_Flag)
#define Debug(...)\
        do{\
           fprintf(stderr,"-----DEBUG-----\n");\
           fprintf(stderr,"%s %s\n",__TIME__,__DATE__);\
           fprintf(stderr,"%s:%d:%s():",__FILE__,__LINE__,__func__);\
           fprintf(stderr,__VA_ARGS__);\
          }while(0)
#endif

static int ret;

/*
* listen()的包裹函数，可以自定义backlog
*/
static void Listen(int fd,int backlog)
{
    char *ptr;

    if((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    ret = listen(fd,backlog);
    if(ret == -1)
        Debug("Error:listen()\n");
}

int main(int argc,const char *argv[])
{
    char buf[MAXSIZE];
    int ser_sockfd,cli_sockfd;
    struct sockaddr_in ser_addr,cli_addr;
    socklen_t cli_addr_size;

    /*get server sockfd*/
    ser_sockfd = socket(PF_INET,SOCK_STREAM,0);
    if(ser_sockfd == -1)
        Debug("Error:socket()\n");

    /*configure ser_addr*/
    bzero(&ser_addr,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(PORT);

    /*bind ser_sockfd*/
    ret = bind(ser_sockfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
    if(ret == -1)
        Debug("Error:bind()\n");

    /*listen*/
    Listen(ser_sockfd,5);

        /*accept*/
        cli_addr_size = sizeof(cli_addr);
        bzero(&cli_addr,cli_addr_size);
       
        cli_sockfd = accept(ser_sockfd,(struct sockaddr *)&cli_addr,&cli_addr_size);
        if(cli_sockfd == -1)
            Debug("Error:accept()\n");
        
        /*read form client*/
        ret = read(cli_sockfd,buf,sizeof(buf)-1);
        if(ret == -1)
            Debug("Error:safe_read()\n");
        printf("%s",buf);

        /*write to client*/
        ret = safe_write(cli_sockfd,status,sizeof(status));
        if(ret == -1)
            Debug("Error:safe_write()--status\n");
        ret = safe_write(cli_sockfd,header,sizeof(header));
        if(ret == -1)
            Debug("Error:safe_write()--header\n");
        ret = safe_write(cli_sockfd,body,sizeof(body));
        if(ret == -1)
            Debug("Error:safe_write()--body\n");

    /*close fd*/
    close(ser_sockfd);

    return 0;
}
