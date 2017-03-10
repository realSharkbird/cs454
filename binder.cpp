#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>
#include <iostream>
#include <cassert>
#include "Socket.h"
#include "message.h"
#include "binder.h"

#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max

using namespace std;

//database for procedures (procedure signature, location)
std::map<Procedure*, Location*>* database;

//This binder is the first thing to run
int main(){
    
    //variables
    database = new std::map<Procedure*, Location*>();
    
    //create listening socket
    Socket * s = new Socket();
    s->printLocation("BINDER");

    //listen for requests
    while(true){
        
        s->listenForConnection();
        char* type = s->readMessage();

        cout << "(debug) binder received type: " << type << endl;

        //server registration request
        if(strcmp(type, TYPE_SERVER_BINDER_MESSAGE) == 0){
            
            s->writeMessage((void*)"Ack", 4);
            
            Location* location = new Location();
            
            location->ip = (char*)s->readMessage();
            s->writeMessage((void*)"Ack", 4);
            
            location->port = (char*)s->readMessage();
            s->writeMessage((void*)"Ack", 4);

            Procedure* procedure = new Procedure();
            
            procedure->name = (char*)s->readMessage();
            s->writeMessage((void*)"Ack", 4);
            
            procedure->argTypes = (int *)s->readMessage();
            s->writeMessage((void*)"Ack", 4);

            database->insert( pair<Procedure*,Location*>(procedure, location) );
            
            string str = procedure->name;
            cout << "registered procedure " << str << " from server" << endl;
            cout << "with ip: " << location->ip << " and port: "<< location->port << endl;

        }
        
    }
    
    return 0;
}