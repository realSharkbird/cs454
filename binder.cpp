#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <map>
#include <iostream>
#include <cassert>
#include "Socket.h"
#include "message.h"
#include "binder.h"

using namespace std;

//database for procedures (procedure signature, location)
std::map<Procedure, Location>* database;

//we need this tedious code to make the database work
bool Procedure::operator<(const Procedure right) const{
    
    cout << "comparing " << name << " with " << right.name << endl;
    cout << "and " << argTypes << " with " << right.argTypes << endl;

    if(strcmp(name, right.name) == 0){
        
        bool result = (strcmp((char*)argTypes,(char*)right.argTypes) < 0);
        cout << "result in similar names: " << result << endl;
        
        cout << "true is: " << true << endl;
        
        return result;
        
    }
    
    bool result = (strcmp(name, right.name) < 0);
    cout << "result in different names: " << result << endl;

    return result;

}

//This binder is the first thing to run
int main(){
    
    //variables
    database = new std::map<Procedure, Location>();
    
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
            
            Location location;
            
            location.ip = s->readMessage();
            s->writeMessage((void*)"Ack", 4);
            
            location.port = s->readMessage();
            s->writeMessage((void*)"Ack", 4);

            Procedure procedure;
            
            procedure.name = s->readMessage();
            s->writeMessage((void*)"Ack", 4);
            
            procedure.argTypes = (int *)s->readMessage();
            s->writeMessage((void*)"Ack", 4);

            database->insert( std::pair<Procedure,Location>(procedure, location) );
            
            string str = procedure.name;
            cout << "registered procedure " << str << " from server" << endl;
            cout << "with ip: " << location.ip << " and port: "<< location.port << endl;
            
        }else if(strcmp(type, TYPE_CLIENT_BINDER_MESSAGE) == 0){
            
            s->writeMessage((void*)"Ack", 4);
            type = s->readMessage();

            //location request from client
            if(strcmp(type, CONTENT_TYPE_LOC_REQUEST) == 0){
                
                cout << "(debug) binder received content type: " << type << endl;
                
                s->writeMessage((void*)"Ack", 4);
                
                char* name = s->readMessage();
                s->writeMessage((void*)"Ack", 4);

                int* argTypes = (int*)s->readMessage();
                
                cout << "received procedure details" << endl;
                cout << "name: " << name << endl;
                
                int i = 0;
                while(argTypes[i] != 0){
                    cout << "args - " << i << ": " << argTypes[i] << endl;
                    i++;
                }
                
                //create key to find location from database
                Procedure procedure;
                procedure.name = name;
                procedure.argTypes = argTypes;
                
                cout << "getting location from database" << endl;
                
                //get location from database
                Location serverLoc = database->find(procedure)->second;
                
                char* ip = new char[serverLoc.ip.length()];
                char* port = new char[serverLoc.port.length()];
                strcpy(ip, serverLoc.ip.c_str());
                strcpy(port, serverLoc.port.c_str());

                cout << "sending ip: " << ip << " and port: " << port << endl;
                
                //send location info to client
                s->writeMessage(ip, strlen(ip));
                s->readMessage();
                s->writeMessage(port, strlen(port));
                
                cout << "Location info sent to client" << endl;
                
            }
            
        //termination request
        }else if(strcmp(type, TYPE_TERMINATE_MESSAGE) == 0){
            
            s->closeSocket();
            break;
        }
        
    }
    
    return 0;
}