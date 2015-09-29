/*************************************************************************
	> File Name: function.h
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
extern ssize_t safe_read(int fd,void *vptr,size_t n);
extern ssize_t safe_write(int fd,const void *vptr,size_t n);

/*handle the request*/
extern void handleRequest(void *client_sockfd);

/*read file and send*/
extern void sendDate(void *client_sockfd,char *filename);
extern void catHTML(void *client_sockfd,char *filename);
extern void catJPEG(void *client_sockfd,char *filename);
extern void sendError(void *client_sockfd);

#endif
