#include "HTTP.h"
#include <string>
#include <iostream>
HTTP::HTTP()
{
    //ctor
}

HTTP::~HTTP()
{
    //dtor
}



void HTTP::parse(std::string request)
{

    if(request=="")
        return;

    std::string::size_type s=0,e=0;

    //get request method
    s = request.find_first_not_of(' ',s);
    e = request.find_first_of(' ',s);
    _method = request.substr(s,(e-s));


    //get url
    s = request.find_first_not_of(' ',e+1);
    e = request.find_first_of(' ',s);
    std::string url = request.substr(s,(e-s));

    //get arg form url
    if (_method=="GET")
    {
        std::string::size_type url_s=0,url_e=0;
        url_s = url.find_first_of('?',0);
        if(url_s!=std::string::npos)
        {
            url_e = url.find_first_of(' ',url_s);
            _query_string = url.substr(url_s,(url_e-url_s));
            _path = url.substr(0,url_s);
         }else{
            _path = url;
         }
    }

    //get the HTTP version
    s = request.find_first_not_of(' ',e);
    e = request.find_first_of('\r',s);
    _version = request.substr(s,(e-s));

     //请求报文头
//    s = e + 1;
//    std::string::size_type sp=0;
//    while( (s = request.find_first_not_of('\n',s))!=std::string::npos )
//    {
//        if( (e = request.find_first_of('\r',s))!=std::string::npos )
//        {
//            if( (sp = request.find_first_of(':',s))!=std::string::npos )
//
//        }
//
//    }

}

void HTTP::toString()
{
    std::cout<<"method:"<<_method<<","<<std::endl;
    std::cout<<"path:"<<_path<<","<<std::endl;
    std::cout<<"argc:"<<_query_string<<","<<std::endl;
    std::cout<<"version:"<<_version<<","<<std::endl;
}
