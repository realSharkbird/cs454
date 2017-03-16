#include "utils.h"
#include <string>

using namespace std;

//class for messaging protocols

//types of messages
char* TYPE_SERVER_BINDER_MESSAGE = "0";
char* TYPE_CLIENT_BINDER_MESSAGE = "1";
char* TYPE_CLIENT_SERVER_MESSAGE = "2";
char* TYPE_TERMINATE_MESSAGE = "3";

//types of message content
char* CONTENT_TYPE_REGISTER = "0";
char* CONTENT_TYPE_REGISTER_SUCCESS = "1";
char* CONTENT_TYPE_REGISTER_FAILURE = "2";
char* CONTENT_TYPE_REGISTER_WARNING = "3";
char* CONTENT_TYPE_LOC_REQUEST = "4";
char* CONTENT_TYPE_LOC_SUCCESS = "5";
char* CONTENT_TYPE_LOC_FAILURE = "6";
char* CONTENT_TYPE_EXECUTE = "7";
char* CONTENT_TYPE_EXECUTE_SUCCESS = "8";
char* CONTENT_TYPE_EXECUTE_FAILURE = "9";
char* CONTENT_TYPE_TERMINATE = "10";
char* CONTENT_TYPE_INIT = "11";

//types of errors
int ERROR_SOCKET_CREATION_FAILED = -1;
int ERROR_BINDER_NOT_SET = -2;
int ERROR_LOC_DATABASE_NULL = -3;
int ERROR_LOC_NOT_FOUND = -4;
int ERROR_EXECUTE_FAILURE = -5;
int ERROR_SOCKET_READ_FAILED = -6;
int ERROR_SOCKET_WRITE_FAILED = -7;
int ERROR_SOCKET_ACCEPT_FAILED = -8;
int ERROR_SOCKET_CONNECT_FAILED = -9;

//we need this tedious code to make the database work
bool Procedure::operator<(const Procedure right) const{
    
    if(strcmp(name, right.name) == 0){
        
        bool result = (strcmp((char*)argTypes, (char*)right.argTypes) < 0);
        
        return result;
        
    }
    
    bool result = (strcmp(name, right.name) < 0);
    
    return result;
    
}
//we need this tedious code to make the database work
bool Location::operator<(const Location right) const{
    
    string l1 = port + ip;
    string l2 = right.port + right.ip;
    bool result = (strcmp(l1.c_str(), l2.c_str()) < 0);
    
    return result;
    
}