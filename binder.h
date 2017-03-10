class Procedure {
    
public:
    char * name;
    int * argTypes;
    void ** args;
};

class Location {
    
public:
    std::string ip;
    std::string port;
};