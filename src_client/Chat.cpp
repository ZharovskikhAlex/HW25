#include "Chat.h"

Chat::Chat(std::string & user1, std::string & user2) : user1(user1), user2(user2){}

void Chat::addMessage(Message & m)
{
    messages.push_back(m);
}