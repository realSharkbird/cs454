/*
 * Custom socket class to make sockets easier to use.
 * NOTE: I used some of this code from my assignment #2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <iostream>
#include <cassert>
#include <netdb.h>
#include "Socket.h"
#include "message.h"

using namespace std;

//create a server socket
Socket::Socket(){
    //setup sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(DEFAULT_PORT);

}

//create a client socket
Socket::Socket(string address, string port) {

    //variables
    int portNum, n;
    struct hostent *server;

    //initialize sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    newsocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    portNum = atoi(port.c_str());
    if (newsocketDescriptor < 0)
        cout << "error opening socket" << endl;
    server = gethostbyname(address.c_str());
    if (server == NULL) {
        cout << "error, host doesnt exist" << endl;
        exit(0);
    }

    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(portNum);
    if (connect(newsocketDescriptor,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
        cout << "error connecting" << endl;
}

//listen for connection
void Socket::listenForConnection(){
    
    //wait for requests
    clientLen = sizeof(client_address);
    newsocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &client_address, &clientLen);
    if (newsocketDescriptor < 0)
        cout << "error on accept: " << newsocketDescriptor << endl;
    
    
}

char* Socket::getLocationAddress(){
    char* hostname = new char[1024];
    gethostname(hostname, 1024);
    
    return hostname;
}

char* Socket::getLocationPort(){
    string portStr = to_string(ntohs(server_address.sin_port));
    char* port = new char[portStr.length()];
    strcpy(port, portStr.c_str());
    
    return port;
}

void Socket::printLocation(string locationName){
    
    if (::bind(socketDescriptor, (struct sockaddr *) &server_address,
             sizeof(server_address)) < 0)
        cout << "error on binding" << endl;
    listen(socketDescriptor,MULTIPLEX);
    serverLen = sizeof(server_address);
    
    //print out server and port numbers
    if (getsockname(socketDescriptor, (struct sockaddr *)&server_address, &serverLen) < 0){
        cout << "failed to get hostname with errno: " << endl;
        exit(1);
    }else{
        
        cout << locationName << "_ADDRESS " << getLocationAddress() << endl;
        cout << locationName << "_PORT " << getLocationPort() << endl;
    }
}

//listen for a message and return the address and port of the sender
char* Socket::readMessage(){
    
    char* buffer = new char[16777216];
    
    //read message
    bzero(buffer,16777216);
    n = read(newsocketDescriptor,buffer,16777216);
    if (n < 0) cout << "error reading from socket" << endl;
    
    return buffer;
}

//send a stream of bytes to specified location
void Socket::writeMessage(void* message, int length){
    
    //send message to location
    n = write(newsocketDescriptor,message,length);
    if (n < 0){
        cout << "error writing to socket: " << n << endl;
        exit(1);
    }
    
}

//close socket
void Socket::closeSocket(){
    close(newsocketDescriptor);
}