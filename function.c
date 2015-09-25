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
