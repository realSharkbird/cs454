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
char* CONTENT_TYPE_LOC_REQUEST = "1";
char* CONTENT_TYPE_LOC_SUCCESS = "2";
char* CONTENT_TYPE_LOC_FAILURE = "3";
char* CONTENT_TYPE_EXECUTE = "4";
char* CONTENT_TYPE_EXECUTE_SUCCESS = "5";
char* CONTENT_TYPE_EXECUTE_FAILURE = "6";
char* CONTENT_TYPE_TERMINATE = "7";