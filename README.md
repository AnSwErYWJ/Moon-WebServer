# Moon——月亮服务器
## Description
最近在使用apache，所以参考了一些资料，用C语言写了一个支持PHP的Web服务器。因为完成时临近是中秋节，所以取名为Moon.  

## Feature
- 支持多线程
- 支持FastCGI
- 实现日志系统
- 支持PHP语言
- 实现GET方法
- 支持处理HTML
- 支持JPEG等格式图片

## Configure
### log backup
```
cd log/ 
./empty.sh cli_log.txt
./empty.sh send_log.txt
./empty.sh req_log.txt
./empty.sh error_log.txt
```

###install php-fpm
```
sudo apt-get install php5-fpm
```
###modify configuration
```
修改php5-fpm的配置文件(/etc/php5/fpm/pool.d/www.conf),第33行，将 listen = /var/run/php5-fpm.sock 改为 listen = 127.0.0.1:9000
```
###start php-fpm
```
./start_php-fpm
```

## Compile and run
```
make
./Moon
#打开浏览器，在地址栏输入"127.0.0.1:8080/www/one.html"或"127.0.0.1:8080/www/two.html"
#打开浏览器，在地址栏输入"127.0.0.1:8080/test.php?name=AnSwEr"
make clean
```

## Feedback
- 微博：[@AnSwEr不是答案](http://weibo.com/1783591593)
- github：[AnSwErYWJ](https://github.com/AnSwErYWJ)
- 博客：[AnSwEr不是答案的专栏](http://blog.csdn.net/u011192270)
