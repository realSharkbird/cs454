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
    
    //call the binder, inform it that a server procedure with the corresponding arguments are available at this server
    
    //get server address and port
    string SERVER_ADDRESS = getenv("SERVER_ADDRESS");
    string SERVER_PORT = getenv("SERVER_PORT");
    
    //variables
    int socketDescriptor, portNum, n;
    struct sockaddr_in server_address;
    struct hostent *server;
    
    //initialize sockets and stuff
    bzero((char *) &server_address, sizeof(server_address));
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    portNum = atoi(SERVER_PORT.c_str());
    if (socketDescriptor < 0)
        cout << "error opening socket" << endl;
    server = gethostbyname(SERVER_ADDRESS.c_str());
    if (server == NULL) {
        cout << "error, host doesnt exist" << endl;
        exit(0);
    }
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(portNum);
    if (connect(socketDescriptor,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
        cout << "error connecting" << endl;
    
    string feridun = "mr goose";
    
    //send requests from queue to server
    n = write(socketDescriptor,feridun.c_str(),strlen(feridun.c_str()));
    
    if (n < 0)
        cout << "error writing to socket" << endl;
    
    char buffer[256];
    bzero(buffer,256);
    
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

