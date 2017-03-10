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
    
    //create socket
    Socket * s = new Socket();
    s->printLocation("BINDER");
    
    //listen for requests
    while(true){
    
        Message* message = (Message*)(s->readMessage());
        
        //determine if message from client or server
        cout << "(debug) binder received message: " << message << endl;

        //server registration request
        if(message->type == TYPE_SERVER_BINDER_MESSAGE && message->content_type == CONTENT_TYPE_REGISTER){
            
            Location* location = new Location();
            
            cout << "0" << endl;
            
            location->ip = *((char *)(message->content[0]));
            
            cout << "1" << endl;
            
            location->port = *((char *)(message->content[1]));
            
            Procedure* procedure = new Procedure();
            
            cout << "2" << endl;
            
            procedure->name = (char *)(message->content[2]);
            
            cout << "3" << endl;
            
            procedure->argTypes = (int *)message->content[3];
            
            database->insert( pair<Procedure*,Location*>(procedure, location) );
            
        }
        
    }
    
    return 0;
}