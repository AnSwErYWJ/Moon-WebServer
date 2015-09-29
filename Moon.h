/*************************************************************************
	> File Name: Moon.h
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月24日 星期四 17时55分09秒
 ************************************************************************/

#ifndef _FUNCTION_H
#define _FUNCTION_H

#define TRUE 1
#define FALSE 0

#define PORT 8080 //apache default port

#define MAXSIZE 1024 //BufSize
#define MAX_THREAD_NUM 100

int ret;

int ser_sockfd,cli_sockfd;

pthread_mutex_t send_log;
pthread_mutex_t error_log;
pthread_mutex_t req_log;

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

/*safe to read and write*/
ssize_t safe_read(int fd,void *vptr,size_t n);
ssize_t safe_write(int fd,const void *vptr,size_t n);

/*handle the request*/
void handleRequest(void *client_sockfd);

/*read file and send*/
void sendError(void *client_sockfd);
void catHTML(void *client_sockfd,char *filename);
void catJPEG(void *client_sockfd,char *filename);
void catPHP(void *client_sockfd,char *filename,char *query);
void sendDate(void *client_sockfd,char *filename);
void handleRequest(void *client_sockfd); 

/*listen()的包裹函数*/
static void Listen(int fd,int backlog);

/*退出函数*/
static void quit(int signo);
#endif
