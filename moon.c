/*************************************************************************
	> File Name: moon.c
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月17日 星期四 19时11分05秒
 ************************************************************************/

#include <stdio.h>
#include "configure.h"

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


int main(int argc,char *argv[])
{
    Debug("hi!");
    return 0;
}
