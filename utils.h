#include <string>
#include <stdlib.h>
#include "rpc.h"

//the purpose of this file is to make things easier

//types of messages
extern char* TYPE_SERVER_BINDER_MESSAGE;
extern char* TYPE_CLIENT_BINDER_MESSAGE;
extern char* TYPE_CLIENT_SERVER_MESSAGE;
extern char* TYPE_TERMINATE_MESSAGE;

//types of message content
extern char* CONTENT_TYPE_REGISTER;
extern char* CONTENT_TYPE_REGISTER_SUCCESS;
extern char* CONTENT_TYPE_REGISTER_FAILURE;
extern char* CONTENT_TYPE_LOC_REQUEST;
extern char* CONTENT_TYPE_LOC_SUCCESS;
extern char* CONTENT_TYPE_LOC_FAILURE;
extern char* CONTENT_TYPE_EXECUTE;
extern char* CONTENT_TYPE_EXECUTE_SUCCESS;
extern char* CONTENT_TYPE_EXECUTE_FAILURE;
extern char* CONTENT_TYPE_TERMINATE;
extern char* CONTENT_TYPE_INIT;

extern int ERROR_BINDER_NOT_SET;
extern int ERROR_SOCKET_CREATION_FAILED;
extern int ERROR_LOC_DATABASE_NULL;
extern int ERROR_LOC_NOT_FOUND;
extern int ERROR_EXECUTE_FAILURE;
extern int ERROR_SOCKET_READ_FAILED;
extern int ERROR_SOCKET_WRITE_FAILED;
extern int ERROR_SOCKET_ACCEPT_FAILED;
extern int ERROR_SOCKET_CONNECT_FAILED;

//this is here to make debugging easier so we can switch debug messages on/off
//we will turn this off before submitting
//#define DEBUG //* comment out to turn off

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << "(DEBUG): " << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

class Procedure {
    
public:
    bool operator<(const Procedure rhs) const;
    char * name;
    int * argTypes;
};

class Location {
    
public:
    bool operator<(const Location rhs) const;
    std::string ip;
    std::string port;
};
