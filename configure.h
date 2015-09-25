/*************************************************************************
	> File Name: configure.h
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月24日 星期四 17时27分20秒
 ************************************************************************/

/*
 *Confirguration
 */

#ifndef _CONFIGURE_H
#define _CONFIGURE_H

#define TRUE 1
#define FALSE 0

#define PORT 8080 //apache default port

#define MAXSIZE 1024 //BufSize

char status[] = "HTTP/1.0 200 OK\r\n";
char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";
char body[] = "<html><head><title>A Simple Web Server</title></head><body><h2>Welcome!</h2><p>This is shiyanlou!</p></body></html>";

#endif
