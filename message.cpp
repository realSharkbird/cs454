#include "message.h"
#include <string>

using namespace std;

//class for messaging protocols

Message::Message(int length, void** content){
    this->length = length;
    this->content = content;
    
}