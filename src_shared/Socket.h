#pragma once

#include <iostream>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <string>

#define SIZE 1024 

class Socket 
{
private:
    std::string msg;
    int fd_socket;
    char buffer[SIZE];
    size_t ptr;
public:
    Socket() = delete;
    Socket(int);
    ~Socket() = default;
    void getMsg();
    void getString(std::string &);
    void sendMsg(std::string &);
    int getInstruction();
};