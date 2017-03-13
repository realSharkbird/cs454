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
#include <queue>
#include "Socket.h"
#include "utils.h"
#include "binder.h"

using namespace std;

//database for procedures (procedure signature, queue of locations)
std::map<Procedure, std::queue<Location>>* database;

//list of connected servers
std::queue<Location> servers;

//we need this tedious code to make the database work
bool Procedure::operator<(const Procedure right) const{
    
    if(strcmp(name, right.name) == 0){
        
        bool result = (strcmp((char*)argTypes,(char*)right.argTypes) < 0);
        
        return result;
        
    }
    
    bool result = (strcmp(name, right.name) < 0);

    return result;

}

//This binder is the first thing to run
int main() {

    //variables
    database = new std::map<Procedure, std::queue<Location>>();

    //create listening socket
    Socket *s = new Socket();
    s->printLocation("BINDER");

    //listen for requests
    while (true) {

        s->listenForConnection();
        char *type = s->readMessage();

        DEBUG_MSG("binder received type: " << type);

        //server registration request
        if(strcmp(type, TYPE_SERVER_BINDER_MESSAGE) == 0){
            
            s->writeMessage((void*)"Ack", 4);
            type = s->readMessage();
            
            DEBUG_MSG("binder received content type: " << type);
            
            if(strcmp(type, CONTENT_TYPE_REGISTER) == 0){
                s->writeMessage((void*)"Ack", 4);
                
                Location location;
                
                location.ip = s->readMessage();
                s->writeMessage((void*)"Ack", 4);
                
                location.port = s->readMessage();
                s->writeMessage((void*)"Ack", 4);

                DEBUG_MSG("got address and port");
                
                Procedure procedure;
                
                procedure.name = s->readMessage();
                s->writeMessage((void*)"Ack", 4);
                
                procedure.argTypes = (int *)s->readMessage();
                s->writeMessage((void*)"Ack", 4);

            procedure.argTypes = (int *) s->readMessage();
            s->writeMessage((void *) "Ack", 4);

                if (database->find(procedure) == database->end()) {
                    queue<Location> locationQueue;
                    locationQueue.push(location);
                    database->insert( std::pair<Procedure, queue<Location>>(procedure, locationQueue) );
                } else {
                    database->at(procedure).push(location);
                }

                string str = procedure.name;
                DEBUG_MSG("registered procedure " << str << " from server");
                DEBUG_MSG("with ip: " << location.ip << " and port: "<< location.port);
            }else if(strcmp(type, CONTENT_TYPE_INIT) == 0){
                s->writeMessage((void*)"Ack", 4);
                
                Location location;
                
                location.ip = s->readMessage();
                s->writeMessage((void*)"Ack", 4);
                
                location.port = s->readMessage();
                s->writeMessage((void*)"Ack", 4);
                
                servers.push(location);
            }
            
        }else if(strcmp(type, TYPE_CLIENT_BINDER_MESSAGE) == 0){
            
            s->writeMessage((void*)"Ack", 4);
            type = s->readMessage();
            
            DEBUG_MSG("binder received content type: " << type);

            //location request from client
            if (strcmp(type, CONTENT_TYPE_LOC_REQUEST) == 0) {

                DEBUG_MSG("(debug) binder received content type: " << type);

                s->writeMessage((void *) "Ack", 4);

                char *name = s->readMessage();
                s->writeMessage((void *) "Ack", 4);

                int *argTypes = (int *) s->readMessage();

                int i = 0;
                while (argTypes[i] != 0) {
                    i++;
                }

                //create key to find location from database
                Procedure procedure;
                procedure.name = name;
                procedure.argTypes = argTypes;

                DEBUG_MSG("getting location from database");

                //get location from database
                //locations stored in a queue for round-robin scheduling, each access moves server to back
                queue<Location> serverLocQueue = database->find(procedure)->second;
                Location serverLoc = serverLocQueue.front();
                serverLocQueue.push(serverLoc);
                serverLocQueue.pop();
                database->at(procedure) = serverLocQueue;

                char *ip = new char[serverLoc.ip.length()];
                char *port = new char[serverLoc.port.length()];
                strcpy(ip, serverLoc.ip.c_str());
                strcpy(port, serverLoc.port.c_str());

                //send location info to client
                s->writeMessage(ip, strlen(ip));
                s->readMessage();
                s->writeMessage(port, strlen(port));

                DEBUG_MSG("Location info sent to client");
                
            }else if(strcmp(type, CONTENT_TYPE_TERMINATE) == 0){
                
                int numServers = servers.size();
                for(int i = 0; i < numServers; i++){
                    Location server = servers.front();
                    Socket* serverSocket = new Socket(server.ip, server.port);
                    serverSocket->writeMessage(TYPE_SERVER_BINDER_MESSAGE, strlen(TYPE_SERVER_BINDER_MESSAGE));
                    serverSocket->readMessage();
                    serverSocket->writeMessage(TYPE_TERMINATE_MESSAGE, strlen(TYPE_TERMINATE_MESSAGE));
                    serverSocket->readMessage();
                    servers.pop();

                }
                
                s->closeSocket();
                return 0;
            }
            
        }
    }
    return 0;
}