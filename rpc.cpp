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
#include "utils.h"
#include <map>
#include <cassert>
#include <cstring>
#include "binder.h"
#include "server_function_skels.h"

const static int SUCCESS = 0;
const static int ERROR = -1;
const static int WARNING = 1;

using namespace std;

Socket * binderSocket;
Socket * clientSocket;
Socket * serverSocket;

std::map<skeleton, Location*>* localDatabase;

//methods for dealing with args
int getArgIO(int argType){
    
    int io = (argType >> ARG_OUTPUT) & 3;
    
    switch(io){
        case 0:
            //none
            DEBUG_MSG("arg is none");
            
            break;
        case 1:
            //input
            DEBUG_MSG("arg is input");
            
            break;
        case 2:
            //output
            DEBUG_MSG("arg is output");
            
            break;
        case 3:
            //both
            DEBUG_MSG("arg is both");
            
            break;
        default:
            DEBUG_MSG("error in detecting io: " << io);
            
            break;
    }
    
    return io;

}

int getArgType(int argType){
    
    int type = (argType >> 16) & 255;
    
    return type;

}

int getArgLength(int argType){
    
    int length = 65535 & argType;
    
    if(length == 0){
        DEBUG_MSG("arg is scalar");
        
    }else if(length > 0){
        DEBUG_MSG("arg is array: " << length);
        
    }else{
        DEBUG_MSG("error, arg length is negative: " << length);
        
    }
    
    return length;

}

int getNumArgs(int* argTypes){
    int length = 0;
    while(argTypes[length] != 0){
        length++;
    }
    return length;
}

int getArgSize(int argType){
    int length = getArgLength(argType);
    if(length == 0){
        length = 1;
    }
    int type = getArgType(argType);
    //send argument(s) to the server
    switch(type){
        case ARG_CHAR:
            return sizeof(char) * length;
            
        case ARG_SHORT:
            return sizeof(short) * length;
            
        case ARG_INT:
            return sizeof(int) * length;
            
        case ARG_LONG:
            return sizeof(long) * length;
            
        case ARG_DOUBLE:
            return sizeof(double) * length;
            
        case ARG_FLOAT:
            return sizeof(float) * length;
            
        default:
            DEBUG_MSG("error detecting argType");
            return 0;
            
    }
}

//write args to remote socket
void writeArgs(Socket* serverSocket, int* argTypes, void** args){
    int length = getNumArgs(argTypes);

    serverSocket->writeMessage(argTypes, sizeof(int) * length);
    serverSocket->readMessage();
    
    DEBUG_MSG("arg length: " << length);
    
    //send args
    for(int i = 0; i < length; i++){
        
        int argType = argTypes[i];
        
        DEBUG_MSG("sending arg " << i);
        
        //determine if input, output or both (I guess this means whether it's a pointer or not)
        int io = getArgIO(argTypes[i]);
        serverSocket->writeMessage(&io, sizeof(int));
        serverSocket->readMessage();
        
        //determine if argument is scalar or array
        int argLength = getArgLength(argTypes[i]);
        
        serverSocket->writeMessage(&argLength, sizeof(int));
        serverSocket->readMessage();
        
        DEBUG_MSG("sent arg length");
        
        //determine typename
        int type = getArgType(argTypes[i]);
        
        DEBUG_MSG("sending arg type");
        
        serverSocket->writeMessage(&type, sizeof(int));
        serverSocket->readMessage();
        
        if(argLength == 0){
            
            DEBUG_MSG("arg length is 0");
            
            serverSocket->writeMessage(args[i], getArgSize(argType));
            
            DEBUG_MSG("arg sent");
            
            serverSocket->readMessage();
            
            DEBUG_MSG("server ack received");
            
        }else{
            
            DEBUG_MSG("arg is an array");
            
            for(int j = 0; j < argLength; j++){
                
                //gotta do this cuz no reflection in c++
                switch(type){
                    case ARG_CHAR:
                        DEBUG_MSG("arg is char");
                        serverSocket->writeMessage(&(((char*)args[i])[j]), sizeof(char));
                        break;
                    case ARG_SHORT:
                        DEBUG_MSG("arg is short");
                        serverSocket->writeMessage(&(((short*)args[i])[j]), sizeof(short));
                        break;
                    case ARG_INT:
                        DEBUG_MSG("arg is int");
                        serverSocket->writeMessage(&(((int*)args[i])[j]), sizeof(int));
                        break;
                    case ARG_LONG:
                        DEBUG_MSG("arg is long");
                        serverSocket->writeMessage(&(((long*)args[i])[j]), sizeof(long));
                        break;
                    case ARG_DOUBLE:
                        DEBUG_MSG("arg is double");
                        serverSocket->writeMessage(&(((double*)args[i])[j]), sizeof(double));
                        break;
                    case ARG_FLOAT:
                        DEBUG_MSG("arg is float");
                        serverSocket->writeMessage(&(((float*)args[i])[j]), sizeof(float));
                        break;
                    default:
                        DEBUG_MSG("error detecting argType: " << type);
                        break;
                        
                }
                
                serverSocket->readMessage();
                
            }
            DEBUG_MSG("arg sent");
            
        }
        
        serverSocket->writeMessage((void*)"Ack", 4);
        
        DEBUG_MSG("done sending arg " << i);
        
    }
}

//read args from remote rocket
void readArgs(Socket* clientSocket, int* argTypes, void** args){
    argTypes = (int*)clientSocket->readMessage();
    clientSocket->writeMessage((void*)"Ack", 4);
    
    int length = getNumArgs(argTypes);
    
    //proceed to read args
    args = (void **)malloc(length * sizeof(void *));;
    
    for(int i = 0; i < length; i++){
        
        //read io
        int io = *(clientSocket->readMessage());
        clientSocket->writeMessage((void*)"Ack", 4);
        DEBUG_MSG("received io: " << io);
        
        //read array or scalar
        int argLength = *(clientSocket->readMessage());
        clientSocket->writeMessage((void*)"Ack", 4);
        DEBUG_MSG("received array length: " << argLength);
        
        //read argType
        int argType = *(clientSocket->readMessage());
        clientSocket->writeMessage((void*)"Ack", 4);
        DEBUG_MSG("received arg type: " << argType);
        
        //read args
        if(argLength == 0){
            args[i] = clientSocket->readMessage();
            clientSocket->writeMessage((void*)"Ack", 4);
            
        }else{
            void** array = (void **)malloc(argLength * sizeof(void *));;
            for(int k = 0; k < argLength; k++){
                array[k] = clientSocket->readMessage();
                clientSocket->writeMessage((void*)"Ack", 4);
                
            }
            args[i] = array;
        }
        
    }
    
    //read ack from client
    clientSocket->readMessage();
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
    
    //get number of args
    int length = getNumArgs(argTypes);

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
                
                void** args;
                int* argTypes;
                readArgs(clientSocket, argTypes, args);
                
                //forward requests to skeletons
                int result = 0;
                if(name == "f0"){
                    DEBUG_MSG("forwarding request to f0");
                    result = f0_Skel(argTypes, args);
                }else if(name == "f1"){
                    DEBUG_MSG("forwarding request to f1");
                    result = f1_Skel(argTypes, args);
                }else if(name == "f2"){
                    DEBUG_MSG("forwarding request to f2");
                    result = f2_Skel(argTypes, args);
                }else if(name == "f3"){
                    DEBUG_MSG("forwarding request to f3");
                    result = f3_Skel(argTypes, args);
                }else if(name == "f4"){
                    DEBUG_MSG("forwarding request to f4");
                    result = f4_Skel(argTypes, args);
                }
                
                DEBUG_MSG("result of procedure: " << result);
                
                //Send back procedure result (eg. success fail)
                
                DEBUG_MSG("sending back result args " << result);

                //send back results
                //Send back modified arg values
                
            }
        }
    }
    
    return SUCCESS;
};

//called by client
int rpcCall(char* name, int* argTypes, void** args){
    
    int length = getNumArgs(argTypes);
    
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
    binderSocket->writeMessage(argTypes, sizeof(int) * length);
    
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
    
    serverSocket->readMessage();
    serverSocket->writeMessage(CONTENT_TYPE_EXECUTE, strlen(CONTENT_TYPE_EXECUTE));
    serverSocket->readMessage();
    serverSocket->writeMessage(name, strlen(name));
    serverSocket->readMessage();
    
    //send args to server to execute
    DEBUG_MSG("Sending args for remote procedure: " << name);
    writeArgs(serverSocket, argTypes, args);
    
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

