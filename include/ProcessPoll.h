#ifndef PROCESSPOLL_H
#define PROCESSPOLL_H

#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <Socket.h>
#include <iostream>
#include "HTTP.h"
#include "HttpResponse.h"
#include "CGI.h"

//控制缓存区的大小
#define CONTROLLEN CMSG_LEN(sizeof(int))

class ProcessPoll
{
public:

    //资源进程管理的结构
    typedef struct
    {
        int pid;
        int pipefd;
        int status;
    }T_ProcessStatus;


    //要建立的进程数和监听的端口号
    ProcessPoll(int processNum,int port);
    virtual ~ProcessPoll();

    //初始化进程池
    void initProcessPoll();

    //开始监听并将连接描述符传给子进程
    void start_Mng();




private:

    int _maxProcessNum;//最大进程数
    T_ProcessStatus *_pProcessMng;//进程表，记录进程信息
    int _navail;//当前可用的子进程数

    fd_set _rset;
    fd_set _masterset;//包括listenfd和各个与子进程通讯的管道
    int _maxfd;//最大的描述符
    int _nsel;//返回的select集中可用的个数

    Socket _sock;
    //int _port;//监听的端口
    int _listenfd;//由父进程监听

    //创建一个工作进程进程
    pid_t createWorker(int i);
    //处理客户端的请求
    void server(int connfd);

    //将描述符fd_to_send发送到文件fd
    int send_fd(int fd,int fd_to_send);
    //从文件fd接收描述符号
    int recv_fd(int fd);



};


#endif // PROCESSPOLL_H
