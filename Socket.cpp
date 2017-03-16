/*
 * Custom socket class to make sockets easier to use.
 * NOTE: I used some of this code from my assignment #2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <cassert>
#include <netdb.h>
#include "Socket.h"
#include "utils.h"
#include <sstream>

using namespace std;

//create a server socket
Socket::Socket(){
    //setup sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        exit(ERROR_SOCKET_CREATION_FAILED);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(DEFAULT_PORT);

    if (::bind(socketDescriptor, (struct sockaddr *) &server_address,
               sizeof(server_address)) < 0)
        exit(ERROR_SOCKET_CREATION_FAILED);
    listen(socketDescriptor,MULTIPLEX);
    serverLen = sizeof(server_address);
    
    //print out server and port numbers
    if (getsockname(socketDescriptor, (struct sockaddr *)&server_address, &serverLen) < 0){
        exit(ERROR_SOCKET_CREATION_FAILED);
    }
}

//create a client socket
Socket::Socket(string address, string port) {

    //variables
    int portNum, n;
    struct hostent *server;

    //initialize sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    newSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    portNum = atoi(port.c_str());
    if (newSocketDescriptor < 0)
        exit(ERROR_SOCKET_CREATION_FAILED);
    server = gethostbyname(address.c_str());
    if (server == NULL || portNum == 0) {
        exit(ERROR_SOCKET_CREATION_FAILED);
    }

    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(portNum);
    if (connect(newSocketDescriptor,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
        exit(ERROR_SOCKET_CONNECT_FAILED);
}

//listen for connection
void Socket::listenForConnection(){
    
    //wait for requests
    clientLen = sizeof(client_address);
    newSocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &client_address, &clientLen);
    if (newSocketDescriptor < 0)
        exit(ERROR_SOCKET_ACCEPT_FAILED);
    
    
}

char* Socket::getLocationAddress(){
    gethostname(hostname, MAX_HOSTNAME_LEN);
    
    return hostname;
}

char* Socket::getLocationPort(){
    string portStr = to_string(ntohs(server_address.sin_port));
    strcpy(port, portStr.c_str());
    
    return port;
}

void Socket::printLocation(string locationName){
    
    cout << locationName << "_ADDRESS " << getLocationAddress() << endl;
    cout << locationName << "_PORT " << getLocationPort() << endl;

}

//listen for a message and return the address and port of the sender
char* Socket::readMessage(){

    buffer = new char[MESSAGE_BUFFER_SIZE];

    //read message
    bzero(buffer, MESSAGE_BUFFER_SIZE);
    n = read(newSocketDescriptor, buffer, MESSAGE_BUFFER_SIZE);
    if (n < 0) {
        exit(ERROR_SOCKET_READ_FAILED);
    }
    
    return buffer;
}

//send a stream of bytes to specified location
void Socket::writeMessage(void* message, int length){

    //send message to location
    n = write(newSocketDescriptor,message,length);
    if (n < 0){
        exit(ERROR_SOCKET_WRITE_FAILED);
    }
    
}

//close socket
void Socket::closeSocket(){
    close(newSocketDescriptor);
}
