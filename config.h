/*************************************************************************
	> File Name: config.h
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月17日 星期四 19时18分50秒
 ************************************************************************/

#ifndef _CONFIG_H
#define _CONFIG_H

#define PORT 8080
const char status[] = "HTTP/1.0 200 OK\r\n";
const char header[] = "Server: A Simple Web Server\r\nContent-Type:text/html\r\n\r\n ";
const char body[] = "<html<head><title>Web Server for PHP</title></head><body><h2>Hello World!</h2><p>I'm AnSwEr!</p></body></html>";
#endif
