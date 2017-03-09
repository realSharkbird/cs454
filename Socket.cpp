/*
 * Custom socket class to make code look cleaner and less repetitive.
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

using namespace std;

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

void Socket::printLocation(string endpointName){
    
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
        
        cout << endpointName << "_ADDRESS " << hostname << endl;
        cout << endpointName << "_PORT " << ntohs(client_address.sin_port) << endl;
    }
}

string Socket::readMessage(){
    
    //wait for requests
    serverLen = sizeof(server_address);
    newsocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &server_address, &serverLen);
    if (newsocketDescriptor < 0)
        cout << "error on accept" << endl;
    
    //read server request
    bzero(buffer,256);
    n = read(newsocketDescriptor,buffer,255);
    if (n < 0) cout << "error reading from socket" << endl;
    
    return buffer;
}

void Socket::writeMessage(string message, string address, string port){
    
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
        
    //send requests from queue to server
    n = write(socketDescriptor,message.c_str(),strlen(message.c_str()));
    if (n < 0)
        cout << "error writing to socket" << endl;
    
}