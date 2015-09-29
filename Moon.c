/*************************************************************************
> File Name: Moon.c
> Author: AnSwEr
> Mail: 1045837697@qq.com
> Created Time: 2015年09月17日 星期四 19时11分05秒
************************************************************************/

/*
 * Moon服务器主程序
 */

#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "Moon.h"
#include "fastCGI.h"

/*
 * safe to read and write
 * 套接字使用字节流读写时内核套接字的缓冲区有可能会达到极限.
 */

ssize_t safe_read(int fd,void *vptr,size_t n)
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

ssize_t safe_write(int fd,const void *vptr,size_t n)
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
void sendError(void *client_sockfd)
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

    pthread_mutex_lock(&send_log);

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

    pthread_mutex_unlock(&send_log);
}

void catHTML(void *client_sockfd,char *filename)
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

    pthread_mutex_lock(&send_log);

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
    
    pthread_mutex_unlock(&send_log);

    fclose(fp);
    close(c_sockfd);
}

void catJPEG(void *client_sockfd,char *filename)
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

    pthread_mutex_lock(&send_log);

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
    
    pthread_mutex_unlock(&send_log);

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
    char msg[100];
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
    
    pthread_mutex_lock(&send_log);

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
    strcpy(msg, "/home/answer/Moon-WebServer/www/");
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

    if(respHeader.type == FCGI_STDOUT)
    {
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

    fclose(fp_send);
    
    pthread_mutex_unlock(&send_log);

    free(message);
    close(c_sockfd);
}

/*read file and send*/
void sendDate(void *client_sockfd,char *filename)
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
        pthread_mutex_lock(&error_log);

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
        
        pthread_mutex_unlock(&error_log);

        sendError(client_sockfd);
        close(c_sockfd);
    }
}

/*
 * handle request
 */
void handleRequest(void *client_sockfd) 
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
  
    pthread_mutex_lock(&req_log);

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

    pthread_mutex_unlock(&req_log);
    
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

/*退出函数*/
static void quit(int signo)
{
    close(ser_sockfd);
    exit(EXIT_SUCCESS);
}

int main(int argc,const char *argv[])
{
    struct sockaddr_in ser_addr,cli_addr;
    char buf[MAXSIZE];
    socklen_t cli_addr_size;
    int thread_count = 0;
    pthread_t threads[MAX_THREAD_NUM];
    char ipaddress[20];
    FILE *fp_log;
    time_t t_log;  
    fd_set readfds,testfds;

    /*get server sockfd*/
    ser_sockfd = socket(PF_INET,SOCK_STREAM,0);
    if(ser_sockfd == -1)
        Debug("Error:socket()\n");

    /*quit signal*/
    signal(SIGINT,quit);

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

    FD_ZERO(&readfds);
    FD_SET(ser_sockfd,&readfds);

    while(1)
    {
        /*io多路复用*/
        testfds = readfds;
        ret = select(ser_sockfd+1,&testfds,(fd_set *)0,(fd_set *)0,NULL);
        if(ret == -1)
        {
            Debug("Error:select()\n");
        }
        
        if(FD_ISSET(ser_sockfd,&testfds))
        {       
            /*accept*/
            cli_addr_size = sizeof(cli_addr);
            bzero(&cli_addr,cli_addr_size);
       
            cli_sockfd = accept(ser_sockfd,(struct sockaddr *)&cli_addr,&cli_addr_size);
            if(cli_sockfd == -1)
            {
                Debug("Error:accept()\n");
            }

            /*client log*/
            fp_log = fopen("log/cli_log.txt","a+");
            if(fp_log == NULL)
            {
                Debug("Error:fopen()-log/cli_log.txt\n");
            }
        
            time(&t_log);
            fputs(ctime(&t_log),fp_log);
            inet_ntop(AF_INET,(void *)&cli_addr.sin_addr,ipaddress,16);
            fprintf(fp_log,"ipaddress:%s port:%d\n",ipaddress,cli_addr.sin_port);
            fclose(fp_log);
           
            /*init  mutex*/
            ret = pthread_mutex_init(&send_log,NULL);
            if(ret != 0)
            {
                Debug("Error:pthread_mutex_init()-send_log");
            }
            ret = pthread_mutex_init(&error_log,NULL);
            if(ret != 0)
            {
                Debug("Error:pthread_mutex_init()-error_log");
            }
            ret = pthread_mutex_init(&req_log,NULL);
            if(ret != 0)
            {
                Debug("Error:pthread_mutex_init()-req_log");
            }

            /*多线程*/
            ret = pthread_create(threads+(thread_count++),NULL,(void *)handleRequest,&cli_sockfd);
            if(ret != 0)
            {
                Debug("Error:pthread_create()\n");
            }
        }
    }

    /*close fd*/
    close(ser_sockfd);

    return EXIT_SUCCESS;
}
