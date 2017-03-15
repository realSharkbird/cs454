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
char* CONTENT_TYPE_LOC_REQUEST = "3";
char* CONTENT_TYPE_LOC_SUCCESS = "4";
char* CONTENT_TYPE_LOC_FAILURE = "5";
char* CONTENT_TYPE_EXECUTE = "6";
char* CONTENT_TYPE_EXECUTE_SUCCESS = "7";
char* CONTENT_TYPE_EXECUTE_FAILURE = "8";
char* CONTENT_TYPE_TERMINATE = "9";
char* CONTENT_TYPE_INIT = "10";

//types of errors
int ERROR_SOCKET_CREATION_FAILED = -1;
int ERROR_BINDER_NOT_SET = -2;