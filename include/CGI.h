#ifndef CGI_H
#define CGI_H
#include <string>
#include <sys/stat.h>
#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include "HTTP.h"
#include "HttpResponse.h"
#include "CGI.h"
using namespace std;

class CGI
{
    public:
        CGI();
        CGI(HTTP &httpRequest);
        virtual ~CGI();

        std::string handle();
        std::string result();
    protected:
    private:
        std::string _interpreter;
        std::string _htdocs;
        struct stat st;
        int _cgi_permission;
        std::string _body;

        HTTP _httpRequest;
        HttpResponse _httResponse;


        void execute_cgi(std::string real_path,std::string method, std::string query_string);
        void not_found();
        void cat_file(std::string real_path);
};

#endif // CGI_H
