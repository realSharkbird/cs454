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

#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max

using namespace std;

class procedure {
    char * name;
    int * argtypes;
    void ** args;
};

class location {
    string ip;
    int port;
};

//database for procedures (procedure signature, location)
std::map<procedure, location> database;

//This binder is the first thing to run
int main(){
        
    //create socket
    Socket * s = new Socket();
    s->printLocation();
    
    /*
    //listen for requests
    while(true){
    
        //wait for requests
        clientLen = sizeof(client_address);
        newsocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &client_address, &clientLen);
        if (newsocketDescriptor < 0)
            cout << "error on accept" << endl;
        
        //determine whether client or server request
        
        //read server request
        bzero(buffer,256);
        n = read(newsocketDescriptor,buffer,255);
        if (n < 0) cout << "error reading from socket" << endl;
        
        //print string received from server
        cout << buffer;
        
        //send binder response with newly converted string
        n = write(newsocketDescriptor,buffer,strlen(buffer));
        if (n < 0) cout << "error writing to socket" << endl;
        
    }*/
    
    return 0;
}