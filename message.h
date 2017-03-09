//class for messaging protocols

//types of message content
const int CONTENT_TYPE_REGISTER = 0;
const int CONTENT_TYPE_LOC_REQUEST = 1;
const int CONTENT_TYPE_LOC_SUCCESS = 2;
const int CONTENT_TYPE_LOC_FAILURE = 3;
const int CONTENT_TYPE_EXECUTE = 4;
const int CONTENT_TYPE_EXECUTE_SUCCESS = 5;
const int CONTENT_TYPE_EXECUTE_FAILURE = 6;
const int CONTENT_TYPE_TERMINATE = 7;

class MessageContent{
    
    int content_type;
    std::string message;
};

class ServerBinderMessage: public MessageContent {
    std::string server_identifier, name;
    int port;
    void* argTypes;
};

class ClientBinderMessage: public MessageContent {
    std::string name;
    void* argTypes;

};

class ClientServerMessage: public MessageContent {
    
};

//types of messages
const int TYPE_SERVER_BINDER_MESSAGE = 0;
const int TYPE_CLIENT_BINDER_MESSAGE = 1;
const int TYPE_CLIENT_SERVER_MESSAGE = 2;
const int TYPE_TERMINATE_MESSAGE = 3;

class Message{
    
    int length, type;
    MessageContent messageContent;
    
public:
    Message();
    
};