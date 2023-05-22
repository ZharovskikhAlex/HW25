#pragma once
#include "Chat.h"
#include <string>
#include <set>
#include <map>

struct User
{
    std::string login;
    std::set<std::string> u_list;
    std::map<std::string, Chat> u_chats;
    User(std::string &);
    ~User() = default;
};
