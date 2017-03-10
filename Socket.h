#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max
#define MESSAGE_BUFFER_SIZE 16777216
#define MAX_HOSTNAME_LEN 1024

class Socket{
    //declare socket variables
    int socketDescriptor, newSocketDescriptor;
    socklen_t clientLen, serverLen;
    struct sockaddr_in server_address, client_address;
    int n;
    
public:
    Socket();
    Socket(int, int);
    Socket(std::string, std::string);
    void listenForConnection();
    void printLocation(std::string);
    char* getLocationAddress();
    char* getLocationPort();
    char* readMessage();
    void writeMessage(void*, int);
    void closeSocket();

};