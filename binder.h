#define DEFAULT_PORT 0 // port with #0 will be assigned to next available port.
#define MULTIPLEX 5 //5 connections at max

class Procedure {
    
public:
    bool operator<(const Procedure rhs) const;
    char * name;
    int * argTypes;
};

class Location {
    
public:
    std::string ip;
    std::string port;
};