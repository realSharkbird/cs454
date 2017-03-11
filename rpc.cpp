#include "rpc.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <netdb.h>
#include <queue>
#include <thread>
#include "Socket.h"
#include "message.h"
#include <map>
#include <cassert>
#include <cstring>
#include "binder.h"

const static int SUCCESS = 0;
const static int ERROR = -1;
const static int WARNING = 1;

using namespace std;

Socket * binderSocket;
Socket * clientSocket;
Socket * serverSocket;

std::map<skeleton, Location*>* localDatabase;

int getArgLength(int* argTypes){
    int length = 0;
    while(argTypes[length] != 0){
        length++;
    }
    return length;
}

//thread for connection to clients
void readClient(){

}

//thread for connection to the binder
void readBinder(){

}

//first called by server
int rpcInit(){
    
    //create a socket for accepting connections from clients
    clientSocket = new Socket();
    clientSocket->printLocation("SERVER");
    
    //no need to open connection to the binder from the start
    /*char* BINDER_ADDRESS = getenv("BINDER_ADDRESS");
    *char* BINDER_PORT = getenv("BINDER_PORT");
    *binderSocket = new Socket(BINDER_ADDRESS, BINDER_PORT);
    */
    
    //init local database
    localDatabase = new std::map<skeleton, Location*>();
    
    //create threads to enable binder and client connections to run in parallel
    std::thread t1(readClient);
    std::thread t2(readBinder);
    t1.join();
    t2.join();
    
    return SUCCESS;
};

//2nd function called by server
int rpcRegister(char* name, int* argTypes, skeleton f){
    
    DEBUG_MSG("sending register message");
    
    //get length of argTypes
    int length = getArgLength(argTypes);

    //get server address and port
    char* SERVER_ADDRESS = clientSocket->getLocationAddress();
    char* SERVER_PORT = clientSocket->getLocationPort();
    
    char* BINDER_ADDRESS = getenv("BINDER_ADDRESS");
    char* BINDER_PORT = getenv("BINDER_PORT");
    
    //create socket
    binderSocket = new Socket(BINDER_ADDRESS, BINDER_PORT);
    Socket * s = binderSocket;
    
    //call the binder, inform it that a server procedure with the corresponding arguments are available at this server
    s->writeMessage(TYPE_SERVER_BINDER_MESSAGE, strlen(TYPE_SERVER_BINDER_MESSAGE));

    //wait for binder for acknowledgement
    s->readMessage();

    //send message content
    s->writeMessage(SERVER_ADDRESS, strlen(SERVER_ADDRESS));
    s->readMessage();
    s->writeMessage(SERVER_PORT, strlen(SERVER_PORT));
    s->readMessage();
    s->writeMessage(name, strlen(name));
    s->readMessage();
    s->writeMessage(argTypes, length * sizeof(int));
    s->readMessage();

    s->closeSocket();
    
    //Make an entry in a local database, associating the server skeleton with the name and list of argument types.
    Location* location = new Location();
    location->ip = SERVER_ADDRESS;
    location->port = SERVER_PORT;
    localDatabase->insert( pair<skeleton,Location*>(f, location) );
    
    DEBUG_MSG("registered skeleton " << name << " into local db");
    
    return SUCCESS;
};

//3rd function called by server
int rpcExecute(){
    
    DEBUG_MSG("rpc execute called");
    
    while(true){
        
        clientSocket->listenForConnection();
        
        //wait and receive requests
        char* type = clientSocket->readMessage();

        DEBUG_MSG("received rpc call from a client");
        
        //check request
        if(strcmp(type, TYPE_CLIENT_SERVER_MESSAGE) == 0){
            clientSocket->writeMessage((void*)"Ack", 4);
            type = clientSocket->readMessage();
            if(strcmp(type, CONTENT_TYPE_EXECUTE) == 0){
                clientSocket->writeMessage((void*)"Ack", 4);

                char* name = clientSocket->readMessage();
                clientSocket->writeMessage((void*)"Ack", 4);
                int* argTypes = (int*)clientSocket->readMessage();
                clientSocket->writeMessage((void*)"Ack", 4);
                
                int length = getArgLength(argTypes);
                
                //read args
                void** args;
                
                //forward requests to skeletons
                //skeleton f = localDatabase.get();
                
                //send back results
                //Send back procedure result (eg. success fail)
                //Send back modified arg values
                
            }
        }
    }
    
    return SUCCESS;
};

//called by client
int rpcCall(char* name, int* argTypes, void** args){
    
    DEBUG_MSG("rpcCall called");
    
    char* BINDER_ADDRESS = getenv("BINDER_ADDRESS");
    char* BINDER_PORT = getenv("BINDER_PORT");
    
    DEBUG_MSG("sending location request to " << BINDER_ADDRESS);
    
    //send a location request message to the binder to locate the server for the procedure
    binderSocket = new Socket(BINDER_ADDRESS, BINDER_PORT);
    binderSocket->writeMessage(TYPE_CLIENT_BINDER_MESSAGE, strlen(TYPE_CLIENT_BINDER_MESSAGE));
    binderSocket->readMessage();
    binderSocket->writeMessage(CONTENT_TYPE_LOC_REQUEST, strlen(CONTENT_TYPE_LOC_REQUEST));
    binderSocket->readMessage();
    
    //send procedure
    binderSocket->writeMessage(name, strlen(name));
    binderSocket->readMessage();
    binderSocket->writeMessage(argTypes, sizeof(argTypes));
    
    DEBUG_MSG("retreiving server location");
    
    char* SERVER_ADDRESS = binderSocket->readMessage();
    binderSocket->writeMessage((void*)"Ack", 4);
    char* SERVER_PORT = binderSocket->readMessage();
    
    DEBUG_MSG("received server location from binder");
    DEBUG_MSG("server address: " << SERVER_ADDRESS);
    DEBUG_MSG("server port: " << SERVER_PORT);

    binderSocket->closeSocket();
    
    DEBUG_MSG("sending rpc call to server");
    
    //Send an execute-request message to the server
    serverSocket = new Socket(SERVER_ADDRESS, SERVER_PORT);
    
    DEBUG_MSG("server socket initiated");
    
    serverSocket->writeMessage(TYPE_CLIENT_SERVER_MESSAGE, strlen(TYPE_CLIENT_SERVER_MESSAGE));
    
    DEBUG_MSG("first msg sent");

    serverSocket->readMessage();
    serverSocket->writeMessage(CONTENT_TYPE_EXECUTE, strlen(CONTENT_TYPE_EXECUTE));
    serverSocket->readMessage();
    serverSocket->writeMessage(name, sizeof(name));
    serverSocket->readMessage();
    serverSocket->writeMessage(argTypes, sizeof(argTypes));
    serverSocket->readMessage();
    
    int length = getArgLength(argTypes);

    //send args
    
    //retreive results
    
};

//called by client
int rpcCacheCall(char* name, int* argTypes, void** args){
    
    //similar to rpcCall except some differences
    
};

//called by client to terminate system
int rpcTerminate(){

    //send request to the binder

    //binder in turn will inform servers to terminate
    
    //servers authenticate request by checking binder ip address
    
    //binder terminates after all servers have terminated
    return SUCCESS;
    
};

