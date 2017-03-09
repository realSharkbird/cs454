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
    Socket(INADDR_ANY, DEFAULT_PORT);
}

Socket::Socket(int address, int port){
    //setup sockets and stuff

    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    if (::bind(socketDescriptor, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
        cout << "error on binding" << endl;
    listen(socketDescriptor,MULTIPLEX);
    clientLen = sizeof(server_address);
}

Socket::Socket(string address, string port){
    //setup sockets and stuff
    int portNum = atoi(port.c_str());
    struct hostent *server = gethostbyname(address.c_str());

    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(portNum);
    if (::bind(socketDescriptor, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
        cout << "error on binding" << endl;
    listen(socketDescriptor,MULTIPLEX);
    clientLen = sizeof(server_address);
}

void Socket::printLocation(){
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
}

string Socket::readMessage(){
    //wait for requests
    clientLen = sizeof(client_address);
    newsocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &client_address, &clientLen);
    if (newsocketDescriptor < 0)
        cout << "error on accept" << endl;
    
    //read server request
    bzero(buffer,256);
    n = read(newsocketDescriptor,buffer,255);
    if (n < 0) cout << "error reading from socket" << endl;
    
    //print string received from server
    cout << buffer;
    
    return buffer;
}

void Socket::writeMessage(string message){
    
     //send requests from queue to server
     n = write(socketDescriptor,message.c_str(),strlen(message.c_str()));
     
     if (n < 0)
     cout << "error writing to socket" << endl;
    
}