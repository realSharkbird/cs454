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
    
    return io;

}

int getArgType(int argType){
    
    int type = (argType >> 16) & 255;
    
    return type;

}

int getArgLength(int argType){
    
    int length = 65535 & argType;
    
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
            break;
        case ARG_SHORT:
            return sizeof(short) * length;
            break;
        case ARG_INT:
            return sizeof(int) * length;
            break;
        case ARG_LONG:
            return sizeof(long) * length;
            break;
        case ARG_DOUBLE:
            return sizeof(double) * length;
            break;
        case ARG_FLOAT:
            return sizeof(float) * length;
            break;
        default:
            return 0;
            break;
    }
}

//write args to remote socket
void writeArgs(Socket* serverSocket, int* argTypes, void** args){
    int length = getNumArgs(argTypes);

    serverSocket->writeMessage(argTypes, sizeof(int) * (length + 1));
    serverSocket->readMessage();
    
    //send args
    for(int i = 0; i < length; i++){
        
        int argType = argTypes[i];
        
        //determine if input, output or both (I guess this means whether it's a pointer or not)
        int io = getArgIO(argType);

        //determine if argument is scalar or array
        int argLength = getArgLength(argType);

        //determine typename
        int type = getArgType(argType);
        
        //serverSocket->writeMessage(args[i], sizeof(char));
        
        if(argLength == 0){
            argLength = 1;
        }
        
        //gotta do this cuz no reflection in c++
        switch(type){
            case ARG_CHAR:
                serverSocket->writeMessage((char*)(args[i]), sizeof(char) * argLength);

                break;
            case ARG_SHORT:
                serverSocket->writeMessage((short*)(args[i]), sizeof(short) * argLength);

                break;
            case ARG_INT:
                serverSocket->writeMessage((int*)(args[i]), sizeof(int) * argLength);

                break;
            case ARG_LONG:
                serverSocket->writeMessage((long*)(args[i]), sizeof(long) * argLength);
                break;
            case ARG_DOUBLE:
                serverSocket->writeMessage((double*)(args[i]), sizeof(double) * argLength);

                break;
            case ARG_FLOAT:
                serverSocket->writeMessage((float*)(args[i]), sizeof(float) * argLength);

                break;
            default:
                break;
                
        }
        
        serverSocket->readMessage();

    }
    serverSocket->writeMessage((void*)"Ack", 4);

}

//read args from remote rocket
void readArgs(Socket* clientSocket, int** argTypes, void*** args){
    
    *argTypes = (int*)clientSocket->readMessage();
    
    clientSocket->writeMessage((void*)"Ack", 4);

    int length = getNumArgs(*argTypes);
    
    //proceed to read args
    *args = (void **)malloc(length * sizeof(void *));;
    
    for(int i = 0; i < length; i++){
        
        //read io
        int io = getArgIO((*argTypes)[i]);
        
        //read array or scalar
        int argLength = getArgLength((*argTypes)[i]);
        
        //read argType
        int argType = getArgType((*argTypes)[i]);
        
        //read args
        if(argLength == 0){
            
            argLength = 1;

        }
        
        //gotta do this cuz no reflection in c++
        switch(getArgType((*argTypes)[i])){
            case ARG_CHAR:
                (*args)[i] = (char *)malloc(argLength * sizeof(char));
                (*args)[i] = (char *)(clientSocket->readMessage());

                break;
            case ARG_SHORT:
                (*args)[i] = (short *)malloc(argLength * sizeof(short));
                (*args)[i] = (short *)(clientSocket->readMessage());

                break;
            case ARG_INT:
                (*args)[i] = (int *)malloc(argLength * sizeof(int));
                (*args)[i] = (int *)(clientSocket->readMessage());

                break;
            case ARG_LONG:
                (*args)[i] = (long *)malloc(argLength * sizeof(long));
                (*args)[i] = (long *)(clientSocket->readMessage());
                break;
            case ARG_DOUBLE:
                (*args)[i] = (double *)malloc(argLength * sizeof(double));
                (*args)[i] = (double *)(clientSocket->readMessage());

                break;
            case ARG_FLOAT:
                (*args)[i] = (float *)malloc(argLength * sizeof(float));
                (*args)[i] = (float *)(clientSocket->readMessage());

                break;
            default:
                break;
            
        }
        
        clientSocket->writeMessage((void*)"Ack", 4);
        
    }
    
    clientSocket->readMessage();

}

//first called by server
int rpcInit(){
    
    //create a socket for accepting connections from clients
    clientSocket = new Socket();
    clientSocket->printLocation("SERVER");
    
    //tell binder that server exists
    char* BINDER_ADDRESS = getenv("BINDER_ADDRESS");
    char* BINDER_PORT = getenv("BINDER_PORT");
    binderSocket = new Socket(BINDER_ADDRESS, BINDER_PORT);
    binderSocket->writeMessage(TYPE_SERVER_BINDER_MESSAGE, strlen(TYPE_SERVER_BINDER_MESSAGE));
    binderSocket->readMessage();
    binderSocket->writeMessage(CONTENT_TYPE_INIT, strlen(CONTENT_TYPE_INIT));
    binderSocket->readMessage();
    binderSocket->writeMessage(clientSocket->getLocationAddress(), strlen(clientSocket->getLocationAddress()));
    binderSocket->readMessage();
    binderSocket->writeMessage(clientSocket->getLocationPort(), strlen(clientSocket->getLocationPort()));
    binderSocket->readMessage();
    binderSocket->closeSocket();
    
    //init local database
    localDatabase = new std::map<skeleton, Location*>();
    
    return SUCCESS;
};

//2nd function called by server
int rpcRegister(char* name, int* argTypes, skeleton f){
    
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
    
    s->writeMessage(TYPE_SERVER_BINDER_MESSAGE, strlen(TYPE_SERVER_BINDER_MESSAGE));
    s->readMessage();
    
    //call the binder, inform it that a server procedure with the corresponding arguments are available at this server
    s->writeMessage(CONTENT_TYPE_REGISTER, strlen(CONTENT_TYPE_REGISTER));

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
    
    return SUCCESS;
};

//3rd function called by server
int rpcExecute(){
    
    while(true){
        
        clientSocket->listenForConnection();
        
        //wait and receive requests
        char* type = clientSocket->readMessage();
        
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
                readArgs(clientSocket, &argTypes, &args);

                //forward requests to skeletons
                int result = 0;
                
                int returnSize = 0;
                
                if(strcmp(name, "f0") == 0){
                    result = f0_Skel(argTypes, args);
                    
                    //return is a single arg of type int
                    returnSize = sizeof(int);
                    
                }else if(strcmp(name, "f1") == 0){
                    result = f1_Skel(argTypes, args);
                    
                    //return is a single arg of type long
                    returnSize = sizeof(long);
                    
                }else if(strcmp(name, "f2") == 0){
                    result = f2_Skel(argTypes, args);
                    
                    //return is a single arg of type string
                    returnSize = strlen((char*)(args[0]));
                    
                }else if(strcmp(name, "f3") == 0){
                    result = f3_Skel(argTypes, args);
                    
                    //return is a single arg of type long array
                    returnSize = sizeof(long) * getArgLength(argTypes[0]);
                    
                }else if(strcmp(name, "f4") == 0){
                    result = f4_Skel(argTypes, args);
                    
                    //not actually supposed to return a value

                }
                
                if (result == 0) {
                    //Send back procedure success
                    clientSocket->writeMessage(CONTENT_TYPE_EXECUTE_SUCCESS, strlen(CONTENT_TYPE_EXECUTE_SUCCESS));
                    clientSocket->readMessage();

                    clientSocket->writeMessage(args[0], returnSize);
                    clientSocket->readMessage();

                } else {
                    
                    //Send back procedure fail
                    clientSocket->writeMessage(CONTENT_TYPE_EXECUTE_FAILURE, strlen(CONTENT_TYPE_EXECUTE_FAILURE));
                    clientSocket->readMessage();
                    
                }

            }
        }else if(strcmp(type, TYPE_SERVER_BINDER_MESSAGE) == 0){
            
            clientSocket->writeMessage((void*)"Ack", 4);
            type = clientSocket->readMessage();
            
            if(strcmp(type, TYPE_TERMINATE_MESSAGE) == 0){
                
                clientSocket->writeMessage((void*)"Ack", 4);
                clientSocket->closeSocket();
                
                return SUCCESS;
                
            }
            
        }
    }
    
    return SUCCESS;
};

//called by client
int rpcCall(char* name, int* argTypes, void** args){
    
    int length = getNumArgs(argTypes);
    
    char* BINDER_ADDRESS = getenv("BINDER_ADDRESS");
    char* BINDER_PORT = getenv("BINDER_PORT");
    
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
    
    char* SERVER_ADDRESS = binderSocket->readMessage();
    binderSocket->writeMessage((void*)"Ack", 4);
    char* SERVER_PORT = binderSocket->readMessage();

    binderSocket->closeSocket();
    
    //Send an execute-request message to the server
    serverSocket = new Socket(SERVER_ADDRESS, SERVER_PORT);
    
    serverSocket->writeMessage(TYPE_CLIENT_SERVER_MESSAGE, strlen(TYPE_CLIENT_SERVER_MESSAGE));
    
    serverSocket->readMessage();
    serverSocket->writeMessage(CONTENT_TYPE_EXECUTE, strlen(CONTENT_TYPE_EXECUTE));
    serverSocket->readMessage();
    serverSocket->writeMessage(name, strlen(name));
    serverSocket->readMessage();
    
    //send args to server to execute
    writeArgs(serverSocket, argTypes, args);

    //retreive results
    char* response = serverSocket->readMessage();
    serverSocket->writeMessage((void*)"Ack", 4);

    if (strcmp(response, CONTENT_TYPE_EXECUTE_SUCCESS) == 0) {
        //retreive results
        args[0] = serverSocket->readMessage();
        serverSocket->closeSocket();
        return SUCCESS;

    } else {
        
        serverSocket->closeSocket();

        return ERROR;
    }


    return SUCCESS;

    
};

//called by client
int rpcCacheCall(char* name, int* argTypes, void** args){
    
    //similar to rpcCall except some differences
    
};

//called by client to terminate system
int rpcTerminate(){
    
    char* BINDER_ADDRESS = getenv("BINDER_ADDRESS");
    char* BINDER_PORT = getenv("BINDER_PORT");
    
    //send request to the binder
    binderSocket = new Socket(BINDER_ADDRESS, BINDER_PORT);
    
    //binder in turn will inform servers to terminate
    binderSocket->writeMessage(TYPE_CLIENT_BINDER_MESSAGE, strlen(TYPE_CLIENT_BINDER_MESSAGE));
    binderSocket->readMessage();
    binderSocket->writeMessage(CONTENT_TYPE_TERMINATE, strlen(CONTENT_TYPE_TERMINATE));
    
    binderSocket->closeSocket();
    
    //servers authenticate request by checking binder ip address
    
    //binder terminates after all servers have terminated
    return SUCCESS;
    
};

