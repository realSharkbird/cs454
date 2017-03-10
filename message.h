#include <string>
#include <stdlib.h>

//class for messaging protocols
//types of messages
extern char* TYPE_SERVER_BINDER_MESSAGE;
extern char* TYPE_CLIENT_BINDER_MESSAGE;
extern char* TYPE_CLIENT_SERVER_MESSAGE;
extern char* TYPE_TERMINATE_MESSAGE;

//types of message content
extern char* CONTENT_TYPE_REGISTER;
extern char* CONTENT_TYPE_LOC_REQUEST;
extern char* CONTENT_TYPE_LOC_SUCCESS;
extern char* CONTENT_TYPE_LOC_FAILURE;
extern char* CONTENT_TYPE_EXECUTE;
extern char* CONTENT_TYPE_EXECUTE_SUCCESS;
extern char* CONTENT_TYPE_EXECUTE_FAILURE;
extern char* CONTENT_TYPE_TERMINATE;

class Message{
    
public:
    int length, type, content_type;
    char* content;
    Message(int, char*);
    
};