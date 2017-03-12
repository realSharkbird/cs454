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
extern char* CONTENT_TYPE_LOC_REQUEST;
extern char* CONTENT_TYPE_LOC_SUCCESS;
extern char* CONTENT_TYPE_LOC_FAILURE;
extern char* CONTENT_TYPE_EXECUTE;
extern char* CONTENT_TYPE_EXECUTE_SUCCESS;
extern char* CONTENT_TYPE_EXECUTE_FAILURE;
extern char* CONTENT_TYPE_TERMINATE;

//this is here to make debugging easier so we can switch debug messages on/off
//we will turn this off before submitting
#define DEBUG //* comment out to turn off

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << "(DEBUG): " << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

//make it easier to get typenames
template <typename T>
class Foo
{ };

template <int N>
struct select_type;

template<>
struct select_type<ARG_CHAR>
{
    typedef char type;
};

template<>
struct select_type<ARG_SHORT>
{
    typedef short type;
};

template<>
struct select_type<ARG_INT>
{
    typedef int type;
};

template<>
struct select_type<ARG_LONG>
{
    typedef long type;
};

template<>
struct select_type<ARG_DOUBLE>
{
    typedef double type;
};

template<>
struct select_type<ARG_FLOAT>
{
    typedef float type;
};