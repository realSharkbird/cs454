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
#include "Socket.h"

using namespace std;

Socket::Socket(){
    //setup sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(DEFAULT_PORT);
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