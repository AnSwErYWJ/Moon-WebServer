/*************************************************************************
	> File Name: function.c
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月24日 星期四 17时51分28秒
 ************************************************************************/

/*
 * Function
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "function.h"
#include <string.h>
#include <pthread.h>
#include <time.h>

/*
 * safe to read and write
 * 套接字使用字节流读写时内核套接字的缓冲区有可能会达到极限.
 */

extern ssize_t safe_read(int fd,void *vptr,size_t n)
 {
     char *ptr = vptr;
     size_t nleft = n;
     ssize_t nread;

     while(nleft > 0)
     {
         if((nread = read(fd,ptr,nleft)) < 0)
         {
             if(errno == EINTR)
                nread = 0;
             else
                return -1;
         }
         else
         if(nread == 0)
            break;
         nleft -= nread;
         ptr += nread;
     }

     return (n - nleft);
 }

extern ssize_t safe_write(int fd,const void *vptr,size_t n)
{
    const char *ptr = vptr;
    ssize_t nleft = n;
    size_t nwritten;

    while(nleft > 0)
    {
        if((nwritten = write(fd,ptr,nleft)) <= 0)
        {
            if(nwritten==-1 && errno==EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }

    return n;
}

/*send error message to client*/
extern void sendError(void *client_sockfd)
{
    int c_sockfd = *((int *)client_sockfd);
    time_t t_send;
    FILE *fp_send;

    char status[] = "HTTP/1.0 400 Bad Request\r\n";
    char header[] = "Server: Moon Server\r\nContent-Type: text/html\r\n\r\n";
    char body[] = "<html><head><title>Bad Request</title></head><body><p>Connect failed,please check network!</p></body></html>";

    /*send*/
    safe_write(c_sockfd,status,strlen(status));
    safe_write(c_sockfd,header,strlen(header));
    safe_write(c_sockfd,body,strlen(body));
    
    /*send log*/
    fp_send = fopen("log/send.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-send.txt\n");
    }
    time(&t_send);
    fputs(ctime(&t_send),fp_send);
    fwrite(status,1,strlen(status),fp_send);
    fwrite(header,1,strlen(header),fp_send);
    fwrite(body,1,strlen(body),fp_send);
    fprintf(fp_send,"\n");
    fclose(fp_send);
}


/*read file and send*/
extern void sendDate(void *client_sockfd,char *filename)
{
    int c_sockfd = *((int *)client_sockfd);
    char name[20];
    char ext[10];
    time_t t_err;
    FILE *fp_err;

    strcpy(name,filename);

    /*check ext*/
    strtok(name,".");
    strcpy(ext,strtok(NULL,"."));
    if(strcmp(ext,"php") == 0)
    {
        
    }
    else if(strcmp(ext,"html") == 0)
    {
        catHTML(client_sockfd,filename);
    }
    else if(strcmp(ext,"jpg") == 0)
    {
        catJPEG(client_sockfd,filename);
    }
    else
    {
        /*error log*/
        fp_err = fopen("log/error.txt","a+");
        if(fp_err == NULL)
        {
            Debug("Error:fopen()-error.txt\n");
        }
        time(&t_err);
        fputs(ctime(&t_err),fp_err);
        fprintf(fp_err,"ext is error\n");
        fclose(fp_err);
        
        sendError(client_sockfd);
        close(c_sockfd);
    }
}

extern void catHTML(void *client_sockfd,char *filename)
{
    char buf[MAXSIZE];
    int c_sockfd = *((int *)client_sockfd);
    FILE *fp;
    time_t t_send;
    FILE *fp_send;

    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: Moon Server\r\nContent-Type: text/html\r\n\r\n";

    /*send status and header*/
    safe_write(c_sockfd,status,strlen(status));
    safe_write(c_sockfd,header,strlen(header));

    /*send log*/
    fp_send = fopen("log/send.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-send.txt\n");
    }
    time(&t_send);
    fputs(ctime(&t_send),fp_send);
    fwrite(status,1,strlen(status),fp_send);
    fwrite(header,1,strlen(header),fp_send);
    
    /*open file*/
    fp = fopen(filename,"r");
    if(fp == NULL)
    {
        Debug("Error:fopen()-html\n");
        sendError(client_sockfd);
        close(c_sockfd);
    }

    /*send*/
    fgets(buf,sizeof(buf),fp);
    while(!feof(fp))
    {
        safe_write(c_sockfd,buf,strlen(buf));
        fwrite(buf,1,strlen(buf),fp_send);
        fgets(buf,sizeof(buf),fp);
    }

    fclose(fp_send);
    fclose(fp);
    close(c_sockfd);
}

extern void catJPEG(void *client_sockfd,char *filename)
{
    char buf[MAXSIZE];
    int c_sockfd = *((int *)client_sockfd);
    FILE *fp;
    FILE *fw;
    time_t t_send;
    FILE *fp_send;

    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: Moon Server\r\nContent-Type: image/jpeg\r\n\r\n";

    /*send status and header*/
    safe_write(c_sockfd,status,strlen(status));
    safe_write(c_sockfd,header,strlen(header));

    /*send log*/
    fp_send = fopen("log/send.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-send.txt\n");
    }
    time(&t_send);
    fputs(ctime(&t_send),fp_send);
    fwrite(status,1,strlen(status),fp_send);
    fwrite(header,1,strlen(header),fp_send);
    
    /*open jpeg in binary*/
    fp = fopen(filename,"rb");
    if(fp == NULL)
    {
        Debug("Error:fopen()-JPEG\n");
        sendError(client_sockfd);
        close(c_sockfd);
    }

    /*open fp in fd*/
    /*非文本文件最好都用二进制模式打开*/
    fw = fdopen(c_sockfd,"w");
    fread(buf,1,sizeof(buf),fp);
    while(!feof(fp))
    {
        fwrite(buf,1,sizeof(buf),fw);
        fwrite(buf,1,strlen(buf),fp_send);
        fread(buf,1,sizeof(buf),fp);
    }

    fclose(fp_send);
    fclose(fw);
    fclose(fp);
    close(c_sockfd);
}

/*
 * handle request
 */
extern void handleRequest(void *client_sockfd) 
{
    char buf[MAXSIZE];
    char *buf_backup;
    int c_sockfd = *((int *) client_sockfd);
    char method[10]; //请求方法
    char filename[20]; //请求文件名
    FILE *fp_req;
    time_t t_req;
    char temp[20];

    /*read request*/
    read(c_sockfd,buf,sizeof(buf)-1);
    
    /*request log*/
    fp_req = fopen("log/request.txt","a+");
    if(fp_req == NULL)
    {
        Debug("Error:fopen()-request.txt");
    }
    time(&t_req);
    fputs(ctime(&t_req),fp_req);
    fwrite(buf,1,strlen(buf),fp_req);
    fclose(fp_req);
    
    /*check if http*/
    if(strstr(buf,"HTTP/") == NULL)
    {
        Debug("Error: Not HTTP\n");
        sendError(client_sockfd);
        close(c_sockfd);
    }

    /*take method and check if GET*/
    strcpy(method,strtok(buf," /"));
    if(strcmp(method,"GET") != 0)
    {
        Debug("Error:Not GET\n");
        sendError(client_sockfd);
        close(c_sockfd);
    }

    /*take filename*/
    strcpy(temp,strtok(NULL," "));
    strcpy(filename,temp+1);

    /*read file and send*/
    sendDate(client_sockfd,filename);
    
    pthread_exit("Bye");
}
