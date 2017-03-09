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
    s->printLocation("BINDER");
    
    //listen for requests
    while(true){
    
        //string message = s->readMessage();
        
        //determine if message from client or server
        
        //server registration request
        //cout << "registered message: " << message << endl;
    }
    
    return 0;
}