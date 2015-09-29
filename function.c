/*************************************************************************
	> File Name: function.c
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月24日 星期四 17时51分28秒
 ************************************************************************/

/*
 * 功能接口
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "function.h"
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "fastCGI.h"
#include <sys/socket.h>
#include <arpa/inet.h>

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
    fp_send = fopen("log/send_log.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-log/send_log.txt\n");
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
    char query[100];

    if(strstr(filename,"?") != NULL)
    {
        strcpy(name,strtok(filename,"?"));
        strcpy(query,strtok(NULL,"?"));
    }
    else
    {
        strcpy(name,filename);
        strcpy(query,"name=123456");
    }

    /*check ext*/
    strtok(name,".");
    strcpy(ext,strtok(NULL,"."));
    if(strcmp(ext,"php") == 0)
    {
       catPHP(client_sockfd,filename,query); 
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
        fp_err = fopen("log/error_log.txt","a+");
        if(fp_err == NULL)
        {
            Debug("Error:fopen()-log/error_log.txt\n");
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
    fp_send = fopen("log/send_log.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-log/send_log.txt\n");
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
    fp_send = fopen("log/send_log.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-log/send_log.txt\n");
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

void catPHP(void *client_sockfd, char *filename, char *query)
{
    int c_sockfd = *((int *) client_sockfd);
    int sock;
    struct sockaddr_in serv_addr;
    int str_len;
    int contentLengthR;
    char msg[50];
    char buf[MAXSIZE];
    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: Moon Server\r\n";
    int i, contentLength, paddingLength;
    time_t t_send;
    FILE *fp_send;

    // 创建套接字
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock)
    {
        Debug("Error:socket()\n");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(FCGI_HOST);
	serv_addr.sin_port = htons(FCGI_PORT);

    // 连接服务器
	if(-1 == connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
    {
        Debug("Error:connetct()\n");
	}

    // 首先构造一个FCGI_BeginRequestRecord结构
    FCGI_BeginRequestRecord beginRecord;
    beginRecord.header = makeHeader(FCGI_BEGIN_REQUEST, FCGI_REQUEST_ID, sizeof(beginRecord.body), 0);
    beginRecord.body = makeBeginRequestBody(FCGI_RESPONDER);
    
    str_len = safe_write(sock, &beginRecord, sizeof(beginRecord));
	if(-1 == str_len)
    {
        Debug("Error:Write beginRecord failed!\n");
	}
        
    /*send log*/
    fp_send = fopen("log/send_log.txt","a+");
    if(fp_send == NULL)
    {
        Debug("Error:fopen()-log/send_log.txt\n");
    }
    time(&t_send);
    fputs(ctime(&t_send),fp_send);
    fwrite(&beginRecord,1,sizeof(beginRecord),fp_send);
    
    // 传递FCGI_PARAMS参数
    strcpy(msg, "/home/answer/Moon-WebServer/");
    strcat(msg, filename);
    char *params[][2] = {
        {"SCRIPT_FILENAME", msg}, 
        {"REQUEST_METHOD", "GET"}, 
        {"QUERY_STRING", query}, 
        {"", ""}
    };

    FCGI_ParamsRecord *paramsRecordp;
    for(i = 0; params[i][0] != ""; i++)
    {
        contentLength = strlen(params[i][0]) + strlen(params[i][1]) + 2; // 2字节是两个长度值
        paddingLength = (contentLength % 8) == 0 ? 0 : 8 - (contentLength % 8);
        paramsRecordp = (FCGI_ParamsRecord *)malloc(sizeof(FCGI_ParamsRecord) + contentLength + paddingLength);
        paramsRecordp->nameLength = (unsigned char)strlen(params[i][0]);    // 填充参数值
        paramsRecordp->valueLength = (unsigned char)strlen(params[i][1]);   // 填充参数名
        paramsRecordp->header = makeHeader(FCGI_PARAMS, FCGI_REQUEST_ID, contentLength, paddingLength);
        memset(paramsRecordp->data, 0, contentLength + paddingLength);
        memcpy(paramsRecordp->data, params[i][0], strlen(params[i][0]));
        memcpy(paramsRecordp->data + strlen(params[i][0]), params[i][1], strlen(params[i][1]));
        str_len = safe_write(sock, paramsRecordp, 8 + contentLength + paddingLength);
		if(-1 == str_len){
            Debug("Error:Write beginRecord failed!\n");
		}
        fwrite(paramsRecordp,1,8+contentLength+paddingLength,fp_send);
        free(paramsRecordp);
    }

    // 传递FCGI_STDIN参数
    FCGI_Header stdinHeader;
    stdinHeader = makeHeader(FCGI_STDIN, FCGI_REQUEST_ID, 0, 0);
    safe_write(sock, &stdinHeader, sizeof(stdinHeader));
    fwrite(&stdinHeader,1,sizeof(stdinHeader),fp_send);
    
    // 读取解析FASTCGI应用响应的数据
    FCGI_Header respHeader;
    char *message;
    str_len = safe_read(sock, &respHeader, 8);
	if(-1 == str_len){
        Debug("Error:read responder failed!\n");
	}
    
    if(respHeader.type == FCGI_STDOUT){
        contentLengthR = ((int)respHeader.contentLengthB1 << 8) + (int)respHeader.contentLengthB0;
        message = (char *)malloc(contentLengthR);
        read(sock, message, contentLengthR);
    }
    
    safe_write(c_sockfd, status, strlen(status));    // 发送响应报文状态行
    safe_write(c_sockfd, header, strlen(header));    // 发送响应报文消息头
    safe_write(c_sockfd, message, contentLengthR);
    fwrite(status,1,strlen(status),fp_send);
    fwrite(header,1,strlen(header),fp_send);
    fwrite(message,1,contentLengthR,fp_send);

printf("123\n");
    fclose(fp_send);
    free(message);
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
    fp_req = fopen("log/req_log.txt","a+");
    if(fp_req == NULL)
    {
        Debug("Error:fopen()-log/req_log.txt");
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
