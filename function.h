/*************************************************************************
	> File Name: function.h
	> Author: AnSwEr
	> Mail: 1045837697@qq.com
	> Created Time: 2015年09月24日 星期四 17时55分09秒
 ************************************************************************/

#ifndef _FUNCTION_H
#define _FUNCTION_H

/*safe to read and write*/
ssize_t safe_read(int fd,void *vptr,size_t n);
ssize_t safe_write(int fd,const void *vptr,size_t n);

#endif
