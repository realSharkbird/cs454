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

//create the socket. Assignment next available port #.
Socket::Socket(){
    //setup sockets and stuff
    bzero((char *) &client_address, sizeof(client_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(DEFAULT_PORT);
}

void Socket::printLocation(string locationName){
    
    if (::bind(socketDescriptor, (struct sockaddr *) &client_address,
             sizeof(client_address)) < 0)
        cout << "error on binding" << endl;
    listen(socketDescriptor,MULTIPLEX);
    clientLen = sizeof(client_address);
    
    //print out server and port numbers
    if (getsockname(socketDescriptor, (struct sockaddr *)&client_address, &clientLen) < 0){
        cout << "failed to get hostname with errno: " << endl;
        exit(1);
    }else{
        
        char hostname[1024];
        gethostname(hostname, 1024);
        
        cout << locationName << "_ADDRESS " << hostname << endl;
        cout << locationName << "_PORT " << ntohs(client_address.sin_port) << endl;
    }
}

//listen for a message
void* Socket::readMessage(){
    
    //wait for requests
    serverLen = sizeof(server_address);
    newsocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &server_address, &serverLen);
    if (newsocketDescriptor < 0)
        cout << "error on accept" << endl;
    
    //read message
    bzero(buffer,65536);
    n = read(newsocketDescriptor,buffer,65535);
    if (n < 0) cout << "error reading from socket" << endl;
    
    return buffer;
}

//send a message to specified location
void Socket::writeMessage(void* message, string address, string port){
    
    //variables
    int socketDescriptor, portNum, n;
    struct sockaddr_in server_address;
    struct hostent *server;
    
    //initialize sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    portNum = atoi(port.c_str());
    if (socketDescriptor < 0)
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
    if (connect(socketDescriptor,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
        cout << "error connecting" << endl;
        
    //send message to location
    n = write(socketDescriptor,message,sizeof(message));
    if (n < 0)
        cout << "error writing to socket" << endl;
    
}