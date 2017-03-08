//NOTE: I used some of this code from my assignment #2.


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max

using namespace std;

//This binder is the first thing to run
int main(){
    
    //define socket variables
    int socketDescriptor, newsocketDescriptor;
    socklen_t clientLen;
    char buffer[256];
    struct sockaddr_in server_address, client_address;
    int n;
    
    //setup sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(DEFAULT_PORT);
    if (bind(socketDescriptor, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
        cout << "error on binding" << endl;
    listen(socketDescriptor,MULTIPLEX);
    clientLen = sizeof(server_address);
    
    //print out server and port numbers
    if (getsockname(socketDescriptor, (struct sockaddr *)&server_address, &clientLen) < 0){
        cout << "failed to get hostname with errno: " << endl;
        exit(1);
    }else{
        
        char hostname[1024];
        gethostname(hostname, 1024);
        
        cout << "SERVER_ADDRESS " << hostname << endl;
        cout << "SERVER_PORT " << ntohs(server_address.sin_port) << endl;
    }
    
    return 0;
}