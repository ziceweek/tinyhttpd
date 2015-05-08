#include "CGI.h"
#include<stdio.h>
#include <string.h>
CGI::CGI(HTTP &httpRequest)
{
    //ctor
    _htdocs = "./htdocs";
    _interpreter = "/bin/python";
    _httpRequest = httpRequest;
    _cgi_permission = 0;
    _body = "";
}

CGI::~CGI()
{
    //dtor
}


std::string CGI::handle()
{


    string real_path = _htdocs + _httpRequest.getPath();
    std::cout<<"real_path:"<<real_path<<std::endl;

    if(real_path[real_path.size()- 1] == '/')  //以/结尾的加上index.html
        real_path += "index.html";

    std::cout<<"real_path:"<<real_path<<std::endl;

    if (stat(real_path.c_str(), &st) == 0)
    {
        std::cout<<real_path<<" is existed."<<std::endl;
        //Is it dir?
        if ((st.st_mode & S_IFMT) == S_IFDIR)
            real_path += "/index.html";

        //permission?
        if ((st.st_mode & S_IXUSR) ||(st.st_mode & S_IXGRP) ||(st.st_mode & S_IXOTH))       //可执行
            _cgi_permission = 1;

        std::cout<<"permission is "<<_cgi_permission<<std::endl;
        //cgi?
        if (!_cgi_permission)
        {
            cat_file(real_path);
        }

        else
        {
            execute_cgi(real_path, _httpRequest.getMethod(), _httpRequest.getQuery_string());
        }


    }
    else
    {
        not_found();
    }

    return real_path;

}


void CGI::not_found()
{
    _httResponse.setStatusCode("404");
}


void CGI::cat_file(std::string filename)
{
    FILE *resource = NULL;
    std::string str;
    resource = fopen(filename.c_str(), "r");
    if (resource == NULL)
    {
        std::cout<<"fail to open file:"<<filename<<std::endl;
        _httResponse.setStatusCode("404");
    }
    else
    {
        std::cout<<"success to open file:"<<filename<<std::endl;
        char buf[1024];

        fgets(buf, sizeof(buf), resource);
        while (!feof(resource))
        {
            std::string tmp(buf);
            str += tmp;
            fgets(buf, sizeof(buf), resource);
        }
    }
    fclose(resource);
    _httResponse.setStatusCode("200");
    _body = str;
    std::cout<<filename<<"'s content is "<<_body<<std::endl;
    //_httResponse.setBody(str);

    //return str;
}

std::string CGI::result()
{
    //std::cout<<"result"<<std::endl;
    _httResponse.setVersion("HTTP/1.1");
    _httResponse.setConnection("Close");
    _httResponse.setServer("icehttpd/0.1.0");
    _httResponse.setContentType("text/html");

    std::cout<<"_body:"<<_body<<std::endl;
    _httResponse.setBody(_body);

    return _httResponse.toString();
}



void CGI::execute_cgi(std::string real_path,std::string method, std::string query_string)
{
    //char buf[1024];
    int cgi_output[2];
    int cgi_input[2];
    int status;
    pid_t pid;

    std::cout<<"execute_cgi()"<<std::endl;
    if (pipe(cgi_output) < 0||pipe(cgi_input) < 0||(pid = fork() ) < 0)
    {
        _httResponse.setStatusCode("505");
    }



    if (pid == 0)  /* child: CGI script */
    {
        char meth_env[255];
        char query_env[255];
        //char length_env[255];

        dup2(cgi_output[1], 1);
        dup2(cgi_input[0], 0);
        close(cgi_output[0]);
        close(cgi_input[1]);

        sprintf(meth_env, "REQUEST_METHOD=%s", method.c_str());
        putenv(meth_env);
        if (method=="GET")
        {
            sprintf(query_env, "QUERY_STRING=%s", query_string.c_str());
            putenv(query_env);
        }
        else     /* POST */
        {
            //sprintf(length_env, "CONTENT_LENGTH=%d", _httpRequest. ontent_length.c_str());
            //putenv(length_env);
        }
        execl(_interpreter.c_str(),_interpreter.c_str(), real_path.c_str(), NULL);
        exit(0);
    }
    else        /* parent */
    {
        close(cgi_output[1]);
        close(cgi_input[0]);
        std::string output;
        char buf[1024];

        while (read(cgi_output[0], buf, 1024) > 0)
        {
            std::string tmp(buf);
            output += tmp;
        }
        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);

        _body = output;
        _httResponse.setStatusCode("200");
        //std::cout<<_body<<std::endl;
    }
}
