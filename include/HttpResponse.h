#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>
#include <stdio.h>
#include <iostream>

class HttpResponse
{
    public:
        HttpResponse();
        virtual ~HttpResponse();


        //响应行
        void setVersion(const std::string &version)
        {
            _version = version;
        }
        void setStatusCode(std::string code)
        {
            _statusCode = code;

            if(_statusCode== "200")setStatusMessage("OK");
            else if(_statusCode== "204")setStatusMessage("No Content");
            else if(_statusCode== "206")setStatusMessage("Partial Content");
            else if(_statusCode== "301")setStatusMessage("Moved Permanently");
            else if(_statusCode== "302")setStatusMessage("Found");
            else if(_statusCode== "303")setStatusMessage("See Other");
            else if(_statusCode== "304")setStatusMessage("Not Modified");
            else if(_statusCode== "400")setStatusMessage("Bad Request");
            else if(_statusCode== "401")setStatusMessage("Unauthorized");
            else if(_statusCode== "403")setStatusMessage("Forrbidden");
            else if(_statusCode== "404")setStatusMessage("Not Found");
            else if(_statusCode== "500")setStatusMessage("Internal Server Error");
            else if(_statusCode== "503")setStatusMessage("Service");
            else setStatusMessage("Unknown");
        };
        void setStatusMessage(const std::string &message)
        {
            _statusMessage = message;
        };

        //响应首部

        //通用首部字段
        void setCacheControl(const std::string &cacheControl)
        {
            add_Header("Cache-Control",cacheControl);
        }
        void setConnection(const std::string &connection)
        {
            add_Header("Connection",connection);
        }

        void setDate(const std::string &date)
        {
             add_Header("Date",date);
        }

        //响应首部
        void setAcceptRanges(const std::string &acceptRanges)
        {
             add_Header("Accept-Ranges",acceptRanges);
        }

        void setLocation(const std::string &location)
        {
            add_Header("Location",location);
        }

        void setServer(const std::string &server)
        {
            add_Header("Server",server);
        }

        //实体首部
        void setContentType(const std::string &contentType)
        {
            add_Header("Content-Type",contentType);
        }

        void setAllow(const std::string &allow)
        {
            add_Header("Allow",allow);
        }

        void setContentEncoding(const std::string &contentEncoding)
        {
            add_Header("Content-Encoding",contentEncoding);
        }

        void setContentLanguage(const std::string &contentLanguage)
        {
            add_Header("Content-Language",contentLanguage);
        }

        void setContentLength(const std::string contentLength)
        {
            add_Header("Content-Length",contentLength);
        }

        //设置主体

        void setBody(const std::string &body)
        {
            _body = body;
        }

        std::string toString();


    protected:
    private:
        void add_Header(const std::string &key,const std::string &values)
        {
            _header[key] = values;
        }

        std::map<std::string,std::string> _header;
        std::string _version;
        std::string _statusCode;
        std::string _statusMessage;

        std::string _body;

        std::string buf;

};

#endif // HTTPRESPONSE_H
