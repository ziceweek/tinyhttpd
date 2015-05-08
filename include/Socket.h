#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>
#define BUFSIZE 1024
#define PORT 4321
class Socket
{
    public:
        Socket();
        virtual ~Socket();

        void my_connect(std::string rmt_ip,int port);
        void my_listen(int port);
        void my_accept();

        std::string my_recv();
        void my_send(std::string str);

        void printRecv();

        int get_local_sockfd()
        {
            return _local_sockfd;
        }
        int get_rmt_sockfd()
        {
            return _rmt_sockfd;
        }
        void set_rmt_sockfd(int rmt_sockfd)
        {
            _rmt_sockfd = rmt_sockfd;
        }
        void close_rmt_sockfd()
        {
            close(_rmt_sockfd);
        }
    protected:
    private:
        int _local_sockfd;
        int _rmt_sockfd;

        struct sockaddr_in _localAddr;
        struct sockaddr_in _rmtAddr;
        //std::string host;
        int _port;
        std::string send_buf;
        std::string recv_buf;

};

#endif // SOCKET_H
