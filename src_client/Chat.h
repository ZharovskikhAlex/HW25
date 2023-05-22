#pragma once
#include <list>
#include "Message.h"

struct Chat
{
    std::string user1;
    std::string user2;
    std::string c_id;
    std::list<Message> messages;
    Chat(std::string &, std::string &);
    ~Chat() = default;
    void addMessage(Message &);
};