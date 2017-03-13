#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max
#define MESSAGE_BUFFER_SIZE 8192
#define MAX_HOSTNAME_LEN 255

class Socket{
    //declare socket variables
    int socketDescriptor, newSocketDescriptor;
    char port[10];
    char hostname[MAX_HOSTNAME_LEN];
    char* buffer;
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