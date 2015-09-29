# 参考资料

## WEB 服务器执行的任务
一般的商用 WEB 服务器都要执行下面这些任务：

1. 建立连接——接受一个客户端连接。
2. 接收请求——从网络中读取一条 HTTP 请求报文。 
3. 处理请求——对请求报文进行解释，并采取行动。 
4. 访问资源——访问报文中指定的资源。
5. 构建响应——创建带有正确首部的 HTTP 响应报文。 
6. 发送响应——将响应回送给客户端。 
7. 记录事务处理过程——将与已完成事务有关的内容记录在一个日志文件中。

### 接收请求
一个 HTTP 请求报文由请求行（request line）、请求头部（header）、空行和请求数据 4 个部分组成。例如当我们在浏览器中访问：127.0.0.1/index.html 时，浏览器发送给 WEB 服务器的请求报文大致如下：
```
/*请求行*/
GET  /index.html  HTTP/1.1  

/*请求头部*/
User-Agent: Mozilla/5.0

/*
 * 典型的请求头：
 * User-Agent：产生请求的浏览器类型。 
 * Accept：客户端可识别的内容类型列表。 
 * Host：请求的主机名，允许多个域名同处一个 IP 地址，即虚拟主机。
 */
```
1. 请求行由请求方法字段、URL 字段和 HTTP 协议版本字段 3 个字段组成，它们用空格分隔。HTTP 协议的请求方法有 GET、POST、HEAD、PUT、DELETE、OPTIONS、TRACE、CONNECT。
2. 请求头部由 关键字/值对 组成，每行一对，关键字和值用英文冒号 “:” 分隔。请求头部通知服务器有关于客户端请求的信息。
3. 最后一个请求头之后是一个空行，发送回车符和换行符，通知服务器以下不再有请求头。
4. 请求数据不在GET 方法中使用，而是在 POST 方法中使用。POST 方法适用于需要客户填写表单的场合。

###处理请求
处理请求，一旦 Web 服务器收到了请求，就可以根据方法、资源、首部和可选的主体部分来对请求进行处理了。
本项目中处理请求的流程是，首先检查请求协议是否是 HTTP 协议，然后，检查请求是否是 GET 请求，最后，提取浏览器请求的文件名。

###访问资源
访问资源，根据浏览器请求的文件名读取文件内容，并将内容发送给浏览器。本项目实现了读取静态 html、jpg 和 php 文件。

###构建响应
构建响应就是构建一个响应报文发送给浏览器。
HTTP 响应报文主要由状态行、响应头部、空行以及响应正文 4 部分组成。
状态行由 3 部分组成，分别为：HTTP 协议版本，状态代码，状态代码描述。
```
char status[] = "HTTP/1.0 200 OK\r\n";
```
状态代码为 3 位数字，由 1、2、3、4 以及 5 开头，2 开头的指响应成功，3 开头的指重定向，4 开头的指客户端错误，5 开头的指服务端错误。详细的状态码就不介绍了，这里列举几个常见的：
1. 200：响应成功
2. 400：错误的请求，用户发送的HTTP请求不正确 
3. 404：文件不存在，也就是HTTP请求URI错误 
4. 500：服务器内部错误

响应头部与请求头部类似，也包含了很多有用的信息。如响应内容的类型，图片类型为： Content-Type:image/jpeg。
```
char header[] = "Server: A Simple Web Server\r\nContent-Type: image/jpeg\r\n\r\n";
```

##WEB 服务器的几种模型
高性能的 WEB 服务器能够同时支持数千条连接。这些连接使得服务器可以与世界各地的客户端进行通信，每个客户端都向服务器打开了一条或多条连接。因为请求可能会在任意时刻到达，所以 WEB 服务器会不停地观察有无新的 WEB 请求。不同的 WEB 服务器模型会以不同的方式为请求服务。
###单线程 WEB 服务器
单线程的 WEB 服务器一次只处理一个请求，直到其完成为止。一个事务处理结束之后，才去处理下一条连接。这种结构易于实现，但在处理过程中，所有其他连接都会被忽略。这样会造成严重的性能问题，只适用于低负荷的服务器。
###多进程及多线程 WEB 服务器
多进程和多线程 WEB 服务器用多个进程， 或更高效的线程同时对请求进行处理。 可以根据需要创建，或者预先创建一些线程 / 进程。 有些服务器会为每条连接分配一个线程 / 进程，但当服务器同时要处理成百、上千，甚至数以万计的连接时，需要的进程或线程数量可能会消耗太多的内存或系统资源。因此，很多多线程 WEB 服务器都会对线程 / 进程的最大数量进行限制。
###复用 I/O 的服务器
为了支持大量的连接，很多 WEB 服务器都采用了复用结构。在复用结构中，要同时监视所有连接上的活动。当连接的状态发生变化时（比如，有数据可用，或出现错误时） ，就对那条连接进行少量的处理；处理结束之后，将连接返回到开放连接列表中，等待下一次状态变化。只有在有事情可做时才会对连接进行处理；在空闲连接上等待的时候并不会绑定线程和进程
###复用的多线程 WEB 服务器
有些系统会将多线程和复用功能结合在一起，以利用计算机平台上的多个 CPU。多个线程（通常是一个物理处理器）中的每一个都在观察打开的连接（或打开的连接中的一个子集） ，并对每条连接执行少量的任务。

##PHP 支持
目前 WEB 服务器可以处理 html 文件和 jpg 图片了。下面考虑怎样支持解析 php 文件，以前要执行动态程序都是通过 CGI 接口来完成的。CGI 全称是 “通用网关接口”(Common Gateway Interface)， 它可以让一个客户端，从网页浏览器向执行在 Web 服务器上的程序请求数据。 CGI 描述了客户端和这个程序之间传输数据的一种标准。 CGI 的一个目的是要独立于任何语言的，所以 CGI 可以用任何一种语言编写，只要这种语言具有标准输入、输出和环境变量， 如 php 等。

FastCGI 是 Web 服务器和处理程序之间通信的一种协议， 是 CGI 的一种改进方案，它为所有因特网应用提供高性能。FastCGI 像是一个常驻 (long-live) 型的 CGI， 它可以一直执行，在请求到达时不会花费时间去 fork 一个进程来处理(这是 CGI 最为人诟病的 fork-and-execute 模式)。 正是因为他只是一个通信协议，它还支持分布式的运算，即 FastCGI 程序可以在网站服务器以外的主机上执行并且接受来自其它网站服务器来的请求。

FastCGI 是语言无关的、可伸缩架构的 CGI 开放扩展，将 CGI 解释器进程保持在内存中，以此获得较高的性能。 CGI 程序反复加载是 CGI 性能低下的主要原因，如果 CGI 程序保持在内存中并接受 FastCGI 进程管理器调度， 则可以提供良好的性能、伸缩性、Fail-Over 特性等。

一般情况下，FastCGI 的整个工作流程是这样的：
1. Web Server 启动时载入 FastCGI 进程管理器（PHP-FPM 或 Apache Module)。
2. FastCGI 进程管理器自身初始化，启动多个 CGI 解释器进程(可见多个 php-cgi)并等待来自 Web Server 的连接。
3. 当客户端请求到达 Web Server 时，FastCGI 进程管理器选择并连接到一个 CGI 解释器。 Web server 将 CGI 环境变量和标准输入发送到 FastCGI 子进程 php-cgi。
4. FastCGI 子进程完成处理后将标准输出和错误信息从同一连接返回 Web Server。当 FastCGI 子进程关闭连接时， 请求便告处理完成。FastCGI 子进程接着等待并处理来自 FastCGI 进程管理器(运行在 Web Server 中)的下一个连接。在 CGI 模式中，php-cgi 在此便退出了。

##FastCGI 协议规范
当 WEB 服务器接收到一个 PHP 文件请求时，WEB 服务器会通过 FastCGI 将请求转发给 FastCGI 进程管理器（即 PHP-FPM），FastCGI 进程管理器接收到请求后，调用 php 解释器执行 php 文件，然后将执行结果返回给WEB服务器，之后 WEB 服务器再将 FastCGI 进程管理器返回的结果处理成 HTTP 响应报文格式发送给浏览器。

从上面描述可以看出，浏览器与 WEB 服务器之间的通信协议是 HTTP 协议，那么 WEB 服务器与 FastCGI 进程管理器之间通信协议是什么呢，答案就是 FastCGI 协议。

详细协议规范可参见： 
中文版：http://andylin02.iteye.com/blog/648412 
英文版：http://www.fastcgi.com/devkit/doc/fcgi-spec.html

FastCGI 协议大概流程：
请求由 FCGI_BEGIN_REQUEST 开始，FCGI_PARAMS 表示需要传递环境变量 (PHP中的 $_SERVER 数组就是通过 FCGI_PARAMS 来传递的，当然您还可以附加自定义的数据)。FCGI_STDIN 表示一个输入的开始，比如您需要 POST 过去的数据。FCGI_STDOUT 和 FCGI_STDERR标识应用开始响应。FCGI_END_REQUEST 表示一次请求的完成，由应用发送。

FastCGI 是基于流的协议，并且是 8 字节对齐，因此不需要考虑字节序，但是要考虑填充。FastCGI 的包头是固定的 8 字节，不同的请求有不同的包体结构。包头和包体组成一个 Record(记录)。具体请参考协议规范。包头见下面的 FCGI_Header 结构体，FCGI_BEGIN_REQUEST 请求包体见下面的 FCGI_BeginRequestBody 结构体， FCGI_BEGIN_REQUEST 请求记录见下面的 FCGI_BeginRequestRecord结构体，FCGI_PARAMS 传递参数记录见下面的 FCGI_ParamsRecord 结构体：
```
/*
 * 包头
 */
typedef struct{
    unsigned char version;
    unsigned char type;
    unsigned char requestIdB1;
    unsigned char requestIdB0;
    unsigned char contentLengthB1;
    unsigned char contentLengthB0;
    unsigned char paddingLength;
    unsigned char reserved;
}FCGI_Header;

/*
 * FCGI_BEGIN_REQUEST请求包体
 */
typedef struct{
    unsigned char roleB1;
    unsigned char roleB0;
    unsigned char flags;
    unsigned char reserved[5];
}FCGI_BeginRequestBody;

/*
 * FCGI_BEGIN_REQUEST请求记录
 */
typedef struct{
    FCGI_Header header;
    FCGI_BeginRequestBody body;
}FCGI_BeginRequestRecord;

/*
 * FCGI_PARAMS传递参数记录
 */
typedef struct{
    FCGI_Header header;
    unsigned char nameLength;
    unsigned char valueLength;
    unsigned char data[0];
}FCGI_ParamsRecord;
```

为了简化项目的复杂性，项目中会忽略 FCGI_STDERR 和 FCGI_END_REQUEST。WEB 服务器处理 PHP 文件请求时，首先会使用 FCGI_BEGIN_REQUEST 记录结构体向 FastCGI 进程管理器开始请求，然后使用 FCGI_PARAMS 记录结构体向 FastCGI 进程管理器传递参数，包含 php 文件名、query 字符串，HTTP 请求方法，之后发送一个空包体的 STDIN 包来结束这次请求。由于是空包体，所以直接发送 type 为 FCGI_STDIN 的 FCGI_Header 结构体就行了。
```
// 首先构造一个FCGI_BeginRequestRecord结构
    FCGI_BeginRequestRecord beginRecord;
    beginRecord.header = 
        makeHeader(FCGI_BEGIN_REQUEST, FCGI_REQUEST_ID, sizeof(beginRecord.body), 0);
    beginRecord.body = makeBeginRequestBody(FCGI_RESPONDER);
    str_len = write(sock, &beginRecord, sizeof(beginRecord));
```

上面的代码构造一个FCGI_Header结构体和FCGI_BeginRequestBody结构体组成一个FCGI_BEGIN_REQUEST记录，发送给FastCGI进程管理器表示开始请求。
```
// 传递FCGI_PARAMS参数
    char *params[][2] = {
        {"SCRIPT_FILENAME", "/home/shiyanlou/php-server/test.php"}, 
        {"REQUEST_METHOD", "GET"}, 
        {"QUERY_STRING", "name=shiyanlou"}, 
        {"", ""}
    };

    int i, contentLength, paddingLength;
    FCGI_ParamsRecord *paramsRecordp;
    for(i = 0; params[i][0] != ""; i++){
        contentLength = strlen(params[i][0]) + strlen(params[i][1]) + 2; // 2字节是两个长度值
        paddingLength = (contentLength % 8) == 0 ? 0 : 8 - (contentLength % 8);
        paramsRecordp = (FCGI_ParamsRecord *)malloc(sizeof(FCGI_ParamsRecord) + contentLength + paddingLength);
        paramsRecordp->nameLength = (unsigned char)strlen(params[i][0]);    // 填充参数值
        paramsRecordp->valueLength = (unsigned char)strlen(params[i][1]);   // 填充参数名
        paramsRecordp->header = 
            makeHeader(FCGI_PARAMS, FCGI_REQUEST_ID, contentLength, paddingLength);
        memset(paramsRecordp->data, 0, contentLength + paddingLength);
        memcpy(paramsRecordp->data, params[i][0], strlen(params[i][0]));
        memcpy(paramsRecordp->data + strlen(params[i][0]), params[i][1], strlen(params[i][1]));
        str_len = write(sock, paramsRecordp, 8 + contentLength + paddingLength);

        if(-1 == str_len){
            errorHandling("Write beginRecord failed!");
        }
        printf("Write params %s  %s\n",params[i][0], params[i][1]);
        free(paramsRecordp);
    }
```

请求开始后，WEB 服务器通过上面的代码向 FastCGI 进程管理器发送 FCGI_PARAMS 记录，由于 FastCGI 协议规定数据 8 字节对齐，需要对数据进行 8 字节对齐。

```
 // 传递FCGI_STDIN参数
    FCGI_Header stdinHeader;
    stdinHeader = makeHeader(FCGI_STDIN, FCGI_REQUEST_ID, 0, 0);
    write(sock, &stdinHeader, sizeof(stdinHeader));
```

传递完FCGI_PARAMS参数后，通过上面的代码向FastCGI进程管理器发送一个空包体的FCGI_STDIN记录，表示请求发送完毕。
```
// 读取FastCGI进程管理器响应的数据
    FCGI_Header respHeader;
    char *message;
    str_len = read(sock, &respHeader, 8);
    if(-1 == str_len){
        errorHandling("read responder failed!");
    }
    //printf("Start read....\n");
    //printf("fastcgi responder is : %X\n", respHeader.type);
    //printf("fastcgi responder is : %X\n", respHeader.contentLengthB1);
    //printf("fastcgi responder is : %X\n", respHeader.contentLengthB0);
    if(respHeader.type == FCGI_STDOUT){
        int contentLengthR = 
            ((int)respHeader.contentLengthB1 << 8) + (int)respHeader.contentLengthB0;
        message = (char *)malloc(contentLengthR);
        read(sock, message, contentLengthR);
        //printf("%s",message);
        free(message);
    }
```

上面的代码首先读取 FastCGI 进程管理器响应数据的前 8 字节数据，这 8 字节数据正好是一个 FCGI_Header 结构体变量。然后通过判断 type 是否是 FastCGI 进程管理器响应数据。如果是，读取响应数据长度，之后继续读取该长度的数据。至此，WEB 服务器与 FastCGI 进程管理器的交互结束了。

##配置
首先安装 php-fpm，使用如下命令：
```
sudo apt-get install php5-fpm
```
然后修改 php5-fpm 的配置文件，即 /etc/php5/fpm/pool.d/www.conf 文件。

定位到第 33 行，将 listen = /var/run/php5-fpm.sock 改为 listen = 127.0.0.1:9000，然后执行以下命令运行 php-fpm。
```
sudo php5-fpm
```
使用 netstat -apn | head -10可 以查看是否启动成功。
FastCGI 和 PHP-FPM 的关系可参见：http://segmentfault.com/q/1010000000256516
