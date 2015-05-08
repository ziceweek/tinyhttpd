#include "HTTP.h"
#include <string>
#include <iostream>
#include <Socket.h>
#include <HTTP.h>
#include <unistd.h>
#include <HttpResponse.h>
#include <ProcessPoll.h>
using namespace std;

void test_HTTP()
{
    HTTP hp;
    string request_row = "GET\r/index.html?is=sdf&sf\rHTTP/1.0\r\n";
    hp.parse(request_row);
    hp.toString();
}

void try_server()
{
    Socket sock;
    sock.my_listen(4321);
    sock.my_accept();
    std::string s = sock.my_recv();

    HTTP httpParse;
    httpParse.parse(s);
    httpParse.toString();
    //std::cout<<s.c_str();
    //sock.printRecv();
}

void try_client()
{
    Socket sock;
    sock.my_connect("127.0.0.1",4321);
    sock.my_send("GET\r/index.html\rHTTP/1.0\r\n");
    sock.my_recv();
    sock.printRecv();
}

//一个连接一个进程
void try_server_2()
{
    Socket sock;
    sock.my_listen(4321);

    for(;;)
    {
        sock.my_accept();
        if(fork()==0)
        {

            std::string s = sock.my_recv();

            HTTP httpParse;
            httpParse.parse(s);
            httpParse.toString();

            HttpResponse re;
            re.setVersion("HTTP/1.1");
            re.setStatusCode("200");
            re.setStatusMessage("OK");

            re.setConnection("Close");
            re.setServer("jdbhttpd/0.1.0");
            re.setContentType("text/html");
            re.setBody("<html><h1>this is a re!</h1></html>");

            std::string sendStr = re.toString();

            sock.my_send(sendStr);
            //exit(0)
        }
    }




}


//----------------------

//进程池2select
void tryProcessPoll2()
{
    ProcessPoll poll(5,4321);
    poll.initProcessPoll();
    poll.start_Mng();
}
int main()
{
    //try_client();
    //try_server_2();
    //test_HTTP();

    tryProcessPoll2();
}
