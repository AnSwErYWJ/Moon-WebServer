#!/bin/bash
#备份并清空日志文件

datetime=$(date +%Y%m%d-%H%M%S)

if [ -s $1 ];then
    cp $1 ../log_backup/$1-$datetime.txt;
    : > $1;
    echo "$1 has been cleaned up";
fi;
