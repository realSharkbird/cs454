#include "rpc.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <queue>
#include <thread>
#include "Socket.h"

const static int SUCCESS = 0;
const static int ERROR = -1;
const static int WARNING = 1;

using namespace std;

//thread for connection to clients
void readClient(){
    
}

//thread for connection to the binder
void readBinder(){
    
}

//first called by server
int rpcInit(){
    
    //create a connection socket for accepting connections from clients
    std::thread t1(readClient);
    
    //Open a connection to the binder
    std::thread t2(readBinder);

    t1.join();
    t2.join();
    
    return SUCCESS;
};

//2nd function called by server
int rpcRegister(char* name, int* argTypes, skeleton f){
    
    cout << "sending register message" << endl;
    
    //get server address and port
    string SERVER_ADDRESS = getenv("SERVER_ADDRESS");
    string SERVER_PORT = getenv("SERVER_PORT");
    
    //create socket
    Socket * s = new Socket();

    //call the binder, inform it that a server procedure with the corresponding arguments are available at this server
    s->writeMessage("father", SERVER_ADDRESS, SERVER_PORT);
    
    //Make an entry in a local database, associating the server skeleton with the name and list of argument types.
    
    return SUCCESS;
};

//3rd function called by server
int rpcExecute(){
    
    //wait and receive requests
    
    //forward requests to skeletons
    
    //send back results
    //Send back procedure result (eg. success fail)
    //Send back modified arg values
    
    return SUCCESS;
};

//called by client
int rpcCall(char* name, int* argTypes, void** args){
    //send a location request message to the binder to locate the server for the procedure
    
    //Send an execute-request message to the server
    
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
    
};

