# �ο�����
## WEB ������ִ�е�����
һ������� WEB ��������Ҫִ��������Щ����

1. �������ӡ�������һ���ͻ������ӡ�
2. �������󡪡��������ж�ȡһ�� HTTP �����ġ� 
3. �������󡪡��������Ľ��н��ͣ�����ȡ�ж��� 
4. ������Դ�������ʱ�����ָ������Դ��
5. ������Ӧ��������������ȷ�ײ��� HTTP ��Ӧ���ġ� 
6. ������Ӧ��������Ӧ���͸��ͻ��ˡ� 
7. ��¼��������̡�����������������йص����ݼ�¼��һ����־�ļ��С�

### ��������
һ�� HTTP �������������У�request line��������ͷ����header�������к��������� 4 ��������ɡ����統������������з��ʣ�127.0.0.1/index.html ʱ����������͸� WEB �������������Ĵ������£�
```
/*������*/
GET  /index.html  HTTP/1.1  

/*����ͷ��*/
User-Agent: Mozilla/5.0

/*
 * ���͵�����ͷ��
 * User-Agent�������������������͡� 
 * Accept���ͻ��˿�ʶ������������б� 
 * Host�������������������������ͬ��һ�� IP ��ַ��������������
 */
```
1. �����������󷽷��ֶΡ�URL �ֶκ� HTTP Э��汾�ֶ� 3 ���ֶ���ɣ������ÿո�ָ���HTTP Э������󷽷��� GET��POST��HEAD��PUT��DELETE��OPTIONS��TRACE��CONNECT��
2. ����ͷ���� �ؼ���/ֵ�� ��ɣ�ÿ��һ�ԣ��ؼ��ֺ�ֵ��Ӣ��ð�� ��:�� �ָ�������ͷ��֪ͨ�������й��ڿͻ����������Ϣ��
3. ���һ������ͷ֮����һ�����У����ͻس����ͻ��з���֪ͨ���������²���������ͷ��
4. �������ݲ���GET ������ʹ�ã������� POST ������ʹ�á�POST ������������Ҫ�ͻ���д���ĳ��ϡ�

###��������
��������һ�� Web �������յ������󣬾Ϳ��Ը��ݷ�������Դ���ײ��Ϳ�ѡ�����岿������������д����ˡ�
����Ŀ�д�������������ǣ����ȼ������Э���Ƿ��� HTTP Э�飬Ȼ�󣬼�������Ƿ��� GET ���������ȡ�����������ļ�����

###������Դ
������Դ�����������������ļ�����ȡ�ļ����ݣ��������ݷ��͸������������Ŀʵ���˶�ȡ��̬ html��jpg �� php �ļ���

###������Ӧ
������Ӧ���ǹ���һ����Ӧ���ķ��͸��������
HTTP ��Ӧ������Ҫ��״̬�С���Ӧͷ���������Լ���Ӧ���� 4 ������ɡ�
״̬���� 3 ������ɣ��ֱ�Ϊ��HTTP Э��汾��״̬���룬״̬����������
```
char status[] = "HTTP/1.0 200 OK\r\n";
```
״̬����Ϊ 3 λ���֣��� 1��2��3��4 �Լ� 5 ��ͷ��2 ��ͷ��ָ��Ӧ�ɹ���3 ��ͷ��ָ�ض���4 ��ͷ��ָ�ͻ��˴���5 ��ͷ��ָ����˴�����ϸ��״̬��Ͳ������ˣ������оټ��������ģ�
1. 200����Ӧ�ɹ�
2. 400������������û����͵�HTTP������ȷ 
3. 404���ļ������ڣ�Ҳ����HTTP����URI���� 
4. 500���������ڲ�����

��Ӧͷ��������ͷ�����ƣ�Ҳ�����˺ܶ����õ���Ϣ������Ӧ���ݵ����ͣ�ͼƬ����Ϊ�� Content-Type:image/jpeg��
```
char header[] = "Server: A Simple Web Server\r\nContent-Type: image/jpeg\r\n\r\n";
```

##WEB �������ļ���ģ��
�����ܵ� WEB �������ܹ�ͬʱ֧����ǧ�����ӡ���Щ����ʹ�÷�����������������صĿͻ��˽���ͨ�ţ�ÿ���ͻ��˶������������һ����������ӡ���Ϊ������ܻ�������ʱ�̵������ WEB �������᲻ͣ�ع۲������µ� WEB ���󡣲�ͬ�� WEB ������ģ�ͻ��Բ�ͬ�ķ�ʽΪ�������
###���߳� WEB ������
���̵߳� WEB ������һ��ֻ����һ������ֱ�������Ϊֹ��һ�����������֮�󣬲�ȥ������һ�����ӡ����ֽṹ����ʵ�֣����ڴ�������У������������Ӷ��ᱻ���ԡ�������������ص��������⣬ֻ�����ڵ͸��ɵķ�������
###����̼����߳� WEB ������
����̺Ͷ��߳� WEB �������ö�����̣� �����Ч���߳�ͬʱ��������д��� ���Ը�����Ҫ����������Ԥ�ȴ���һЩ�߳� / ���̡� ��Щ��������Ϊÿ�����ӷ���һ���߳� / ���̣�����������ͬʱҪ����ɰ١���ǧ������������Ƶ�����ʱ����Ҫ�Ľ��̻��߳��������ܻ�����̫����ڴ��ϵͳ��Դ����ˣ��ܶ���߳� WEB ������������߳� / ���̵���������������ơ�
###���� I/O �ķ�����
Ϊ��֧�ִ��������ӣ��ܶ� WEB �������������˸��ýṹ���ڸ��ýṹ�У�Ҫͬʱ�������������ϵĻ�������ӵ�״̬�����仯ʱ�����磬�����ݿ��ã�����ִ���ʱ�� ���Ͷ��������ӽ��������Ĵ����������֮�󣬽����ӷ��ص����������б��У��ȴ���һ��״̬�仯��ֻ�������������ʱ�Ż�����ӽ��д����ڿ��������ϵȴ���ʱ�򲢲�����̺߳ͽ���
###���õĶ��߳� WEB ������
��Щϵͳ�Ὣ���̺߳͸��ù��ܽ����һ�������ü����ƽ̨�ϵĶ�� CPU������̣߳�ͨ����һ�������������е�ÿһ�����ڹ۲�򿪵����ӣ���򿪵������е�һ���Ӽ��� ������ÿ������ִ������������

##PHP ֧��
Ŀǰ WEB ���������Դ��� html �ļ��� jpg ͼƬ�ˡ����濼������֧�ֽ��� php �ļ�����ǰҪִ�ж�̬������ͨ�� CGI �ӿ�����ɵġ�CGI ȫ���� ��ͨ�����ؽӿڡ�(Common Gateway Interface)�� ��������һ���ͻ��ˣ�����ҳ�������ִ���� Web �������ϵĳ����������ݡ� CGI �����˿ͻ��˺��������֮�䴫�����ݵ�һ�ֱ�׼�� CGI ��һ��Ŀ����Ҫ�������κ����Եģ����� CGI �������κ�һ�����Ա�д��ֻҪ�������Ծ��б�׼���롢����ͻ��������� �� php �ȡ�

FastCGI �� Web �������ʹ������֮��ͨ�ŵ�һ��Э�飬 �� CGI ��һ�ָĽ���������Ϊ����������Ӧ���ṩ�����ܡ�FastCGI ����һ����פ (long-live) �͵� CGI�� ������һֱִ�У������󵽴�ʱ���Ứ��ʱ��ȥ fork һ������������(���� CGI ��Ϊ��ڸ���� fork-and-execute ģʽ)�� ������Ϊ��ֻ��һ��ͨ��Э�飬����֧�ֲַ�ʽ�����㣬�� FastCGI �����������վ�����������������ִ�в��ҽ�������������վ��������������

FastCGI �������޹صġ��������ܹ��� CGI ������չ���� CGI ���������̱������ڴ��У��Դ˻�ýϸߵ����ܡ� CGI ���򷴸������� CGI ���ܵ��µ���Ҫԭ����� CGI ���򱣳����ڴ��в����� FastCGI ���̹��������ȣ� ������ṩ���õ����ܡ������ԡ�Fail-Over ���Եȡ�

һ������£�FastCGI ���������������������ģ�
1. Web Server ����ʱ���� FastCGI ���̹�������PHP-FPM �� Apache Module)��
2. FastCGI ���̹����������ʼ����������� CGI ����������(�ɼ���� php-cgi)���ȴ����� Web Server �����ӡ�
3. ���ͻ������󵽴� Web Server ʱ��FastCGI ���̹�����ѡ�����ӵ�һ�� CGI �������� Web server �� CGI ���������ͱ�׼���뷢�͵� FastCGI �ӽ��� php-cgi��
4. FastCGI �ӽ�����ɴ���󽫱�׼����ʹ�����Ϣ��ͬһ���ӷ��� Web Server���� FastCGI �ӽ��̹ر�����ʱ�� �����洦����ɡ�FastCGI �ӽ��̽��ŵȴ����������� FastCGI ���̹�����(������ Web Server ��)����һ�����ӡ��� CGI ģʽ�У�php-cgi �ڴ˱��˳��ˡ�

##FastCGI Э��淶
�� WEB ���������յ�һ�� PHP �ļ�����ʱ��WEB ��������ͨ�� FastCGI ������ת���� FastCGI ���̹��������� PHP-FPM����FastCGI ���̹��������յ�����󣬵��� php ������ִ�� php �ļ���Ȼ��ִ�н�����ظ�WEB��������֮�� WEB �������ٽ� FastCGI ���̹��������صĽ������� HTTP ��Ӧ���ĸ�ʽ���͸��������

�������������Կ������������ WEB ������֮���ͨ��Э���� HTTP Э�飬��ô WEB �������� FastCGI ���̹�����֮��ͨ��Э����ʲô�أ��𰸾��� FastCGI Э�顣

��ϸЭ��淶�ɲμ��� 
���İ棺http://andylin02.iteye.com/blog/648412 
Ӣ�İ棺http://www.fastcgi.com/devkit/doc/fcgi-spec.html

FastCGI Э�������̣�
������ FCGI_BEGIN_REQUEST ��ʼ��FCGI_PARAMS ��ʾ��Ҫ���ݻ������� (PHP�е� $_SERVER �������ͨ�� FCGI_PARAMS �����ݵģ���Ȼ�������Ը����Զ��������)��FCGI_STDIN ��ʾһ������Ŀ�ʼ����������Ҫ POST ��ȥ�����ݡ�FCGI_STDOUT �� FCGI_STDERR��ʶӦ�ÿ�ʼ��Ӧ��FCGI_END_REQUEST ��ʾһ���������ɣ���Ӧ�÷��͡�

FastCGI �ǻ�������Э�飬������ 8 �ֽڶ��룬��˲���Ҫ�����ֽ��򣬵���Ҫ������䡣FastCGI �İ�ͷ�ǹ̶��� 8 �ֽڣ���ͬ�������в�ͬ�İ���ṹ����ͷ�Ͱ������һ�� Record(��¼)��������ο�Э��淶����ͷ������� FCGI_Header �ṹ�壬FCGI_BEGIN_REQUEST ������������� FCGI_BeginRequestBody �ṹ�壬 FCGI_BEGIN_REQUEST �����¼������� FCGI_BeginRequestRecord�ṹ�壬FCGI_PARAMS ���ݲ�����¼������� FCGI_ParamsRecord �ṹ�壺
```
/*
 * ��ͷ
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
 * FCGI_BEGIN_REQUEST�������
 */
typedef struct{
    unsigned char roleB1;
    unsigned char roleB0;
    unsigned char flags;
    unsigned char reserved[5];
}FCGI_BeginRequestBody;

/*
 * FCGI_BEGIN_REQUEST�����¼
 */
typedef struct{
    FCGI_Header header;
    FCGI_BeginRequestBody body;
}FCGI_BeginRequestRecord;

/*
 * FCGI_PARAMS���ݲ�����¼
 */
typedef struct{
    FCGI_Header header;
    unsigned char nameLength;
    unsigned char valueLength;
    unsigned char data[0];
}FCGI_ParamsRecord;
```

Ϊ�˼���Ŀ�ĸ����ԣ���Ŀ�л���� FCGI_STDERR �� FCGI_END_REQUEST��WEB ���������� PHP �ļ�����ʱ�����Ȼ�ʹ�� FCGI_BEGIN_REQUEST ��¼�ṹ���� FastCGI ���̹�������ʼ����Ȼ��ʹ�� FCGI_PARAMS ��¼�ṹ���� FastCGI ���̹��������ݲ��������� php �ļ�����query �ַ�����HTTP ���󷽷���֮����һ���հ���� STDIN ��������������������ǿհ��壬����ֱ�ӷ��� type Ϊ FCGI_STDIN �� FCGI_Header �ṹ������ˡ�
```
// ���ȹ���һ��FCGI_BeginRequestRecord�ṹ
    FCGI_BeginRequestRecord beginRecord;
    beginRecord.header = 
        makeHeader(FCGI_BEGIN_REQUEST, FCGI_REQUEST_ID, sizeof(beginRecord.body), 0);
    beginRecord.body = makeBeginRequestBody(FCGI_RESPONDER);
    str_len = write(sock, &beginRecord, sizeof(beginRecord));
```

����Ĵ��빹��һ��FCGI_Header�ṹ���FCGI_BeginRequestBody�ṹ�����һ��FCGI_BEGIN_REQUEST��¼�����͸�FastCGI���̹�������ʾ��ʼ����
```
// ����FCGI_PARAMS����
    char *params[][2] = {
        {"SCRIPT_FILENAME", "/home/shiyanlou/php-server/test.php"}, 
        {"REQUEST_METHOD", "GET"}, 
        {"QUERY_STRING", "name=shiyanlou"}, 
        {"", ""}
    };

    int i, contentLength, paddingLength;
    FCGI_ParamsRecord *paramsRecordp;
    for(i = 0; params[i][0] != ""; i++){
        contentLength = strlen(params[i][0]) + strlen(params[i][1]) + 2; // 2�ֽ�����������ֵ
        paddingLength = (contentLength % 8) == 0 ? 0 : 8 - (contentLength % 8);
        paramsRecordp = (FCGI_ParamsRecord *)malloc(sizeof(FCGI_ParamsRecord) + contentLength + paddingLength);
        paramsRecordp->nameLength = (unsigned char)strlen(params[i][0]);    // ������ֵ
        paramsRecordp->valueLength = (unsigned char)strlen(params[i][1]);   // ��������
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

����ʼ��WEB ������ͨ������Ĵ����� FastCGI ���̹��������� FCGI_PARAMS ��¼������ FastCGI Э��涨���� 8 �ֽڶ��룬��Ҫ�����ݽ��� 8 �ֽڶ��롣

```
 // ����FCGI_STDIN����
    FCGI_Header stdinHeader;
    stdinHeader = makeHeader(FCGI_STDIN, FCGI_REQUEST_ID, 0, 0);
    write(sock, &stdinHeader, sizeof(stdinHeader));
```

������FCGI_PARAMS������ͨ������Ĵ�����FastCGI���̹���������һ���հ����FCGI_STDIN��¼����ʾ��������ϡ�
```
// ��ȡFastCGI���̹�������Ӧ������
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

����Ĵ������ȶ�ȡ FastCGI ���̹�������Ӧ���ݵ�ǰ 8 �ֽ����ݣ��� 8 �ֽ�����������һ�� FCGI_Header �ṹ�������Ȼ��ͨ���ж� type �Ƿ��� FastCGI ���̹�������Ӧ���ݡ�����ǣ���ȡ��Ӧ���ݳ��ȣ�֮�������ȡ�ó��ȵ����ݡ����ˣ�WEB �������� FastCGI ���̹������Ľ��������ˡ�

##����
���Ȱ�װ php-fpm��ʹ���������
```
sudo apt-get install php5-fpm
```
Ȼ���޸� php5-fpm �������ļ����� /etc/php5/fpm/pool.d/www.conf �ļ���

��λ���� 33 �У��� listen = /var/run/php5-fpm.sock ��Ϊ listen = 127.0.0.1:9000��Ȼ��ִ�������������� php-fpm��
```
sudo php5-fpm
```
ʹ�� netstat -apn | head -10�� �Բ鿴�Ƿ������ɹ���
FastCGI �� PHP-FPM �Ĺ�ϵ�ɲμ���http://segmentfault.com/q/1010000000256516