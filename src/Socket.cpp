#include "Socket.h"
#include <stdlib.h>
Socket::Socket()
{
    _port = PORT;
    _local_sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(_local_sockfd<0)
    {
        std::cout<<"Return "<<_local_sockfd<<", init socket fail!"<<std::endl;//错误代码存于errno中
        exit(0);
    }
}

Socket::~Socket()
{
    //dtor
    close(_local_sockfd);
    close(_rmt_sockfd);
}

void Socket::my_listen(int port)
{
    _port = port;
    memset(&_localAddr,0,sizeof(sockaddr_in));
    _localAddr.sin_family = AF_INET;
    _localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _localAddr.sin_port = htons(_port);

    if( bind(_local_sockfd,(struct sockaddr*)&_localAddr,sizeof(_localAddr))<0)
    {
        std::cout<<"Fail to bind port "<<_port<<" to the fd:"<<_local_sockfd<<std::endl;
        exit(0);
    }
    if( listen(_local_sockfd,1024)<0 ) //BSD:5
    {
        std::cout<<"Fail to listen on fd:"<<_local_sockfd<<std::endl;
        exit(0);
    }

}

//阻塞
void Socket::my_accept()
{
    socklen_t _rmtAddrLen = sizeof(_rmtAddr);
    //std::cout<<"waitting for connections: "<<std::endl;
    _rmt_sockfd = accept(_local_sockfd,(struct sockaddr*)&_rmtAddr, &_rmtAddrLen);

    if(_rmt_sockfd<0)
    {
        std::cout<<"Fail to accept on listenfd:"<<_local_sockfd<<std::endl;
        exit(0);
    }else{
        std::string peer_ip(inet_ntoa(_rmtAddr.sin_addr));
        std::cout<<"Connection from "<<peer_ip<<":"<<_rmtAddr.sin_port<<" on fd:"<<_rmt_sockfd<<std::endl;
    }


}

void Socket::my_connect(std::string rmt_ip,int port)
{
    memset(&_rmtAddr,0,sizeof(sockaddr_in));
    _rmtAddr.sin_family = AF_INET;
    _rmtAddr.sin_port = htons(port);
    int i = inet_pton(AF_INET,rmt_ip.c_str(),&_rmtAddr.sin_addr.s_addr);
    if(i<0)
    {
        std::cout<<"Fail to transform the ip from Binary Notation to Dotted Decimal Notation"<<std::endl;
        exit(0);
    }
    socklen_t _rmtAddrLen = sizeof(_rmtAddr);
    if( connect(_local_sockfd,(struct sockaddr*)&_rmtAddr,_rmtAddrLen)<0 )
    {
        std::cout<<"Fail to connect to "<<rmt_ip<<":"<<port<<std::endl;
        exit(0);
    }
}

//适用于接收HTTP报文，阻塞
std::string Socket::my_recv()
{
    char buf[BUFSIZE];
    recv_buf = "";
    int n = 0;
    while((n = recv(_rmt_sockfd, buf, BUFSIZE, 0))>0)
    {
        //if (buf[n-1]=='\n' && buf[n-2]=='\r')
        //{
            //有bug，每一行的结束都是\r\n，如果刚好读入的结尾是某一行尾，就会错判为整个报文结束
            recv_buf.append(buf);
            return recv_buf;
        //}
    }
    if (n < 0)
        std::cout<<"recv error"<<std::endl;
    return recv_buf;
}

void Socket::my_send(std::string str)
{
    //const char *buf;
    //buf = str.c_str();
    if(send(_rmt_sockfd,str.c_str(),str.size(),0)<0)
        std::cout<<"Fail to sent '"<<str<<"'"<<std::endl;
}

void Socket::printRecv()
{
    std::cout<<"recv_buf:"<<recv_buf<<std::endl;
}
