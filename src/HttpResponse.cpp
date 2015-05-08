#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
    //ctor
}

HttpResponse::~HttpResponse()
{
    //dtor
}

std::string HttpResponse::toString()
{
    //响应行
    buf.append(_version);
    buf.append(" ");
    //std::cout<<buf<<std::endl;
    buf.append(_statusCode);
    buf.append(" ");
    //std::cout<<buf<<std::endl;
    buf.append(_statusMessage);
    buf.append("\r\n");
    //std::cout<<buf<<std::endl;

    //响应头
    for(std::map<std::string,std::string>::const_iterator it=_header.begin();it!=_header.end();++it)
    {
        buf.append(it->first);
        buf.append(":");
        buf.append(it->second);
        buf.append("\r\n");
    }

    //响应头和主体之间的空格
    buf.append("\r\n");

    //响应主体
    //if(_statusCode=="200")
    //    buf.append(_body);
    //else

    if(_statusCode== "200")buf.append(_body);
    else if(_statusCode== "204")buf.append("<h1>No Content</h1>");
    else if(_statusCode== "206")buf.append("<h1>Partial Content</h1>");
    else if(_statusCode== "301")buf.append("<h1>Moved Permanently</h1>");
    else if(_statusCode== "302")buf.append("<h1>Found</h1>");
    else if(_statusCode== "303")buf.append("<h1>See Other</h1>");
    else if(_statusCode== "304")buf.append("<h1>Not Modelse ified</h1>");
    else if(_statusCode== "400")buf.append("<h1>Bad Request</h1>");
    else if(_statusCode== "401")buf.append("<h1>Unauthorized</h1>");
    else if(_statusCode== "403")buf.append("<h1>Forrbidden</h1>");
    else if(_statusCode== "404")buf.append("<h1>Not Found</h1>");
    else if(_statusCode== "500")buf.append("<h1>Internal Server Error</h1>");
    else if(_statusCode== "503")buf.append("<h1>Service</h1>");

    return buf;



}
