#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max

class Socket{
    //declare socket variables
    int socketDescriptor, newsocketDescriptor;
    socklen_t clientLen, serverLen;
    char buffer[65536];
    struct sockaddr_in server_address, client_address;
    int n;
    
public:
    Socket();
    Socket(int, int);
    Socket(std::string, std::string);
    void printLocation(std::string);
    void* readMessage();
    void writeMessage(void*, std::string, std::string);
    
};