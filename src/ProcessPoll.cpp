#include "ProcessPoll.h"

int max(int a,int b)
{
    return a>b?a:b;
}


ProcessPoll::ProcessPoll(int processNum,int port)
{
    //开始监听
    _sock.my_listen(port);
    _listenfd = _sock.get_local_sockfd();
    _maxfd = _listenfd;

    _maxProcessNum = processNum;
    _navail = _maxProcessNum;
    _pProcessMng =(T_ProcessStatus*) calloc(_maxProcessNum,sizeof(T_ProcessStatus));
}

ProcessPoll::~ProcessPoll()
{
    //dtor
}

//资源进程处理任务
void ProcessPoll::server(int connfd)
{
    //现在在子进程里取得了需要处理的消息，可以开始处理该消息直到消息处理完成

    Socket sock;
    sock.set_rmt_sockfd(connfd);

    std::string r = sock.my_recv();
    std::cout<<"server recv:"<<std::endl;

    if(r!="")
    {
        HTTP httpRequest;
        httpRequest.parse(r);
        httpRequest.toString();

        CGI cgi(httpRequest);
        cgi.handle();
        std::cout<<"handle() done"<<std::endl;
        std::string sendStr = cgi.result();
        std::cout<<sendStr<<std::endl;
        sock.my_send(sendStr);
        //sock.close_rmt_sockfd();
    }
    return;
}


//创建一个工作进程进程
pid_t ProcessPoll::createWorker(int i)
{
    int sockfd[2];
    pid_t pid;

    socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd);

    //父进程：记录子进程信息
    if((pid=fork())>0)
    {
        close(sockfd[1]);
        _pProcessMng[i].pid = pid;
        _pProcessMng[i].pipefd = sockfd[0];
        _pProcessMng[i].status = 0;
        return pid;
    }

    //子进程：从父进程接受描述符，提供服务
    close(sockfd[0]);
    close(_listenfd);

    int connfd;
    for(;;)
    {
        connfd = recv_fd(sockfd[1]);
        if(connfd>0)
            server(connfd);
        close(connfd);
        write(sockfd[1],"",1);//通知父进程子进程已经完成工作
    }

}

//初始化进程池
void ProcessPoll::initProcessPoll()
{

    FD_ZERO(&_masterset);
    FD_SET(_listenfd,&_masterset);


    //创建_maxProcessNum个子进程
    for(int i=0;i<_maxProcessNum;i++)
    {
        createWorker(i);
        FD_SET(_pProcessMng[i].pipefd,&_masterset);
        _maxfd = max(_maxfd,_pProcessMng[i].pipefd);
    }


    //start_Mng();

}

//开始监听并将连接描述符传给子进程
void ProcessPoll::start_Mng()
{
    //主进程循环accept并将confd发送到空闲的子进程中
    for(;;)
    {
        _rset = _masterset;
        if(_navail<=0)
        {
            FD_CLR(_listenfd,&_rset);
            std::cout<<"没有可用子进程了,请等待"<<std::endl;
        }
        int connfd;

        //阻塞于select上
        _nsel = select(_maxfd+1,&_rset,NULL,NULL,NULL);

        //监听套接字可用，即新连接到来，accept之
        int i;//可用的进程序号
        if(FD_ISSET(_listenfd,&_rset))
        {
            _sock.my_accept();
            connfd = _sock.get_rmt_sockfd();
            for(i = 0;i<_maxProcessNum;i++)
            {
                if(_pProcessMng[i].status==0)
                    break;
            }
            if(i==_maxProcessNum)
            {
                std::cout<<"没有可用子进程了"<<std::endl;
                exit(0);
            }

            //将选中的空闲进程设为busy(1)，将连接套接字发送过去，然后关闭本进程中的套接字
            _pProcessMng[i].status = 1;
            _navail--;
            send_fd(_pProcessMng[i].pipefd,connfd);
            close(connfd);

            if(--_nsel==0)
                continue;

        }

        //回收可用子进程
        char rc;
        for(i = 0;i<_maxProcessNum;i++)
        {
            if(FD_ISSET(_pProcessMng[i].pipefd,&_rset))
            {
                if(read(_pProcessMng[i].pipefd,&rc,1)==0)
                {
                    std::cout<<"child terminated!"<<std::endl;
                    exit(0);
                }
                _pProcessMng[i].status = 0;
                _navail++;

                //?????
                if(--_nsel==0)
                    break;
            }
        }
    }
}


//将描述符fd_to_send发送到文件fd
int ProcessPoll::send_fd(int fd, int fd_to_send)
{
    struct iovec iov[1];
    struct msghdr msg;
    char buf[1];

    iov[0].iov_base = buf;
    iov[0].iov_len = 1;

    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    cmsghdr cm;
    cm.cmsg_len = CONTROLLEN;
    cm.cmsg_level = SOL_SOCKET;
    cm.cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(&cm) = fd_to_send;

    msg.msg_control = &cm;
    msg.msg_controllen = CONTROLLEN;

    int i = sendmsg(fd,&msg,0);
    if(i<0)
    {
        std::cout<<"sendmsg() return:"<<i<<" send "<<fd_to_send<<" to "<<fd<<std::endl;
    }

    return i;

}

//从文件fd接收描述符号
int ProcessPoll::recv_fd(int fd)
{

    char buf[1];
    struct iovec iov[1];
    struct msghdr msg;

        iov[0].iov_base = buf;
        iov[0].iov_len = sizeof(buf);

        msg.msg_iov = iov;
        msg.msg_iovlen = 1;
        msg.msg_name = NULL;
        msg.msg_namelen = 0;


        cmsghdr cm;

        msg.msg_control = &cm;
        msg.msg_controllen = CONTROLLEN;

        recvmsg(fd,&msg,0);

        int fd_to_read = *(int*)CMSG_DATA((cmsghdr *)msg.msg_control);
        return fd_to_read;

}




