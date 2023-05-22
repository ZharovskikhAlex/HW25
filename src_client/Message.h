#pragma once
#include <string>

struct Message
{
    const std::string  author_;
    const std::string _message;
    const std::string c_id;
    Message() = delete;
    Message(std::string &, std::string &&, std::string &);
    Message(std::string &, std::string &, std::string &);
    ~Message() = default;
};