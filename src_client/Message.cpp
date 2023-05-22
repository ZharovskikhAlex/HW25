#include "Message.h"

Message::Message(std::string & author, std::string && message, std::string & c_id) : author_(author), _message(message), c_id(c_id){}

Message::Message(std::string & author, std::string & message, std::string & c_id) : author_(author), _message(message), c_id(c_id){}