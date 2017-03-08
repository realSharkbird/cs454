#include "rpc.h"
#include <thread>
#include <sys/socket.h>
#include <map>
#include <iostream>
#include <cassert>

const static int SUCCESS = 0;
const static int ERROR = -1;
const static int WARNING = 1;

//database for procedures (procedure signature, location)
std::map<std::string, int> m;

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
    
    //call the binder, inform it that a server procedure with the corresponding arguments are available at this server
    
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

