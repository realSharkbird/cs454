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
#include "message.h"

#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max

using namespace std;

class Procedure {
    
public:
    char * name;
    int * argTypes;
    void ** args;
};

class Location {
    
public:
    string ip;
    string port;
};

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
        int type = *((int*)(s->readMessage()));

        cout << "(debug) binder received type: " << type << endl;

        //server registration request
        if(type == TYPE_SERVER_BINDER_MESSAGE){
            
            s->writeMessage("Ack");
            
            Location* location = new Location();
            
            cout << "0" << endl;
            
            location->ip = s->readMessage();
            s->writeMessage("Ack");

            cout << "1" << endl;
            
            location->port = s->readMessage();
            s->writeMessage("Ack");

            Procedure* procedure = new Procedure();
            
            cout << "2" << endl;
            
            procedure->name = s->readMessage();
            s->writeMessage("Ack");

            cout << "3" << endl;
            
            procedure->argTypes = (int *)s->readMessage();
            s->writeMessage("Ack");

            database->insert( pair<Procedure*,Location*>(procedure, location) );
            
        }
        
    }
    
    return 0;
}