#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <map>
class HTTP
{
    public:
        HTTP();
        virtual ~HTTP();
        void parse(std::string request);
        void parse(char *request_row);
        void toString();

        std::string getMethod(){return _method;}
        std::string getPath(){return _path;}
        std::string getQuery_string(){return _query_string;}
        std::string getVersion(){return _version;}

    protected:
    private:
        std::string _method;
        std::string _path;
        std::string _query_string;
        std::string _version;

        //std::string _header_Host;
        //std::string _header_Connection;
        //std::string _header_Accept;
        //std::string _header_User_Agent;
        //std::string _header_Accept_Encoding;
        //std::string _header_Accept_Language;
        std::map<std::string,std::string> _header;

};

#endif // HTTP_H
